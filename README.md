# LSerialPort 一个C++实现多线程管理读写串口Android库

该库采用C++实现异步线程读写串口。上层使用无需过多关心异步线程读写问题。
<br><br>
<img width="480" alt="12e2d4e30b495362d7ffaf3a1b63719" src="https://user-images.githubusercontent.com/22745782/234218273-ebb2434b-8bb8-47cc-aa7d-0e37c80894b8.png">

* [下载AAR](https://github.com/lxy409875745/LSerialPort/releases/download/20230425/LSerialPort-debug.aar)
* [下载demo.apk](https://github.com/lxy409875745/LSerialPort/releases/download/20230425/app-debug.apk)

## 工程开发环境信息
NDK ：23.1.7779620 <br>
C++ ：17 <br>
Android Gradle Plugin ：7.4.1<br>
Gradle ：7.5 <br>
Android Studio ：Android Studio Electric Eel | 2022.1.1 Patch 1<br>

## 接口说明
<table>
  <tr>
    <td>函数名称</td>
		<td>说明</td>
    <td>返回值</td>
		<td>备注</td>
  </tr>
	<tr>
    <td>LSerialPort.openSerialPort</td>
		<td>开启串口</td>
    <td>0：成功<br>1：失败</td>
		<td>对串口读写前必须先使用该函数打开串口，</td>
  </tr>
	<tr>
    <td>LSerialPort.sendMsg</td>
		<td>发送二进制数据</td>
    <td>0：成功<br>1：失败</td>
		<td>异步发送消息给串口。底层使用queue+mutex维护实现一个线程安全消息队列。上层可在任意线程内调用该函数。</td>
  </tr>
	<tr>
    <td>LSerialPort.setOnLSerialPortListener</td>
		<td>设置串口接收监听器</td>
    <td>0：成功<br>1：失败</td>
		<td>当接收到串口数据时会调用该回调<br><br>注意！！！该回调线程为底层读线程，不推荐在该回调线程做任何耗时操作，以免影响线程消息读取。</td>
   </tr>
  <tr>
    <td>LSerialPort.closeSerialPort</td>
		<td>关闭串口</td>
    <td>0：成功<br>1：失败</td>
		<td>停止底层线程并关闭串口，该函数会阻塞调用线程直到资源释放完毕。</td>
  </tr>
  <tr>
    <td>LSerialPort.hasOpen</td>
		<td>检查串口是否打开</td>
    <td>true：已打开false：未打开</td>
		<td></td>
  </tr>
</table>

<br><br>

## native接口说明
### 以下是jni函数，不推荐直接使用。
<table>
  <tr>
    <td>函数名称</td>
		<td>说明</td>
    <td>返回值</td>
  </tr>
	<tr>
    <td>LSerialPort.native_openSerialPort</td>
		<td>开启串口</td>
    <td>0:成功<br>1:失败</td>
  </tr>
	<tr>
    <td>LSerialPort.native_sendMsg</td>
		<td>发送二进制数据</td>
    <td>0:成功<br>1:失败</td>
  </tr>
	<tr>
    <td>LSerialPort.native_setLSerialPortDataListener</td>
		<td>设置串口接收监听器</td>
    <td>无</td>
   </tr>
  <tr>
    <td>LSerialPort.native_closeSerialPort</td>
		<td>关闭串口</td>
    <td>0:成功<br>1:失败</td>
  </tr>
  <tr>
    <td>LSerialPort.native_hasOpen</td>
		<td>检查串口是否打开</td>
    <td>true:成功<br>false:失败</td>
  </tr>
</table>

<br><br>

## 引入AAR
1. 下载 [AAR包](https://github.com/lxy409875745/LSerialPort/releases/download/20230425/LSerialPort-debug.aar)放入工程内libs目录下
<img width="309" alt="d1bd64379531d24b56db04b24a30bc9" src="https://user-images.githubusercontent.com/22745782/234186399-1afc0fec-3040-4123-8f0a-b2c7681880d1.png">

2. 在build.gradle中dependencies内添加引用声明
```kotlin
implementation fileTree(dir: 'libs', include: ['*.jar','*.aar'])
```
<img width="488" alt="12fde479a9d566889521909bd8f4d10" src="https://user-images.githubusercontent.com/22745782/234187372-ba3ac2b7-2963-48e2-a135-148bba59bd13.png">

<br><br>

## 开始使用（Kotlin）[参考代码](https://github.com/lxy409875745/LSerialPort/blob/master/app/src/main/java/com/lxy/lserialport/MainActivity.kt)

### 打开串口

```kotlin
//打开串口/dev/ttysWK0,波特率为9600
val result = LSerialPort.openSerialPort(path = "/dev/ttysWK0",baudrate =  BaudRate.B_9600)

//当然也可以分别定制数据位，校验位，停止位。
//默认数据位：8，校验位：无，停止位：1
val result = LSerialPort.openSerialPort(
    path = "/dev/ttysWK0", //串口地址
    baudrate = BaudRate.B_9600,//波特率
    dataBits = DataBits.EIGHT,//数据位
    parity = Parity.NONE,//校验位
    stopBits = StopBits.ONE//停止位
)


//如果需要一次返回尽量多一些数据。可以设置checkIntervalWaitMills参数。
//该函数是线程循环检查串口是否有数据并通知回传的等待时间，设置等待时间越长，数据返回量越大，当然数据回调的间隔也会越久，酌情配置。
//默认等待时间：0 单位：毫秒
val result = LSerialPort.openSerialPort(
    path = "/dev/ttysWK0", 
    baudrate = BaudRate.B_9600,
    checkIntervalWaitMills = 100//设置等待时间100ms 
)
```

### 发送一条数据
```kotlin
//打开串口后发送数据
val msg = byteArrayOf(0xFF.toByte(),0x01.toByte(),0x02.toByte(),0x03.toByte(),0x04.toByte(),0x05.toByte(),0xFE.toByte())
val result = LSerialPort.sendMsg("/dev/ttysWK0",msg)

//可以在子线程内发送数据，发送线程以及队列由C++部分维护，无需关心线程同步问题
Thread{
    val msg = byteArrayOf(0xFF.toByte(),0x01.toByte(),0x02.toByte(),0x03.toByte(),0xFE.toByte())
    val result = LSerialPort.sendMsg("/dev/ttysWK0",msg)
}.start()
```

### 设置监听器
```kotlin
//打开串口后设置监听器 返回数据为byteArray
//注意！ 如果进行多次设置，每次会覆盖掉前一个监听器。
val result = LSerialPort.setOnLSerialPortListener("/dev/ttysWK0") { msg ->
  Log.d("LSerialPort","接收到数据长度：${msg.size}")
}
```

### 关闭串口
```kotlin
//调用时该函数会阻塞等待C++线程退出，需要一定耗时
val result = LSerialPort.closeSerialPort("/dev/ttysWK0")
```

### 判断串口是否已经打开
```kotlin
//使用该函数判断串口是否已经打开
val isOpened:Boolean = LSerialPort.hasOpen("/dev/ttysWK0")
Log.d("LSerialPort","串口/dev/ttysWK0是否已打开：$isOpened}")
```

<br><br>

## 编译工程生成AAR
1. 导入工程配置后选择Android Studio 中的build -> Refresh Linked C++ Projects 等待Gradle build完成。
<img width="340" alt="63f048d3450c8e4f3a9e3f12ffdf325" src="https://user-images.githubusercontent.com/22745782/234189635-2e2fc7bb-128c-44d4-baf5-2803c795ed1a.png">

2. Gradle build完成后选择Rebuild Project 等待Gradle build完成。
<img width="340" alt="323f9c45804d06a432885f14fbbfb9c" src="https://user-images.githubusercontent.com/22745782/234189711-51d93b77-7efb-41af-9f82-1bb7f28ea9b1.png">

3. 完成后在LSerialPort/build/outputs/aar/目录下会看到LSerialPort-debug.aar文件
<img width="340" alt="f8eb60ecd662347259874a816ec7a11" src="https://user-images.githubusercontent.com/22745782/234190094-7f41540d-72bd-4235-ae26-2d3806336782.png">

<br><br>

## 可能会遇到的问题
### 1. 调用函数是返回-1失败，我该如何查看是什么问题
返回错误码-1时，logcat中会打印错误信息，可以通过Error等级寻找"LSerialPortLog"关键字查看。
```
package:mine level:error LSerialPortLog
```
<img width="500" alt="2a4f6285874dfb001cb817e97924afc" src="https://user-images.githubusercontent.com/22745782/234192781-087d2a92-083e-450b-985d-e53e96b3f3d2.png">

### 2. 新版Studio怎么下载某个版本的NDK or 新版的SDK Manager找不到想要的NDK版本
首先，打开Studio的Tools -> SDK Manage 
<br><br>
<img width="360" alt="a9b279b844028fea09238cca48c5a37" src="https://user-images.githubusercontent.com/22745782/234193677-0312da64-18ee-431a-8940-cc32187fbac3.png">

然后，打开页面后先选择 SDK Tools选项
<br><br>
<img width="500" alt="d1fc5f8b0dfeb715c1390a92b00ba4c" src="https://user-images.githubusercontent.com/22745782/234194594-c93ebd96-d989-4437-ba41-826d47d1e5a0.png">

最后，勾选Show Package Details选项即可下载想要版本的NDK
<br><br>
<img width="500" alt="2f577bfdcdda4fd5b73c2d3eb7f6613" src="https://user-images.githubusercontent.com/22745782/234194982-244de77d-6d7a-4bf9-b425-36a80d69c8a7.png">

### 3. 想要打包运行其他架构的包。如x86平台
打开LSerialPort库的Modele Gradle，在NDK内填写需要的架构，如下图内的x86
<br><br>
<img width="450" alt="07c2b43ad86f7ff01ed7dcfbeecd138" src="https://user-images.githubusercontent.com/22745782/234198091-ffc27f97-6042-4eaa-9743-3023473fc04a.png">

然后删除app以及LSerialPort中的build目录
<br><br>
<img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://user-images.githubusercontent.com/22745782/234198321-eb018f27-61df-44e4-afb0-1fa18517c661.png">

最后重新编译生成AAR即可
<br><br>
<img width="320" alt="cbf1a9173a7a205ca019501facc90ae" src="https://user-images.githubusercontent.com/22745782/234198622-de582a46-5e3d-4b25-b54a-7e83a490806f.png">

### 4. 想要打包release版本的AAR
打开studio的Build Variants。app与LSerialPort都改为release后，创建release需要的签名文件并重新打包即可
<img width="320" alt="da0c9bb910a2cb306095cfdd40447ee" src="https://user-images.githubusercontent.com/22745782/234199873-ca776f27-28d8-4950-a910-8118a6508a4d.png">

### 5. 为什么有时候Refresh Linked C++ Projects以及Rebuild Project后没有AAR文件
先把app以及LSerialPort目录下的build删除，然后重新Refresh Linked C++ Projects以及Rebuild Project，如果还是没有生成多rebuild几次
<img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://user-images.githubusercontent.com/22745782/234213655-721a42db-673f-42e7-8de5-f55fd92780b6.png">

<br><br>

# 特别感谢
### [MserialPort](https://github.com/flykule/MserialPort)

### [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)
