#include <jni.h>
#include <string>
#include "iostream"
#include "include/LSerialPort/LSerialPortManager.h"
#include "include/LSerialPort/Convert.h"

using namespace LSerialPort;

//串口管理操作
static std::unique_ptr<LSerialPort::LSerialPortManager> mLSerialPortManager;

//初始化jvm
JavaVM *LSerialPortManager::jvm = nullptr;

// JNI_OnLoad函数实现
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    //赋值jvm
    mLSerialPortManager->jvm = vm;
    auto manager_ptr = std::make_unique<LSerialPortManager>();
    mLSerialPortManager = std::move(manager_ptr);

    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        std::__throw_runtime_error("Get java env failed!");
    }
    //一定要返回版本号，否则会出错。
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1openSerialPortReadOnly(JNIEnv *env, jobject thiz,
                                                               jstring path, jint baudrate,
                                                               jint data_bits, jint parity,
                                                               jint stop_bits,
                                                               jlong check_interval_wait_mills) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);
    BaudRate br = convertBaudRate(baudrate);
    NumDataBits db = convertDataBits(data_bits);
    Parity p = convertParity(parity);
    NumStopBits sb = convertStopBits(stop_bits);

    //这里read_interval_timeout_mills设置为0，因为使用独立线程去检查。不需要配置等待时间了
    int32_t rtm_32 = 0;

    //jlong转为long
    auto ctm_ll = static_cast<long long>(check_interval_wait_mills);
    long ctm_l = static_cast<long>(ctm_ll);
    int result = mLSerialPortManager->addReadOnlyDevice(path_str, br, db, p, sb, rtm_32, ctm_l);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}




extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1openSerialPortWriteOnly(JNIEnv *env, jobject thiz,
                                                                jstring path, jint baudrate,
                                                                jint data_bits, jint parity,
                                                                jint stop_bits) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);
    BaudRate br = convertBaudRate(baudrate);
    NumDataBits db = convertDataBits(data_bits);
    Parity p = convertParity(parity);
    NumStopBits sb = convertStopBits(stop_bits);
    //这里read_interval_timeout_mills设置为0，因为使用独立线程去检查。不需要配置等待时间了
    int32_t rtm_32 = 0;
    int result = mLSerialPortManager->addWriteOnlyDevice(path_str, br, db, p, sb, rtm_32);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}



extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1openSerialPort(JNIEnv *env, jobject thiz, jstring path,
                                                       jint baudrate, jint data_bits, jint parity,
                                                       jint stop_bits,
                                                       jlong check_interval_wait_mills) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);
    BaudRate br = convertBaudRate(baudrate);
    NumDataBits db = convertDataBits(data_bits);
    Parity p = convertParity(parity);
    NumStopBits sb = convertStopBits(stop_bits);

    //这里read_interval_timeout_mills设置为0，因为使用独立线程去检查。不需要配置等待时间了
    int32_t rtm_32 = 0;

    //jlong转为long
    auto ctm_ll = static_cast<long long>(check_interval_wait_mills);
    long ctm_l = static_cast<long>(ctm_ll);
    int result = mLSerialPortManager->addDevice(path_str, br, db, p, sb, rtm_32, ctm_l);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1sendMsg(JNIEnv *env, jobject thiz, jstring path,
                                                jbyteArray msg) {
    if (msg == nullptr) {
        return -1;
    }
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    std::vector<uint8_t> msg_vec = convertJByteArrayToVectorU8(env, msg);
    int result = mLSerialPortManager->sendMessage(path_char, msg_vec);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1setLSerialPortDataListener(JNIEnv *env, jobject thiz,
                                                                   jstring path,
                                                                   jobject listener) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);

    jobject jListener = env->NewGlobalRef(listener);
    int result = mLSerialPortManager->setLSerialPortDataListener(path_str, &jListener);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_redrackham_LSerialPort_native_1closeSerialPort(JNIEnv *env, jobject thiz, jstring path) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);
    int result = mLSerialPortManager->removeDevice(path_str);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);
    return result;
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_redrackham_LSerialPort_native_1hasOpen(JNIEnv *env, jobject thiz, jstring path) {
    const char *path_char = env->GetStringUTFChars(path, nullptr);
    auto path_str = std::string(path_char);
    bool result = mLSerialPortManager->hasDevice(path_str);
    //释放资源
    env->ReleaseStringUTFChars(path, path_char);

    return result;
}



