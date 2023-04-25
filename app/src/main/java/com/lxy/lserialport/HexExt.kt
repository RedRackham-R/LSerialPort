package com.lxy.lserialport


import com.blankj.utilcode.util.TimeUtils
import java.util.*


/**
 * 十六进制转二进制
 */
fun String.hex2bin(): String {
    val sb = StringBuilder()
    val len = this.length
    for (i in 0 until len) {
        //每1个十六进制位转换为4个二进制位
        val temp = this.substring(i, i + 1)
        val tempInt = temp.toInt(16)
        val tempBin = Integer.toBinaryString(tempInt)
        //如果二进制数不足4位，补0
        if (tempBin.length < 4) {
            val num = 4 - tempBin.length
            for (j in 0 until num) {
                sb.append("0")
            }
        }
        sb.append(tempBin)
    }
    return sb.toString()
}


/**
 * 二进制转十六进制
 */
fun String.bin2hex(): String {
    val sb = StringBuilder()
    val len = this.length
    for (i in 0 until len / 4) {
        //每4个二进制位转换为1个十六进制位
        val temp = this.substring(i * 4, (i + 1) * 4)
        val tempInt = temp.toInt(2)
        val tempHex = Integer.toHexString(tempInt).uppercase(Locale.getDefault())
        sb.append(tempHex)
    }
    return sb.toString()
}


fun String.convertToASCII(): ByteArray {
    val ch = this.toCharArray()
    val tmp = ByteArray(ch.size)
    for (i in ch.indices) {
        tmp[i] = Integer.valueOf(ch[i].code).toInt().toByte()
    }
    return tmp
}


/**
 * ascii hex字符串转明文
 */
fun String.convertASCIIHexToBasicStr(): String {
    val sb = StringBuilder()
    var i = 0
    while (i < this.length - 1) {
        val h = this.substring(i, i + 2)
        val decimal = h.toInt(16)
        sb.append(decimal.toChar())
        i += 2
    }
    return sb.toString()
}

fun String.hexToByteArray(): ByteArray {
    return HexUtils.hexStringToBytes(this)
}


fun ByteArray.toHexString(): String {
    return HexUtils.bytesToHexString(this)
}


fun Long.millToPayBoxBCDTime(): ByteArray {
    return TimeUtils.millis2String(this, "yyMMddHHmmss").hexToByteArray()
}


/**
 * 需要使用4字节表示int数字 00000000
 */
fun Int.toHexString32(): String {
    return HexUtils.intToHex32(this)
}

/**
 * 需要使用2字节表示int数字 0000
 */
fun Int.toHexString16(): String {
    return HexUtils.intToHex16(this)
}

/**
 * 需要使用1字节表示int数字 00
 */
fun Int.toHexString8(): String {
    return HexUtils.intToHex8(this)
}


/**
 * byte转为int
 */
fun Byte.hexToInt(): Int {
    return this.toInt() and 0xff
}

fun Byte.toHexString(): String {
    return HexUtils.toHexString(this)
}


/**
 * 数组转为int
 */
fun ByteArray.hexToInt(): Int {
    return HexUtils.byteArrayToInt(this)
}


/**
 * 拼接byte数组
 */
fun ByteArray.spliceByteArray(array: ByteArray): ByteArray {
    val data = ByteArray(this.size + array.size)
    System.arraycopy(this, 0, data, 0, this.size)
    System.arraycopy(array, 0, data, this.size, array.size)
    return data
}
