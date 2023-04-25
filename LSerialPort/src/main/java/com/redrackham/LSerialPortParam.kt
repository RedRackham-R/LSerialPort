package com.redrackham

import androidx.annotation.IntDef


/**
 * 波特率
 */
@Retention(AnnotationRetention.SOURCE)
@IntDef(
    BaudRate.B_0,
    BaudRate.B_50,
    BaudRate.B_75,
    BaudRate.B_110,
    BaudRate.B_134,
    BaudRate.B_150,
    BaudRate.B_200,
    BaudRate.B_300,
    BaudRate.B_600,
    BaudRate.B_1200,
    BaudRate.B_1800,
    BaudRate.B_2400,
    BaudRate.B_4800,
    BaudRate.B_9600,
    BaudRate.B_19200,
    BaudRate.B_38400,
    BaudRate.B_57600,
    BaudRate.B_115200,
    BaudRate.B_230400,
    BaudRate.B_460800,
)
annotation class BaudRate {
    companion object {
        const val B_0 = 0
        const val B_50 = 50
        const val B_75 = 75
        const val B_110 = 110
        const val B_134 = 134
        const val B_150 = 150
        const val B_200 = 200
        const val B_300 = 300
        const val B_600 = 600
        const val B_1200 = 1200
        const val B_1800 = 1800
        const val B_2400 = 2400
        const val B_4800 = 4800
        const val B_9600 = 9600
        const val B_19200 = 19200
        const val B_38400 = 38400
        const val B_57600 = 57600
        const val B_115200 = 115200
        const val B_230400 = 230400
        const val B_460800 = 460800
    }
}


/**
 * 数据位
 */
@Retention(AnnotationRetention.SOURCE)
@IntDef(
    DataBits.FIVE,
    DataBits.SIX,
    DataBits.SEVEN,
    DataBits.EIGHT,
)
annotation class DataBits {
    companion object {
        const val FIVE = 5
        const val SIX = 6
        const val SEVEN = 7
        const val EIGHT = 8
    }
};


/**
 * 校验位
 */
@Retention(AnnotationRetention.SOURCE)
@IntDef(
    Parity.NONE,
    Parity.EVEN,
    Parity.ODD,
)
annotation class Parity {
    companion object {
        const val NONE = 0
        const val EVEN = 1
        const val ODD = 2
    }
}


/**
 * 停止位
 */
@Retention(AnnotationRetention.SOURCE)
@IntDef(
    StopBits.ONE,
    StopBits.TWO,
)
annotation class StopBits {
    companion object {
        const val ONE = 1
        const val TWO = 2
    }
}