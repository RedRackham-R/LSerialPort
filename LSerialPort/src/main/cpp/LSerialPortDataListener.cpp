#include "include/LSerialPort/LSerialPortDataListener.h"
#include "queue"
#include "include/Android/AndroidLog.h"
#include "include/CppLinuxSerial/Exception.h"

namespace LSerialPort {
    LSerialPortDataListener::LSerialPortDataListener(jobject *jListener) {
        _jListener = *jListener;
    }

    LSerialPortDataListener::~LSerialPortDataListener() = default;


    void LSerialPortDataListener::recycle(JNIEnv *env) {
        //删除释放全局引用对象
        env->DeleteGlobalRef(_jListener);
        _jListener = nullptr;
        //GetMethodID仅用于在JNI环境中查找方法并返回对应的jmethodID，它不会分配内存或创建对象，所以不需要用env显示删除对象
        _onDataReceivedMethod = nullptr;
    }


    void LSerialPortDataListener::onDataReceived(JNIEnv *env, const std::vector<uint8_t> &msg) {
        if (_onDataReceivedMethod == nullptr) {
            jclass listenerClass = env->GetObjectClass(_jListener);
            _onDataReceivedMethod = env->GetMethodID(listenerClass, "onDataReceived", "([B)V");
            env->DeleteLocalRef(listenerClass);
        }

        auto size = static_cast<uint32_t>(msg.size());
        jbyteArray jData;
        if (size > static_cast<uint32_t>(std::numeric_limits<jsize>::max())) {
            LOGE("data size is too big start splite send data!");
            // 数据过大，需要分段拷贝
            constexpr uint32_t chunkSize = std::numeric_limits<jsize>::max();
            jData = env->NewByteArray(chunkSize);
            uint32_t offset = 0;
            while (offset < size) {
                const uint32_t copySize = std::min(size - offset, chunkSize);
                env->SetByteArrayRegion(jData, 0, copySize,
                                        reinterpret_cast<const jbyte *>(msg.data() + offset));
                offset += copySize;
                env->CallVoidMethod(_jListener, _onDataReceivedMethod, jData);
                env->DeleteLocalRef(jData);
            }

        } else {
            // 直接拷贝到jbyteArray中
            if (env == nullptr) {
                LOGE("JNIEnv pointer is null!");
                return;
            }
            jData = env->NewByteArray(static_cast<jsize>(size));
            env->SetByteArrayRegion(jData, 0, static_cast<jsize>(size),
                                    reinterpret_cast<const jbyte *>(msg.data()));
            env->CallVoidMethod(_jListener, _onDataReceivedMethod, jData);
            env->DeleteLocalRef(jData);
        }

    }


}
