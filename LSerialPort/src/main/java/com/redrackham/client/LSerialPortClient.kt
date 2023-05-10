package com.redrackham.client

import com.redrackham.*
import com.redrackham.LSerialPortJNI.native_closeSerialPort
import com.redrackham.LSerialPortJNI.native_hasOpen
import com.redrackham.LSerialPortJNI.native_openSerialPort
import com.redrackham.LSerialPortJNI.native_openSerialPortReadOnly
import com.redrackham.LSerialPortJNI.native_openSerialPortWriteOnly
import com.redrackham.LSerialPortJNI.native_sendMsg
import com.redrackham.LSerialPortJNI.native_setLSerialPortDataListener

class LSerialPortClient private constructor(
    override val path: String,
    override val baudrate: Int,
    override val dataBits: Int,
    override val parity: Int,
    override val stopBits: Int,
    val checkIntervalWaitMills: Int,
    val clientType: Int,
) : BaseSerialPortClient(path, baudrate, dataBits, parity, stopBits) {

    companion object {
        private const val DEF_CLIENT_TYPE = MutiThreadClientType.READ_WRITE
        private const val DEF_CHECK_INTERVAL_WAIT_MILLS = 0
    }

    override fun open(): Int {
        return when (clientType) {
            MutiThreadClientType.READ_WRITE -> native_openSerialPort(
                path, baudrate, dataBits, parity, stopBits, checkIntervalWaitMills
            )
            MutiThreadClientType.ONLY_READ -> native_openSerialPortReadOnly(
                path, baudrate, dataBits, parity, stopBits, checkIntervalWaitMills
            )
            MutiThreadClientType.ONLY_WRITE -> native_openSerialPortWriteOnly(
                path,
                baudrate,
                dataBits,
                parity,
                stopBits,
            )
            else -> error("Invalid argument: $clientType")
        }
    }


    override fun close(): Int = native_closeSerialPort(path)

    override fun hasOpen(): Boolean = native_hasOpen(path)

    fun setListener(listener: LSerialPortDataListener): Int =
        native_setLSerialPortDataListener(path, listener)

    fun sendMsg(msg: ByteArray): Int = native_sendMsg(path, msg)


    class Builder constructor(val path: String) {

        private var baudrate: Int? = DEF_BAUDRATE
        private var dataBits: Int? = DEF_DATABITS
        private var parity: Int? = DEF_PARITY
        private var stopBits: Int? = DEF_STOPBITS
        private var clientType: Int? = DEF_CLIENT_TYPE
        private var checkIntervalWaitMills: Int? = DEF_CHECK_INTERVAL_WAIT_MILLS

        fun baudrate(@BaudRate baudrate: Int?) = apply { this.baudrate = baudrate }
        fun dataBits(@DataBits dataBits: Int?) = apply { this.dataBits = dataBits }
        fun parity(@Parity parity: Int?) = apply { this.parity = parity }
        fun stopBits(@StopBits stopBits: Int?) = apply { this.stopBits = stopBits }
        fun clientType(@MutiThreadClientType clientType: Int?) =
            apply { this.clientType = clientType }

        fun checkIntervalWaitMills(checkIntervalWaitMills: Int?) =
            apply { this.checkIntervalWaitMills = checkIntervalWaitMills }

        fun build() = LSerialPortClient(
            path,
            baudrate!!,
            dataBits!!,
            parity!!,
            stopBits!!,
            checkIntervalWaitMills!!,
            clientType!!
        )
    }
}

