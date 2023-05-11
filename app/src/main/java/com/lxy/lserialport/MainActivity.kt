package com.lxy.lserialport

import android.R
import android.os.Bundle
import android.util.Log
import android.widget.ArrayAdapter
import androidx.appcompat.app.AppCompatActivity
import com.blankj.utilcode.util.TimeUtils
import com.lxy.lserialport.databinding.ActivityMainBinding
import com.redrackham.*
import com.redrackham.client.LSerialPortClient


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val mPathArr by lazy { SerialPortFinder().allDevicesPath }
    private val mBaudrateArr = arrayOf(
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
    private val mDataBitsArr = arrayOf(
        DataBits.FIVE,
        DataBits.SIX,
        DataBits.SEVEN,
        DataBits.EIGHT,
    )
    private val mParityArr = arrayOf(
        "NONE",
        "EVEN",
        "ODD",
    )
    private val mStopBits = arrayOf(
        StopBits.ONE,
        StopBits.TWO,
    )

    private val defMsg = "FF0102030405060708090A0B0C0D0E0FFE"

    private var lSerialPortClient: LSerialPortClient? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        initViews()
    }

    private fun initViews() {
        //串口地址Path
        binding.spPath.run {
            adapter = ArrayAdapter(
                this@MainActivity, R.layout.simple_spinner_item, mPathArr
            ).apply {
                setDropDownViewResource(R.layout.simple_spinner_dropdown_item)
            }
        }
        //波特率Baudrate
        binding.spBaudrate.run {
            adapter = ArrayAdapter(
                this@MainActivity, R.layout.simple_spinner_item, mBaudrateArr
            ).apply {
                setDropDownViewResource(R.layout.simple_spinner_dropdown_item)
            }
            setSelection(13)//默认选9600
        }
        //数据位DataBits
        binding.spDatabits.run {
            adapter = ArrayAdapter(
                this@MainActivity, R.layout.simple_spinner_item, mDataBitsArr
            ).apply {
                setDropDownViewResource(R.layout.simple_spinner_dropdown_item)
            }
            setSelection(3)//默认选8
        }

        //校验位Parity
        binding.spParity.run {
            adapter = ArrayAdapter(
                this@MainActivity, R.layout.simple_spinner_item, mParityArr
            ).apply {
                setDropDownViewResource(R.layout.simple_spinner_dropdown_item)
            }
        }

        //停止位
        binding.spStopbits.run {
            adapter = ArrayAdapter(
                this@MainActivity, R.layout.simple_spinner_item, mStopBits
            ).apply {
                setDropDownViewResource(R.layout.simple_spinner_dropdown_item)
            }
        }

        //打开串口
        binding.btnOpen.run {
            setOnClickListener {
                //串口地址
                val path = binding.spPath.selectedItem as String
                //波特率
                val baudrate = binding.spBaudrate.selectedItem as Int
                //数据位
                val dataBits = binding.spDatabits.selectedItem as Int
                //校验位
                val parity = when (binding.spParity.selectedItem) {
                    "NONE" -> Parity.NONE
                    "EVEN" -> Parity.EVEN
                    "ODD" -> Parity.ODD
                    else -> Parity.NONE
                }
                //停止位
                val stopBits = binding.spStopbits.selectedItem as Int

                //这里判断下 避免创建多个client对象 底层其实还是调用一个对象的函数，不推荐多个对象同时操作同一个串口，。
                if (lSerialPortClient?.hasOpen() == true) {
                    showLogOnMain("打开串口失败，请先关闭当前运行串口【${lSerialPortClient!!.path}】!!!")
                    return@setOnClickListener
                }

                lSerialPortClient = LSerialPortClient.Builder(path)
                    .baudrate(baudrate)
                    .dataBits(dataBits)
                    .parity(parity)
                    .stopBits(stopBits)
                    .build()
                //打开串口
                val result = lSerialPortClient?.open()
                if (result == 0) {
                    setUIClickbale(false)
                    showLogOnMain("打开串口${path}成功")
                    showLogOnMain("Baudrate:${baudrate}   DataBits:$dataBits   Parity:${binding.spParity.selectedItem}   StopBits:$stopBits")
                    //打开成功设置监听器
                    lSerialPortClient!!.setListener { msg ->
                        showLogOnMain("收 <<< ${msg.toHexString()}")
                    }
                } else {
                    showLogOnMain("打开串口${path}失败！")
                }
            }
        }
        //关闭串口
        binding.btnClose.run {
            setOnClickListener {
                //串口地址
                val path = binding.spPath.selectedItem as String
                val result = lSerialPortClient?.close()
                if (result == 0) {
                    setUIClickbale(true)
                    showLogOnMain("关闭串口${path} 成功！")
                } else {
                    showLogOnMain("关闭串口${path} 失败！")
                }
            }
        }

        binding.btnCleanLog.run {
            setOnClickListener {
                showLogOnMain(null)
            }
        }


        //发送消息
        binding.btnSendMsg.run {
            setOnClickListener {
                //串口地址
                val path = binding.spPath.selectedItem as String
                val msgStr = binding.edtMsg.text.toString().trim()
                val msgHex = msgStr.hexToByteArray()
                lSerialPortClient?.sendMsg(msgHex)
            }
        }

        binding.edtMsg.setText(defMsg)
        binding.edtMsg.isEnabled = false
        binding.btnSendMsg.isEnabled = false
    }


    private fun setUIClickbale(clickable: Boolean) {
        binding.spPath.isEnabled = clickable
        binding.spBaudrate.isEnabled = clickable
        binding.spDatabits.isEnabled = clickable
        binding.spParity.isEnabled = clickable
        binding.spStopbits.isEnabled = clickable
        //反向设置点击
        binding.btnSendMsg.isEnabled = !clickable
        binding.edtMsg.isEnabled = !clickable

    }

    private fun showLogOnMain(msg: String?) {
        Log.e("lxyTest", "$msg")
        if (msg == null) {
            runOnUiThread { binding.tvLog.text = "" }
        } else {
            runOnUiThread {
                binding.tvLog.text = "【${TimeUtils.getNowString()}】${msg}";
            }
            binding.scrollviewLog.run {
                post {
                    val offset: Int = binding.tvLog.measuredHeight - measuredHeight
                    if (offset > 0) {
                        scrollTo(0, offset)
                    }
                }
            }
        }
    }


}


