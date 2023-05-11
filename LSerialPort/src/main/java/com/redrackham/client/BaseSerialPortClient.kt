package com.redrackham.client

import com.redrackham.*

/**
 * 串口client抽象共有方法
 */
abstract class BaseSerialPortClient constructor(
    //串口地址
    open val path: String,
    //波特率
    open val baudrate: Int,
    //数据位
    open val dataBits: Int,
    //校验位
    open val parity: Int,
    //起始位
    open val stopBits: Int,
) {
    companion object {
        //默认参数
        internal const val DEF_BAUDRATE = BaudRate.B_9600
        internal const val DEF_DATABITS = DataBits.EIGHT
        internal const val DEF_PARITY = Parity.NONE
        internal const val DEF_STOPBITS = StopBits.ONE
    }

    /**
     * 打开串口
     */
    abstract fun open(): Int

    /**
     * 关闭串口
     */
    abstract fun close(): Int

    /**
     * 检查串口是否开启
     */
    abstract fun hasOpen(): Boolean

}