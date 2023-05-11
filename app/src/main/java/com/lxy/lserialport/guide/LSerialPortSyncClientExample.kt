package com.lxy.lserialport.guide

import android.util.Log
import com.redrackham.BaudRate
import com.redrackham.DataBits
import com.redrackham.Parity
import com.redrackham.StopBits
import com.redrackham.client.LSerialPortSyncClient

/**
 * LSerialPortSyncClient 使用样例
 */
class LSerialPortSyncClientExample {

    /**
     * 样例代码
     */
    fun example() {
        //创建clientBuilder，该client操作串口：ttysWK0。通过建造者我们可以选择定制参数
        val clientBuilder = LSerialPortSyncClient.Builder("/dev/ttysWK0")
            .baudrate(BaudRate.B_115200)//波特率
            .dataBits(DataBits.EIGHT)//数据位
            .parity(Parity.NONE)//校验位
            .stopBits(StopBits.ONE)//停止位
            //读取数据超时时间，当传入
            //   -1:一直阻塞线程等待返回
            //    0:无论有没有数据，立即返回。如果没有数据，则返回一个空的byte数组
            // 大于0:阻塞当前线程等待返回时间。如果没有数据，则返回一个空的byte数组  单位（ms）
            .readTimeoutMills(-1)//这里设置-1 一直阻塞线程等待返回

        //创建串口操作client
        val client = clientBuilder.build()

        //打开串口，获取返回结果，当返回result == 0 时，说明打开成功
        val result = client.open()

        //必须确保打开成功后再进行读写操作！
        if (result == 0) {
            //这里为了读取函数不阻塞主线程，我们新创建一个线程

            Thread {
                val data = byteArrayOf(
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
                //发送一条数据
                client.write(data)

                //检查串口是否有数据回传
                val dataAvailable = client.dataAvailable()
                Log.d("LSerialPortSyncClientExample", "串口是否有数据可接收：${dataAvailable}")

                //读数据
                val receivedData = client.read()
                Log.d("LSerialPortSyncClientExample", "接收到数据，数据大小：${receivedData.size}")

                //关闭串口
                client.close()
            }.start()
        }
    }
}