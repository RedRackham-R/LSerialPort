#ifndef LSERIALPORT_READWRITEWORKER_H
#define LSERIALPORT_READWRITEWORKER_H

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
    class ReadWriteWorker : public IWorker {

    public:
        ReadWriteWorker(
                const std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readIntervalTimeoutMills,
                long &checkIntervalWaitMills);

        ~ReadWriteWorker();

        void doWork(const std::vector<uint8_t> &msg) override;

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
        bool isOpened();

    private:
        //串口对象
        SerialPort *_serialPort = nullptr;
        //读线程
        std::thread *_readThread = nullptr;
        //写线程
        std::thread *_writeThread = nullptr;
        //检查数据是否可见
        std::thread *_checkAvaliableThread = nullptr;
        //当前消息回调
        LSerialPortDataListener *_curlistener = nullptr;
        //预备消息回调
        LSerialPortDataListener *_prepListener = nullptr;

        JNIEnv *_env;
        //消息锁
        std::mutex _mMsgMutex;
        //监听器锁
        std::mutex _mListenerMutex;
        //消息队列
        std::queue<std::vector<uint8_t>> _mMsgQueue;
        //消息通知
        std::condition_variable _mMsgCond;
        //读取缓存是否有数据
        std::atomic<bool> _dataAvailable = false;
        //检查是否有数据等待时间
        long _checkIntervalWaitMills;


        /**
         * 写循环
         */
        void writeLoop();

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


#endif //LSERIALPORT_READWRITEWORKER_H
