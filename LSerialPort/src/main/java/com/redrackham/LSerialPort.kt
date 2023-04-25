package com.redrackham

object LSerialPort {

    private val TAG = LSerialPort::class.java.simpleName


    private const val DEF_DATABITS = DataBits.EIGHT
    private const val DEF_PARITY = Parity.NONE
    private const val DEF_STOPBITS = StopBits.ONE
    private const val DEF_READ_INTERVAL_TIMEOUT = 0L
    private const val DEF_CHECK_INTERVAL_WAIT_MILLS = 0L

    init {
        System.loadLibrary("lserialport")
    }


    @JvmOverloads
    fun openSerialPort(
        path: String,
        @BaudRate baudrate: Int,
        @DataBits dataBits: Int = DEF_DATABITS,
        @Parity parity: Int = DEF_PARITY,
        @StopBits stopBits: Int = DEF_STOPBITS,
        checkIntervalWaitMills: Long = DEF_CHECK_INTERVAL_WAIT_MILLS
    ): Int {
        return native_openSerialPort(
            path,
            baudrate,
            dataBits,
            parity,
            stopBits,
            checkIntervalWaitMills
        )
    }


    fun sendMsg(path: String, msg: ByteArray): Int {
        return native_sendMsg(path, msg)
    }

    fun hasOpen(path: String): Boolean = native_hasOpen(path)

    fun setOnLSerialPortListener(
        path: String, listener: LSerialPortDataListener
    ): Int = native_setLSerialPortDataListener(path, listener)


    fun closeSerialPort(
        path: String,
    ): Int = native_closeSerialPort(path)


    external fun native_hasOpen(path: String): Boolean


    external fun native_sendMsg(path: String, msg: ByteArray): Int


    external fun native_setLSerialPortDataListener(
        path: String, listener: LSerialPortDataListener
    ): Int

    external fun native_openSerialPort(
        path: String,
        baudrate: Int,
        dataBits: Int,
        parity: Int,
        stopBits: Int,
        checkIntervalWaitMills: Long
    ): Int

    external fun native_closeSerialPort(
        path: String,
    ): Int
}