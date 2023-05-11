package com.lxy.lserialport.guide

import android.util.Log
import com.redrackham.BaudRate
import com.redrackham.DataBits
import com.redrackham.Parity
import com.redrackham.StopBits
import com.redrackham.client.LSerialPortClient

/**
 * LSerialPortClient 使用样例
 */
class LSerialPortClientExample {

    /**
     * 样例代码
     */
    fun example() {
        //创建clientBuilder，该client操作串口：ttysWK0。通过建造者我们可以选择定制参数
        val clientBuilder = LSerialPortClient.Builder("/dev/ttysWK0")
            .baudrate(BaudRate.B_115200)//波特率
            .dataBits(DataBits.EIGHT)//数据位
            .parity(Parity.NONE)//校验位
            .stopBits(StopBits.ONE)//停止位
            .checkIntervalWaitMills(0)//轮循检查串口等待时间，等待时间越长一次返回的数据就越大，单位:ms

        //创建串口操作client
        val client = clientBuilder.build()

        //打开串口，获取返回结果，当返回result == 0 时，说明打开成功
        val result = client.open()

        //必须确保打开成功后再进行读写操作！
        if (result == 0) {
            //设置回调监听器
            client.setListener { data ->
                Log.d("LSerialPortClientExample", "异步线程接收到数据，数据大小：${data.size}")
            }

            //发送一条数据
            val msg = byteArrayOf(
                0xFF.toByte(),
                0x00.toByte(),
                0x01.toByte(),
                0x02.toByte(),
                0x03.toByte(),
                0x04.toByte(),
                0x05.toByte(),
                0x06.toByte(),
                0xFE.toByte()
            )
            client.sendMsg(msg)

            //在子线程发一条数据
            Thread {
                client.sendMsg(msg)
            }.start()

            //等待两秒钟关闭串口
            Thread {
                Thread.sleep(2000)
                client.close()
                Log.d("LSerialPortClientExample", "串口【${client.path}】已关闭")
            }.start()
        }
    }
}