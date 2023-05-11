package com.redrackham.client

import com.redrackham.*

/**
 * LSerialPortSyncClient 串口同步读写客户端
 * 同步线程中读写串口数据，提供给想要自己管理收发线程的用户使用
 */
class LSerialPortSyncClient private constructor(
    //串口地址
    override val path: String,
    //波特率
    override val baudrate: Int,
    //数据位
    override val dataBits: Int,
    //校验位
    override val parity: Int,
    //停止位
    override val stopBits: Int,
    //读取数据超时时间 -1:一直等待    0:无论有没有数据立即返回    x>0:等待多少x毫秒   默认是一直等待
    val readTimeoutMills: Int
) : BaseSerialPortClient(path, baudrate, dataBits, parity, stopBits) {

    companion object {
        //默认一直等待
        private const val DEF_READ_TIME_OUT = -1
    }

    /**
     * 打开串口
     */
    override fun open(): Int = LSerialPortJNI.native_openSerialPortSync(
        path, baudrate, dataBits, parity, stopBits, readTimeoutMills
    )

    /**
     * 关闭串口
     */
    override fun close(): Int = LSerialPortJNI.native_closeSerialPort(path)

    /**
     * 串口是否打开
     */
    override fun hasOpen(): Boolean = LSerialPortJNI.native_hasOpen(path)

    /**
     * 是否有接收到串口数据
     */
    fun dataAvailable(): Boolean = LSerialPortJNI.native_syncDataAvailable(path)

    /**
     * 读数据
     */
    fun read(): ByteArray = LSerialPortJNI.native_syncRead(path)

    /**
     * 写数据
     */
    fun write(data: ByteArray?) = LSerialPortJNI.native_syncWrite(path, data)

    class Builder constructor(val path: String) {
        private var baudrate: Int = DEF_BAUDRATE
        private var dataBits: Int = DEF_DATABITS
        private var parity: Int = DEF_PARITY
        private var stopBits: Int = DEF_STOPBITS
        private var readTimeoutMills: Int = DEF_READ_TIME_OUT

        fun baudrate(@BaudRate baudrate: Int) = apply { this.baudrate = baudrate }
        fun dataBits(@DataBits dataBits: Int) = apply { this.dataBits = dataBits }
        fun parity(@Parity parity: Int) = apply { this.parity = parity }
        fun stopBits(@StopBits stopBits: Int) = apply { this.stopBits = stopBits }
        fun readTimeoutMills(readTimeoutMills: Int) =
            apply { this.readTimeoutMills = readTimeoutMills }

        fun build() = LSerialPortSyncClient(
            path, baudrate, dataBits, parity, stopBits, readTimeoutMills
        )
    }
}