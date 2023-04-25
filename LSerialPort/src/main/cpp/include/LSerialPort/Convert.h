#ifndef LSERIALPORT_CONVERT_H
#define LSERIALPORT_CONVERT_H

#include "string"
#include "iostream"
#include "../CppLinuxSerial/SerialPort.h"
#include "../Android/AndroidLog.h"


using namespace mn::CppLinuxSerial;
namespace LSerialPort {


/**
 * jbyteArray转vector<uint8_t>
 * @param env
 * @param bytearray
 * @return
 */
    inline std::vector<uint8_t>
    convertJByteArrayToVectorU8(JNIEnv * env, jbyteArray & jbytearray) {
        std::vector<uint8_t> vec;
        jbyte *jBytes = env->GetByteArrayElements(jbytearray, nullptr);
        jsize length = env->GetArrayLength(jbytearray);
        vec = std::vector<uint8_t>(reinterpret_cast<uint8_t *>(jBytes),
                                   reinterpret_cast<uint8_t *>(jBytes) + length);
        env->ReleaseByteArrayElements(jbytearray, jBytes, JNI_ABORT);
        return vec;
    }

/**
* 停止位转换
* @param stopBits
* @return
*/
    inline NumStopBits convertStopBits(const int &stopBits) {
        NumStopBits sb;
        switch (stopBits) {
            case 1: {
                sb = NumStopBits::ONE;
                break;
            }
            case 2: {
                sb = NumStopBits::TWO;
                break;
            }
            default: {
                THROW_EXCEPT("StopBits value not supported!");
            }
        }
        return sb;
    }


/**
 * 校验位转换
 * @param parity 0:无校验位(NONE，默认)；1:奇校验位(ODD);2:偶校验位(EVEN)
 * @return
 */
    inline Parity convertParity(const int &parity) {

        Parity p;

        switch (parity) {
            case 0: {
                p = Parity::NONE;
                break;
            }
            case 1: {
                p = Parity::ODD;
                break;
            }
            case 2: {
                p = Parity::EVEN;
                break;
            }
            default: {
                THROW_EXCEPT("Parity value not supported!");
            }
        }
        return p;
    }


/**
 * 数据位
 * @param dataBits 数据位 5-8
 * @return
 */
    inline NumDataBits convertDataBits(const int &dataBits) {
        NumDataBits db;
        switch (dataBits) {
            case 5: {
                db = NumDataBits::FIVE;
                break;
            }
            case 6: {
                db = NumDataBits::SIX;
                break;
            }
            case 7: {
                db = NumDataBits::SEVEN;
                break;
            }
            case 8: {
                db = NumDataBits::EIGHT;
                break;
            }
            default: {
                THROW_EXCEPT("DataBits value not supported!");
            }
        }
        return db;
    }


/**
 * 波特率转换
 * @param baudRate 波特率 0-460800
 * @return SerialPort的枚举类型
 */
    inline BaudRate convertBaudRate(const int &baudRate) {
        BaudRate br;
        switch (baudRate) {
            case 0: {
                br = BaudRate::B_0;
                break;
            }
            case 50: {
                br = BaudRate::B_50;
                break;
            }
            case 75: {
                br = BaudRate::B_75;
                break;
            }
            case 110: {
                br = BaudRate::B_110;
                break;
            }
            case 134: {
                br = BaudRate::B_134;
                break;
            }
            case 150: {
                br = BaudRate::B_150;
                break;
            }
            case 200: {
                br = BaudRate::B_200;
                break;
            }
            case 300: {
                br = BaudRate::B_300;
                break;
            }
            case 600: {
                br = BaudRate::B_600;
                break;
            }
            case 1200: {
                br = BaudRate::B_200;
                break;
            }
            case 1800: {
                br = BaudRate::B_1800;
                break;
            }
            case 2400: {
                br = BaudRate::B_2400;
                break;
            }
            case 4800: {
                br = BaudRate::B_4800;
                break;
            }
            case 9600: {
                br = BaudRate::B_9600;
                break;
            }
            case 19200: {
                br = BaudRate::B_19200;
                break;
            }
            case 38400: {
                br = BaudRate::B_38400;
                break;
            }
            case 57600: {
                br = BaudRate::B_57600;
                break;
            }
            case 115200: {
                br = BaudRate::B_115200;
                break;
            }
            case 230400: {
                br = BaudRate::B_230400;
                break;
            }
            case 460800: {
                br = BaudRate::B_460800;
                break;
            }
            default : {
                THROW_EXCEPT("BaudRate value not supported!")
            }
        }

        return br;
    }


}

#endif //LSERIALPORT_CONVERT_H
