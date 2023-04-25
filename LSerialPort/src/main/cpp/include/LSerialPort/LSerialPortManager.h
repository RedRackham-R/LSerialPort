//
// Created by lxy on 2023/4/18.
//

#ifndef LSERIALPORT_LSERIALPORTMANAGER_H
#define LSERIALPORT_LSERIALPORTMANAGER_H

#include <unordered_map>
#include <jni.h>
#include "ReadWriteWorker.h"

using namespace mn::CppLinuxSerial;

namespace LSerialPort {
    class LSerialPortManager {
    public:
        static JavaVM *jvm;

        LSerialPortManager();

        ~LSerialPortManager();

        /**
         * 添加设备
         * @param path 串口地址
         * @param baudRate 波特率
         * @param dataBits 数据位
         * @param parity 校验位
         * @param stopBits 停止位
         * @param readIntervalTimeoutMills 循环读SerialPort超时时间，超过时间就继续下一次轮循
         * @param checkIntervalWaitMills 循环检查等待时间，如果想要回复多一些数据可以适当延长
         * @return
         */
        int addDevice(
                std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readIntervalTimeoutMills,
                long &checkIntervalWaitMills);

        /**
         * 删除设备
         * @param path 串口地址
         * @return
         */
        int removeDevice(const std::string &path);

        /**
         * 检查串口是否被加载过
         * @param path 串口地址
         * @return
         */
        bool hasDevice(const std::string &path);

        /**
         * 发送二进制消息
         * @param path 串口地址
         * @param msg 二进制消息
         * @return
         */
        int sendMessage(const std::string &path, const std::vector<uint8_t> &msg);

        /**
         * 设置串口数据监听器
         * @param path 串口地址
         * @param listener 监听器
         * @return
         */
        int setLSerialPortDataListener(const std::string &path, jobject *listener);


    private:
        //已加载的串口设备
        std::unordered_map<std::string, std::unique_ptr<ReadWriteWorker>> _mDevices;
    };
}


#endif //LSERIALPORT_LSERIALPORTMANAGER_H
