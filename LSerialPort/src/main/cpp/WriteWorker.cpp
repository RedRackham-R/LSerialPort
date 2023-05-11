#include "include/LSerialPort/WriteWorker.h"
#include "include/Android/AndroidLog.h"

namespace LSerialPort {

    WriteWorker::WriteWorker(const std::string &path, BaudRate &baudRate, NumDataBits &dataBits,
                             Parity &parity, NumStopBits &stopBits,
                             int32_t &readIntervalTimeoutMills) {
        _serialPort = new SerialPort(path, baudRate, dataBits, parity, stopBits);
        //设置读取超时时间 这个参数慢慢调看效果 取值范围 -1 ~ 25500   -1表示无限超时等待 直到有数据返回，0标识表示立即返回任何数据，>0则等待x毫秒后会取消阻塞
        _serialPort->SetTimeout(readIntervalTimeoutMills);
        _serialPort->Open();
        if (_serialPort->GetState() == State::OPEN) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port [%s] open success ! ", path_char);
            //写线程
            _writeThread = new std::thread(&WriteWorker::writeLoop, this);
        } else {
            LOGE("serial port open fail!");
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
        }
    }

    void WriteWorker::doWork(const std::vector<uint8_t> &msg) {
        std::lock_guard<std::mutex> lk(_mMsgMutex);
        _mMsgQueue.push(msg);
        _mMsgCond.notify_all();
    }


    void WriteWorker::writeLoop() {
        // 等待直至 main() 发送数据
        std::unique_lock<std::mutex> lk(_mMsgMutex);
        LOGE("start write loop");
        while (!isInterrupted()) {
            //这里等待需要传入当前对象指针，传this会复制当前对象指针而&只会获得当前对象指针引用，
            //如果在lambda表达式中修改了this内容会使得修改无效，一般建议用地址符&
            _mMsgCond.wait(lk, [&] { return (isInterrupted() || !_mMsgQueue.empty()); });
            if (isInterrupted()) {
                break;
            }
            std::vector<uint8_t> msg = std::move(_mMsgQueue.front());
            _serialPort->WriteBinary(msg);
            _mMsgQueue.pop();
        }
    }


    bool WriteWorker::isOpened() {
        return (_serialPort != nullptr && _serialPort->GetState() == State::OPEN);
    }


    WriteWorker::~WriteWorker() {
        //标记线程结束
        WriteWorker::interrupte();

        //等待检查线程结束
        if ((_writeThread != nullptr) && _writeThread->joinable()) {
            _writeThread->join();//等待写线程结束
        }

        // 清空队列
        while (!_mMsgQueue.empty()) {
            _mMsgQueue.pop();
        }

        //回收内存对象 清理指针
        delete _writeThread;
        _writeThread = nullptr;

        //关闭串口
        if (_serialPort != nullptr) {
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
        }
        LOGE("serialport closed");
    }
}