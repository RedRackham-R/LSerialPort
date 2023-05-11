#include "include/Android/AndroidLog.h"
#include "include/LSerialPort/ReadWriteWorker.h"
#include "include/CppLinuxSerial/SerialPort.h"
#include "include/LSerialPort/LSerialPortManager.h"
#include "include/LSerialPort/WriteWorker.h"
#include "include/LSerialPort/ReadWorker.h"
#include "include/LSerialPort/SyncReadWriteWorker.h"

using namespace mn::CppLinuxSerial;

namespace LSerialPort {

    LSerialPortManager::LSerialPortManager() = default;

    int LSerialPortManager::addSyncReadWriteDevice(std::string &path, BaudRate &baudRate,
                                                   NumDataBits &dataBits, Parity &parity,
                                                   NumStopBits &stopBits,
                                                   int32_t &readTimeoutMills) {
        //检查串口是否存在
        if (hasDevice(path)) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port [%s] has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readTimeoutMills < -1) {
            THROW_EXCEPT("read timeout mills  was < -1, which is invalid.")
        }

        //串口读取数据超时范围
        if (readTimeoutMills > 25500) {
            THROW_EXCEPT("read timeout mills  was > 25500, which is invalid.")
        }
        // worker_ptr 是占有 ReadWorker 的 unique_ptr
        std::unique_ptr<SyncReadWriteWorker> worker_ptr = std::make_unique<SyncReadWriteWorker>(
                path,
                baudRate,
                dataBits,
                parity,
                stopBits,
                readTimeoutMills);

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

    int LSerialPortManager::addReadOnlyDevice(std::string &path, BaudRate &baudRate,
                                              NumDataBits &dataBits, Parity &parity,
                                              NumStopBits &stopBits,
                                              int32_t &readIntervalTimeoutMills,
                                              long &checkIntervalWaitMills) {
        //检查串口是否存在
        if (hasDevice(path)) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port [%s] has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1) {
            THROW_EXCEPT("read timeout mills  was < -1, which is invalid.")
        }

        //串口读取数据超时范围
        if (readIntervalTimeoutMills > 25500) {
            THROW_EXCEPT("read timeout mills  was > 25500, which is invalid.")
        }

        //检查串口是否有数据轮循超时时间
        if (checkIntervalWaitMills < 0) {
            THROW_EXCEPT("check timeout mills must be greater than or equal to 0!");
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
            LOGE("serial port [%s] has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1) {
            THROW_EXCEPT("read timeout mills  was < -1, which is invalid.")
        }

        //串口读取数据超时范围
        if (readIntervalTimeoutMills > 25500) {
            THROW_EXCEPT("read timeout mills  was > 25500, which is invalid.")
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
            LOGE("serial port [%s] has been opened, please do not open the serial port repeatedly",
                 path_char);
            return -1;
        }
        //串口读取数据超时范围
        if (readIntervalTimeoutMills < -1) {
            THROW_EXCEPT("read timeout mills  was < -1, which is invalid.")
        }

        //串口读取数据超时范围
        if (readIntervalTimeoutMills > 25500) {
            THROW_EXCEPT("read timeout mills  was > 25500, which is invalid.")
        }

        //检查串口是否有数据轮循超时时间
        if (checkIntervalWaitMills < 0) {
            THROW_EXCEPT("check timeout mills must be greater than or equal to 0!")
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

    void LSerialPortManager::sendMessage(const std::string &path, const std::vector<uint8_t> &msg) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *rwWroker = dynamic_cast<ReadWriteWorker *>(worker);
            if (rwWroker != nullptr) {
                rwWroker->doWork(msg);
            } else {
                //尝试动态转换为WriteWorker
                auto *wWroker = dynamic_cast<WriteWorker *>(worker);
                if (wWroker != nullptr) {
                    wWroker->doWork(msg);
                } else {
                    THROW_EXCEPT(
                            "unable to send message, Because the serial port [\"" + path +
                            "\"] is opened as read-only.")
                }
            }
        } else {
            THROW_EXCEPT(
                    "failed to send message, please open serial port \"" + path + "\"first")
        }
    }

    void LSerialPortManager::setLSerialPortDataListener(const std::string &path,
                                                        jobject *listener) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *rwWroker = dynamic_cast<ReadWriteWorker *>(worker);
            if (rwWroker != nullptr) {
                rwWroker->setLSerialPortDataListener(listener);
            } else {
                //尝试动态转换为ReadWorker
                auto *rWroker = dynamic_cast<ReadWorker *>(worker);
                if (rWroker != nullptr) {
                    rWroker->setLSerialPortDataListener(listener);
                } else {
                    THROW_EXCEPT(
                            "unable to set listener,  because the serial port [\"" + path +
                            "\"] is opened as write-only.")
                }
            }
        } else {
            THROW_EXCEPT(
                    "failed to set listener, please open serial port [\"" + path + "\"] first")
        }
    }

    std::vector<uint8_t>
    LSerialPortManager::readMessageSync(const std::string &path) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *syncRWWroker = dynamic_cast<SyncReadWriteWorker *>(worker);
            if (syncRWWroker != nullptr) {
                return syncRWWroker->read();
            } else {
                THROW_EXCEPT(
                        "unable to read messages by synchronous thread, because the serial port \"" +
                        path +
                        "\". is currently being read and written by asynchronous thread.please use the [addSyncReadWriteDevice] function to open the serial port");
            }
        } else {
            THROW_EXCEPT(
                    "failed to read message, please open serial port [\"" + path + "\"] first")
        }
    }

    void LSerialPortManager::writeMessageSync(const std::string &path,
                                              const std::vector<uint8_t> &msg) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *syncRWWroker = dynamic_cast<SyncReadWriteWorker *>(worker);
            if (syncRWWroker != nullptr) {
                syncRWWroker->doWork(msg);
            } else {
                THROW_EXCEPT(
                        "unable to send messages by synchronous thread, because the serial port [\"" +
                        path +
                        "\"]. is currently being read and written by asynchronous thread.please use the [addSyncReadWriteDevice] function to open the serial port");
            }
        } else {
            THROW_EXCEPT(
                    "failed to check data available, please open serial port [\"" + path +
                    "\"] first")
        }
    }

    bool LSerialPortManager::dataAvaliableSync(const std::string &path) {
        if (hasDevice(path)) {
            IWorker *worker = _mDevices[path].get();
            //尝试动态转换为ReadWriteWorker
            auto *syncRWWroker = dynamic_cast<SyncReadWriteWorker *>(worker);
            if (syncRWWroker != nullptr) {
                return syncRWWroker->dataAvaliable();
            } else {
                THROW_EXCEPT(
                        "unable to check data available, because the serial port is currently being read and written by asynchronous thread.please use the [addSyncReadWriteDevice] function to open the serial port");
            }
        } else {
            THROW_EXCEPT(
                    "failed to check data available, please open serial port [\"" + path +
                    "\"] first")
        }
    }

    LSerialPortManager::~LSerialPortManager() {
        for (auto &device: _mDevices) {
            removeDevice(device.first);
        }
    }
}

