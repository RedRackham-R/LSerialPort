//
// Created by lxy on 2023/5/10.
//

#ifndef LSERIALPORT_SYNCREADWRITEWORKER_H
#define LSERIALPORT_SYNCREADWRITEWORKER_H


#include <unordered_map>
#include <jni.h>
#include "iostream"
#include "future"
#include "thread"
#include "queue"
#include "mutex"
#include "../CppLinuxSerial/SerialPort.h"
#include "../LSerialPort/IWorker.h"


using namespace mn::CppLinuxSerial;

namespace LSerialPort {
    class SyncReadWriteWorker : public IWorker {

    public:
        /**
         * * 构造函数 创建对象后启动工作线程
         * @param path 串口地址
         * @param baudRate 波特率
         * @param dataBits 数据位
         * @param parity 校验位
         * @param stopBits 停止位
         * @param readTimeoutMills CPPLinuxSerial 读串口超时时间  单位：ms
         */
        SyncReadWriteWorker(
                const std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readTimeoutMills);

        ~SyncReadWriteWorker() override;

        void doWork(const std::vector<uint8_t> &msg) override;

        void interrupte() override {}

        /**
         * 读取内容
         * @return
         */
        std::vector<uint8_t> read();
        
        bool isOpened() override;

        /**
         * 检查是否有数据
         * @return
         */
        bool dataAvaliable();

    private:
        //串口对象
        SerialPort *_serialPort = nullptr;
    };
}


#endif //LSERIALPORT_SYNCREADWRITEWORKER_H
