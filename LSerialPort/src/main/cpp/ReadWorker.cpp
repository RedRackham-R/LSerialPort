#include "include/LSerialPort/ReadWorker.h"
#include "include/Android/AndroidLog.h"
#include "include/LSerialPort/LSerialPortManager.h"


namespace LSerialPort {

    ReadWorker::ReadWorker(
            const std::string &path,
            BaudRate &baudRate,
            NumDataBits &dataBits,
            Parity &parity,
            NumStopBits &stopBits,
            int32_t &readIntervalTimeoutMills,
            long &checkIntervalWaitMills) {

        _serialPort = new SerialPort(path, baudRate, dataBits, parity, stopBits);
        //设置读取超时时间 这个参数慢慢调看效果 取值范围 -1 ~ 25500   -1表示无限超时等待 直到有数据返回，0标识表示立即返回任何数据，>0则等待x毫秒后会取消阻塞
        _serialPort->SetTimeout(readIntervalTimeoutMills);
        _serialPort->Open();
        if (_serialPort->GetState() == State::OPEN) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port [%s] open success ! ", path_char);
            _checkIntervalWaitMills = checkIntervalWaitMills;
            //读线程
            _readThread = new std::thread(&ReadWorker::readLoop, this);
            //检查读数据线程
            _checkAvailableThread = new std::thread(&ReadWorker::checkAvailableLoop, this);

        } else {
            LOGE("serial port open fail!");
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
        }
    }

    void ReadWorker::checkAvailableLoop() {
        LOGE("start check available Loop");
        while (!isInterrupted()) {
            if (_serialPort->GetState() == State::OPEN && _serialPort->Available() > 0) {
                _dataAvailable.store(true);
                _mMsgCond.notify_all();
            }
            //这里设置毫秒等待时间
            std::this_thread::sleep_for(std::chrono::milliseconds(_checkIntervalWaitMills));
        }
    }


    void ReadWorker::readLoop() {
        LOGE("start read loop");
        // 等待直至收到发送数据
        std::unique_lock<std::mutex> lk(_mMsgMutex);
        //C++线程首次访问JEnv函数需要先使用AttachCurrentThread把当前线程附加到JVM上下文才能使用JEnv的函数、
        //Jni的native函数不需要是因为是Jvm线程调用函数，所以不需要附加，C++创建线程不数据Jvm，所以要进行关联附加才能使用Jni的函数
        if (LSerialPortManager::jvm->AttachCurrentThread(&_env, nullptr) != JNI_OK) {
            return THROW_EXCEPT("unable to get jvm instance");
        }
        while (!isInterrupted()) {
            //这里等待需要传入当前对象指针，传this会复制当前对象指针而&只会获得当前对象指针引用，
            //如果在lambda表达式中修改了this内容会使得修改无效，一般建议用地址符&
            _mMsgCond.wait(lk, [&] { return (isInterrupted() || _dataAvailable.load()); });
            if (isInterrupted()) {
                break;
            }
            //读取二进制
            std::vector<uint8_t> msg_vec;
            //读取数据
            _serialPort->ReadBinary(msg_vec);
            //重置标记
            _dataAvailable.store(false);
            if (msg_vec.empty()) {
                //没数据视为超时，进行下一轮循环
                continue;
            }
            //设置监听器锁
            std::lock_guard<std::mutex> llk(_mListenerMutex);
            if (_prepListener != nullptr) {
                //如果之前已经设置过了，先回收删除上一个listener
                if (_curlistener != nullptr) {
                    LOGE("there is currently a listener and it is being recycled...");
                    _curlistener->recycle(_env);
                    delete _curlistener;
                }
                _curlistener = _prepListener;
                _prepListener = nullptr;
            }

            if (_curlistener != nullptr) {
                _curlistener->onDataReceived(_env, msg_vec);
            }

        }
        if (_prepListener != nullptr) {
            _prepListener->recycle(_env);
        }

        if (_curlistener != nullptr) {
            _curlistener->recycle(_env);
        }
        //退出当前线程
        LSerialPortManager::jvm->DetachCurrentThread();
    }


    bool ReadWorker::isOpened() {
        return (_serialPort != nullptr && _serialPort->GetState() == State::OPEN);
    }


    void ReadWorker::setLSerialPortDataListener(
            jobject *listener) {
        //设置监听器锁，避免操作回调时因为设置监听器而出问题
        std::lock_guard<std::mutex> llk(_mListenerMutex);
        _prepListener = new LSerialPortDataListener(listener);
    }


    ReadWorker::~ReadWorker() {
        //标记线程结束
        ReadWorker::interrupte();

        //等待检查线程结束
        if ((_checkAvailableThread != nullptr) && _checkAvailableThread->joinable()) {
            _checkAvailableThread->join();
        }

        //等待读线程结束
        if ((_readThread != nullptr) && _readThread->joinable()) {
            _readThread->join();
        }

        //回收线程监听器内存对象 清理指针
        delete _checkAvailableThread;
        delete _readThread;
        _checkAvailableThread = nullptr;
        _readThread = nullptr;

        if (_prepListener != nullptr) {
            delete _prepListener;
            _prepListener = nullptr;
        }

        if (_curlistener != nullptr) {
            delete _curlistener;
            _curlistener = nullptr;
        }

        //关闭串口
        if (_serialPort != nullptr) {
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
        }
        LOGE("serialport closed");
    }

}