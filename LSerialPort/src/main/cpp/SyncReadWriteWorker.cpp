
#include "include/LSerialPort/SyncReadWriteWorker.h"
#include "include/Android/AndroidLog.h"

namespace LSerialPort {

    SyncReadWriteWorker::SyncReadWriteWorker(const std::string &path,
                                             BaudRate &baudRate,
                                             NumDataBits &dataBits,
                                             Parity &parity,
                                             NumStopBits &stopBits,
                                             int32_t &readTimeoutMills) {


        _serialPort = new SerialPort(path, baudRate, dataBits, parity, stopBits);
        //设置读取超时时间 这个参数慢慢调看效果 取值范围 -1 ~ 25500   -1表示无限超时等待 直到有数据返回，0标识表示立即返回任何数据，>0则等待x毫秒后会取消阻塞
        _serialPort->SetTimeout(readTimeoutMills);
        _serialPort->Open();
        if (_serialPort->GetState() == State::OPEN) {
            char ch[20];
            char *path_char = strcpy(ch, path.c_str());
            LOGE("serial port [%s] open success ! ", path_char);
        } else {
            LOGE("serial port open fail!");
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
        }
    }

    bool SyncReadWriteWorker::isOpened() {
        return (_serialPort != nullptr && _serialPort->GetState() == State::OPEN);
    }

    void SyncReadWriteWorker::doWork(const std::vector<uint8_t> &msg) {
        _serialPort->WriteBinary(msg);
    }

    std::vector<uint8_t> SyncReadWriteWorker::read() {
        //读取二进制
        std::vector<uint8_t> msg_vec;
        //读取数据
        _serialPort->ReadBinary(msg_vec);
        return msg_vec;
    }

    SyncReadWriteWorker::~SyncReadWriteWorker() {
        LOGE("close SerialPort");
        if (_serialPort != nullptr) {
            LOGE("closing...");
            _serialPort->Close();
            delete _serialPort;
            _serialPort = nullptr;
            LOGE("close done!");
        }
        LOGE("finish done!");
    }


}