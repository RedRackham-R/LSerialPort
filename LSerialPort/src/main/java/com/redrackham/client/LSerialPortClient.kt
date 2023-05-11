package com.redrackham.client

import com.redrackham.*
import com.redrackham.LSerialPortJNI.native_closeSerialPort
import com.redrackham.LSerialPortJNI.native_hasOpen
import com.redrackham.LSerialPortJNI.native_openSerialPort
import com.redrackham.LSerialPortJNI.native_openSerialPortReadOnly
import com.redrackham.LSerialPortJNI.native_openSerialPortWriteOnly
import com.redrackham.LSerialPortJNI.native_sendMsg
import com.redrackham.LSerialPortJNI.native_setLSerialPortDataListener

/**
 * LSerialPortClient 串口客户端
 * 可用读写、只读、只写方式异步线程操作串口
 */
class LSerialPortClient private constructor(
    override val path: String,
    override val baudrate: Int,
    override val dataBits: Int,
    override val parity: Int,
    override val stopBits: Int,
    val checkIntervalWaitMills: Int,//循环检查等待时间
    val clientType: Int,//客户端类型
) : BaseSerialPortClient(path, baudrate, dataBits, parity, stopBits) {

    companion object {
        //默认类型：读写
        private const val DEF_CLIENT_TYPE = MutiThreadClientType.READ_WRITE

        //默认检查数据等待时间 0ms
        private const val DEF_CHECK_INTERVAL_WAIT_MILLS = 0
    }

    /**
     *  打开串口
     *  根据建造时的clientType以读写、只读或只写方式打开串口
     */
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


    /**
     * 关闭串口
     */
    override fun close(): Int = native_closeSerialPort(path)

    /**
     * 串口是否已经打开
     */
    override fun hasOpen(): Boolean = native_hasOpen(path)

    /**
     * 设置回调监听器
     */
    fun setListener(listener: LSerialPortDataListener) =
        native_setLSerialPortDataListener(path, listener)

    /**
     * 发送数据
     */
    fun sendMsg(msg: ByteArray?) = native_sendMsg(path, msg)


    class Builder constructor(val path: String) {
        private var baudrate: Int = DEF_BAUDRATE
        private var dataBits: Int = DEF_DATABITS
        private var parity: Int = DEF_PARITY
        private var stopBits: Int = DEF_STOPBITS
        private var checkIntervalWaitMills: Int = DEF_CHECK_INTERVAL_WAIT_MILLS
        private var clientType: Int = DEF_CLIENT_TYPE

        fun baudrate(@BaudRate baudrate: Int) = apply { this.baudrate = baudrate }
        fun dataBits(@DataBits dataBits: Int) = apply { this.dataBits = dataBits }
        fun parity(@Parity parity: Int) = apply { this.parity = parity }
        fun stopBits(@StopBits stopBits: Int) = apply { this.stopBits = stopBits }
        fun clientType(@MutiThreadClientType clientType: Int) =
            apply { this.clientType = clientType }

        fun checkIntervalWaitMills(checkIntervalWaitMills: Int) =
            apply { this.checkIntervalWaitMills = checkIntervalWaitMills }

        fun build() = LSerialPortClient(
            path,
            baudrate,
            dataBits,
            parity,
            stopBits,
            checkIntervalWaitMills,
            clientType
        )
    }
}

