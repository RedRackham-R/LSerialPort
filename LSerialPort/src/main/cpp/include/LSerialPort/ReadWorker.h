#ifndef LSERIALPORT_READWORKER_H
#define LSERIALPORT_READWORKER_H

#include <unordered_map>
#include <jni.h>
#include "iostream"
#include "future"
#include "thread"
#include "queue"
#include "mutex"
#include "../CppLinuxSerial/SerialPort.h"
#include "../LSerialPort/IWorker.h"
#include "../LSerialPort/LSerialPortDataListener.h"

using namespace mn::CppLinuxSerial;

namespace LSerialPort {
    /**
     * 读写串口封装类 ReadWriteWorker
     */
    class ReadWorker : public IWorker {

    public:
        /**
         * 构造函数 创建对象后启动工作线程
         * @param path 串口地址
         * @param baudRate 波特率
         * @param dataBits 数据位
         * @param parity 校验位
         * @param stopBits 停止位
         * @param readIntervalTimeoutMills CPPLinuxSerial 读串口超时时间  单位：ms
         * @param checkIntervalWaitMills 检查是否有数据循环间隔等待时间 单位：ms
         */
        ReadWorker(
                const std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readIntervalTimeoutMills,
                long &checkIntervalWaitMills);

        ~ReadWorker() override;

        /**
         * 只读无需发送消息，这里直接空实现函数
         * @param msg
         */
        void doWork(const std::vector<uint8_t> &msg) override {

        }

        /**
         * 退出任务
         */
        void interrupte() override {
            //监听器锁
            std::lock_guard<std::mutex> llk(_mListenerMutex);
            //消息锁
            std::lock_guard<std::mutex> lk(_mMsgMutex);
            //发送退出信号
            IWorker::interrupte();
            _mMsgCond.notify_all();
        }

        /**
         * 设置监听器
         * @param listener
         */
        void setLSerialPortDataListener(jobject *listener);

        /**
         * 串口是否打开
         * @return
         */
        bool isOpened() override;

    private:
        //串口对象
        SerialPort *_serialPort = nullptr;
        //读线程
        std::thread *_readThread = nullptr;
        //检查数据是否可见
        std::thread *_checkAvailableThread = nullptr;
        //当前消息回调
        LSerialPortDataListener *_curlistener = nullptr;
        //预备消息回调
        LSerialPortDataListener *_prepListener = nullptr;

        JNIEnv *_env{};
        //消息锁
        std::mutex _mMsgMutex;
        //监听器锁
        std::mutex _mListenerMutex;
        //消息通知
        std::condition_variable _mMsgCond;
        //读取缓存是否有数据
        std::atomic<bool> _dataAvailable = false;
        //检查是否有数据等待时间
        long _checkIntervalWaitMills;


        /**
         * 读循环
         */
        void readLoop();

        /**
         * 循环检查是否有数据
         */
        void checkAvailableLoop();
    };

}


#endif //LSERIALPORT_READWORKER_H
