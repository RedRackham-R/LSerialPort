//
// Created by lxy on 2023/5/4.
//

#ifndef LSERIALPORT_WRITEWORKER_H
#define LSERIALPORT_WRITEWORKER_H

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
     * 只写串口封装类 WriteWorker
     */
    class WriteWorker : public IWorker {

    public:
        WriteWorker(const std::string &path,
                    BaudRate &baudRate,
                    NumDataBits &dataBits,
                    Parity &parity,
                    NumStopBits &stopBits,
                    int32_t &readIntervalTimeoutMills);


        ~WriteWorker() override;

        void doWork(const std::vector<uint8_t> &msg) override;

        /**
         * 退出任务
         */
        void interrupte() override {
            //消息锁
            std::lock_guard<std::mutex> lk(_mMsgMutex);
            //发送退出信号
            IWorker::interrupte();
            _mMsgCond.notify_all();
        }

        /**
         * 串口是否打开
         * @return
         */
        bool isOpened() override;

    private:
        //串口对象
        SerialPort *_serialPort = nullptr;
        //写线程
        std::thread *_writeThread = nullptr;
        //消息锁
        std::mutex _mMsgMutex;
        //消息队列
        std::queue<std::vector<uint8_t>> _mMsgQueue;
        //消息通知
        std::condition_variable _mMsgCond;

        /**
         * 写循环
         */
        void writeLoop();

    };


}


#endif //LSERIALPORT_WRITEWORKER_H
