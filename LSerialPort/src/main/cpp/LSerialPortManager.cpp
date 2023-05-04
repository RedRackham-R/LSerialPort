#include "include/Android/AndroidLog.h"
#include "include/LSerialPort/ReadWriteWorker.h"
#include "include/CppLinuxSerial/SerialPort.h"
#include "include/LSerialPort/LSerialPortManager.h"
#include "include/LSerialPort/WriteWorker.h"
#include "include/LSerialPort/ReadWorker.h"

using namespace mn::CppLinuxSerial;

namespace LSerialPort {


    LSerialPortManager::LSerialPortManager() = default;


    int LSerialPortManager::addReadOnlyDevice(std::string &path, BaudRate &baudRate,
                                              NumDataBits &dataBits, Parity &parity,
                                              NumStopBits &stopBits,
                                              int32_t &readIntervalTimeoutMills,
                                              long &checkIntervalWaitMills) {
        //检查串口是否存在
        if (hasDevice(path)) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port %s has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1 || readIntervalTimeoutMills > 25500) {
            LOGE("serial port timeout mills range between -1 and 25500!");
            return -1;
        }

        //检查串口是否有数据轮循超时时间
        if (checkIntervalWaitMills < 0) {
            LOGE("check timeout mills must be greater than or equal to 0!");
            return -1;
        }

        // worker_ptr 是占有 ReadWorker 的 unique_ptr
        std::unique_ptr<ReadWorker> worker_ptr = std::make_unique<ReadWorker>(
                path,
                baudRate,
                dataBits,
                parity,
                stopBits,
                readIntervalTimeoutMills,
                checkIntervalWaitMills);

        if (worker_ptr->isOpened()) {
            // 作为指向基类的指针
            _mDevices[path] = std::move(worker_ptr);
            return 0;
        } else {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("open serial port [%s] fail！", path_char);
            return -1;
        }
    }


    int LSerialPortManager::addWriteOnlyDevice(std::string &path, BaudRate &baudRate,
                                               NumDataBits &dataBits, Parity &parity,
                                               NumStopBits &stopBits,
                                               int32_t &readIntervalTimeoutMills) {
        //检查串口是否存在
        if (hasDevice(path)) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port %s has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1 || readIntervalTimeoutMills > 25500) {
            LOGE("serial port timeout mills range between -1 and 25500!");
            return -1;
        }
        // worker_ptr 是占有 WriteWorker 的 unique_ptr
        std::unique_ptr<WriteWorker> worker_ptr = std::make_unique<WriteWorker>(
                path,
                baudRate,
                dataBits,
                parity,
                stopBits,
                readIntervalTimeoutMills);

        if (worker_ptr->isOpened()) {
            // 作为指向基类的指针
            _mDevices[path] = std::move(worker_ptr);
            return 0;
        } else {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("open serial port [%s] fail！", path_char);
            return -1;
        }

    }


    int LSerialPortManager::addDevice(
            std::string &path,
            BaudRate &baudRate,
            NumDataBits &dataBits,
            Parity &parity,
            NumStopBits &stopBits,
            int32_t &readIntervalTimeoutMills,
            long &checkIntervalWaitMills) {
        //检查串口是否存在
        if (hasDevice(path)) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port %s has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1 || readIntervalTimeoutMills > 25500) {
            LOGE("serial port timeout mills range between -1 and 25500!");
            return -1;
        }

        //检查串口是否有数据轮循超时时间
        if (checkIntervalWaitMills < 0) {
            LOGE("check timeout mills must be greater than or equal to 0!");
            return -1;
        }

        // worker_ptr 是占有 ReadWriteWorker 的 unique_ptr
        std::unique_ptr<ReadWriteWorker> worker_ptr = std::make_unique<ReadWriteWorker>(
                path,
                baudRate,
                dataBits,
                parity,
                stopBits,
                readIntervalTimeoutMills,
                checkIntervalWaitMills);

        if (worker_ptr->isOpened()) {
            // 作为指向基类的指针
            _mDevices[path] = std::move(worker_ptr);
            return 0;
        } else {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("open serial port [%s] fail！", path_char);
            return -1;
        }
    }


    int LSerialPortManager::removeDevice(const std::string &path) {
        if (hasDevice(path)) {
            _mDevices.erase(path);
            return 0;
        }
        char ch[20];
        char *path_char = strcpy(ch, path.c_str());
        LOGE("failed to close the serial port [%s], the serial port is not open", path_char);
        return -1;
    }

    bool LSerialPortManager::hasDevice(const std::string &path) {
        return _mDevices[path] != nullptr;
    }

    int LSerialPortManager::sendMessage(const std::string &path, const std::vector<uint8_t> &msg) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *rwWroker = dynamic_cast<ReadWriteWorker *>(worker);
            if (rwWroker != nullptr) {
                rwWroker->doWork(msg);
                return 0;
            } else {
                //尝试动态转换为WriteWorker
                auto *wWroker = dynamic_cast<WriteWorker *>(worker);
                if (wWroker != nullptr) {
                    wWroker->doWork(msg);
                    return 0;
                } else {
                    char ch[20];
                    char *path_char = strcpy(ch, path.c_str());
                    LOGE("failed to send message, Because the serial port [%s] is opened as read-only",
                         path_char);
                    return -1;
                }
            }
        }
        char ch[20];
        char *path_char = strcpy(ch, path.c_str());
        LOGE("failed to send message, please open serial port [%s] first",
             path_char);
        return -1;
    }


    int LSerialPortManager::setLSerialPortDataListener(const std::string &path,
                                                       jobject *listener) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *rwWroker = dynamic_cast<ReadWriteWorker *>(worker);
            if (rwWroker != nullptr) {
                rwWroker->setLSerialPortDataListener(listener);
                return 0;
            } else {
                //尝试动态转换为ReadWorker
                auto *rWroker = dynamic_cast<ReadWorker *>(worker);
                if (rWroker != nullptr) {
                    rWroker->setLSerialPortDataListener(listener);
                    return 0;
                } else {
                    char ch[20];
                    char *path_char = strcpy(ch, path.c_str());
                    LOGE("failed to set listener, Because the serial port [%s] is opened as write-only",
                         path_char);
                    return -1;
                }
            }
        }
        char ch[20];
        char *path_char = strcpy(ch, path.c_str());
        LOGE("failed to set listener, please open serial port [%s] first",
             path_char);
        return -1;
    }

    LSerialPortManager::~LSerialPortManager() {
        for (auto &device: _mDevices) {
            removeDevice(device.first);
        }
    }
}

