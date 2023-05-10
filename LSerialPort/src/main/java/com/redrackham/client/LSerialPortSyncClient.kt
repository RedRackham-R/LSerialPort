package com.redrackham.client

import com.redrackham.*

class LSerialPortSyncClient private constructor(
    override val path: String,
    override val baudrate: Int,
    override val dataBits: Int,
    override val parity: Int,
    override val stopBits: Int,
    val readTimeoutMills: Int
) : BaseSerialPortClient(path, baudrate, dataBits, parity, stopBits) {

    companion object {
        private const val DEF_READ_TIME_OUT = -1
    }

    override fun open(): Int =
        LSerialPortJNI.native_openSerialPortSync(
            path, baudrate, dataBits, parity, stopBits, readTimeoutMills
        )


    override fun close(): Int = LSerialPortJNI.native_closeSerialPort(path)

    override fun hasOpen(): Boolean = LSerialPortJNI.native_hasOpen(path)


    fun read(): ByteArray = LSerialPortJNI.native_syncRead(path)

    fun write(msg: ByteArray): Int = LSerialPortJNI.native_syncWrite(path, msg)


    class Builder constructor(val path: String) {

        private var baudrate: Int? = DEF_BAUDRATE
        private var dataBits: Int? = DEF_DATABITS
        private var parity: Int? = DEF_PARITY
        private var stopBits: Int? = DEF_STOPBITS
        private var readTimeoutMills: Int? = DEF_READ_TIME_OUT

        fun baudrate(@BaudRate baudrate: Int?) = apply { this.baudrate = baudrate }
        fun dataBits(@DataBits dataBits: Int?) = apply { this.dataBits = dataBits }
        fun parity(@Parity parity: Int?) = apply { this.parity = parity }
        fun stopBits(@StopBits stopBits: Int?) = apply { this.stopBits = stopBits }
        fun readTimeoutMills(readTimeoutMills: Int?) =
            apply { this.readTimeoutMills = readTimeoutMills }

        fun build() = LSerialPortSyncClient(
            path, baudrate!!, dataBits!!, parity!!, stopBits!!, readTimeoutMills!!
        )
    }
}