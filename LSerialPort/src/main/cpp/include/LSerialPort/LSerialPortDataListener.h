#ifndef LSERIALPORT_LSERIALPORTDATALISTENER_H
#define LSERIALPORT_LSERIALPORTDATALISTENER_H

#include "jni.h"
#include "string"

namespace LSerialPort {
    class LSerialPortDataListener {
    public:
        /**
         * 添加监听器
         * @param jListener java监听器对象
         */
        LSerialPortDataListener(jobject *jListener);

        ~LSerialPortDataListener();

        /**
         * 数据回调函数
         * @param env JEnv
         * @param msg 读取串口二进制数据
         */
        void onDataReceived(JNIEnv *env, const std::vector<uint8_t> &msg);

        /**
         * 回收创建的全局对象 _jListener
         * @param env
         */
        void recycle(JNIEnv *env);

    private:
        //转换后的全局（GlobalRef）监听器对象
        jobject _jListener = nullptr;
        //数据回调函数
        jmethodID _onDataReceivedMethod = nullptr;
    };
}


#endif //LSERIALPORT_LSERIALPORTDATALISTENER_H
