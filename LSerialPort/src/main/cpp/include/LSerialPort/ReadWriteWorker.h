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
    /**
     * 读写串口封装类 ReadWriteWorker
     */
    class ReadWriteWorker : public IWorker {

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
        ReadWriteWorker(
                const std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readIntervalTimeoutMills,
                long &checkIntervalWaitMills);

        ~ReadWriteWorker() override;

        /**
         * 发送消息函数
         * @param msg 二进制消息
         */
        void doWork(const std::vector<uint8_t> &msg) override;

        /**
         * 退出任务
         */
        void interrupte() override {
            //监听器锁
            std::lock_guard<std::mutex> llk(_mListenerMutex);
            //写锁
            std::lock_guard<std::mutex> rlk(_mReadMutex);
            //读锁
            std::lock_guard<std::mutex> wlk(_mWriteMutex);
            //发送退出信号
            IWorker::interrupte();
            _mReadCond.notify_all();
            _mWriteCond.notify_all();
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
        //写线程
        std::thread *_writeThread = nullptr;
        //检查数据是否可见
        std::thread *_checkAvailableThread = nullptr;
        //当前消息回调
        LSerialPortDataListener *_curlistener = nullptr;
        //预备消息回调
        LSerialPortDataListener *_prepListener = nullptr;

        JNIEnv *_env{};
        //读锁
        std::mutex _mWriteMutex;
        //写锁
        std::mutex _mReadMutex;

        //监听器锁
        std::mutex _mListenerMutex;
        //消息队列
        std::queue<std::vector<uint8_t>> _mMsgQueue;
        //写消息通知
        std::condition_variable _mWriteCond;
        //读消息通知
        std::condition_variable _mReadCond;
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
