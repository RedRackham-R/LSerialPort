package com.redrackham.client

import com.redrackham.*

abstract class BaseSerialPortClient constructor(
    open val path: String,
    open val baudrate: Int,
    open val dataBits: Int,
    open val parity: Int,
    open val stopBits: Int,
) {
    companion object {
        internal const val DEF_BAUDRATE = BaudRate.B_9600
        internal const val DEF_DATABITS = DataBits.EIGHT
        internal const val DEF_PARITY = Parity.NONE
        internal const val DEF_STOPBITS = StopBits.ONE
    }

    abstract fun open(): Int

    abstract fun close(): Int

    abstract fun hasOpen(): Boolean

}