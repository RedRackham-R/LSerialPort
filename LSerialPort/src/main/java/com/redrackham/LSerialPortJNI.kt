package com.redrackham

object LSerialPortJNI {

    init {
        System.loadLibrary("lserialport")
    }

    external fun native_hasOpen(path: String): Boolean

    external fun native_sendMsg(path: String, msg: ByteArray?)

    external fun native_setLSerialPortDataListener(
        path: String, listener: LSerialPortDataListener
    )

    external fun native_syncDataAvailable(path: String): Boolean

    external fun native_syncWrite(path: String, data: ByteArray?)

    external fun native_syncRead(path: String): ByteArray

    external fun native_openSerialPortSync(
        path: String,
        baudrate: Int,
        dataBits: Int,
        parity: Int,
        stopBits: Int,
        readTimeoutMills: Int
    ): Int


    external fun native_openSerialPort(
        path: String,
        baudrate: Int,
        dataBits: Int,
        parity: Int,
        stopBits: Int,
        checkIntervalWaitMills: Int
    ): Int

    external fun native_openSerialPortWriteOnly(
        path: String,
        baudrate: Int,
        dataBits: Int,
        parity: Int,
        stopBits: Int,
    ): Int

    external fun native_openSerialPortReadOnly(
        path: String,
        baudrate: Int,
        dataBits: Int,
        parity: Int,
        stopBits: Int,
        checkIntervalWaitMills: Int
    ): Int

    external fun native_closeSerialPort(
        path: String,
    ): Int
}