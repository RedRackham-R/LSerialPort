# LSerialPort 一个C++实现多线程管理读写串口Android库

* [项目地址] https://github.com/RedRackham-R/LSerialPort)


该库采用C++实现异步线程读写串口。上层使用无需过多关心异步线程读写问题。
<br><br>
<img width="480" alt="12e2d4e30b495362d7ffaf3a1b63719" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8962cb0e03b74b6da38d743405cdd77d~tplv-k3u1fbpfcp-watermark.image?">

<br><br>
arm64-v8a、armeabi-v7a
* [下载AAR](https://github.com/RedRackham-R/LSerialPort/releases/download/20230425/LSerialPort-debug.aar)
* [下载demo.apk](https://github.com/RedRackham-R/LSerialPort/releases/download/20230425/app-debug.apk) 

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

## Native接口说明
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

<br>

## 引入AAR
1. 下载 [AAR包](https://github.com/lxy409875745/LSerialPort/releases/download/20230425/LSerialPort-debug.aar)放入工程内libs目录下
<img width="309" alt="d1bd64379531d24b56db04b24a30bc9" src="https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b8d78c7ef78c4b19ba95e430adb36d59~tplv-k3u1fbpfcp-watermark.image?">


2. 在build.gradle中dependencies内添加引用声明
```kotlin
implementation fileTree(dir: 'libs', include: ['*.jar','*.aar'])
```
<img width="488" alt="12fde479a9d566889521909bd8f4d10" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/d989a028987b4ae08a3af511447d7292~tplv-k3u1fbpfcp-watermark.image?">

<br>

## 开始使用（Kotlin）[参考代码](https://github.com/RedRackham-R/LSerialPort/blob/master/app/src/main/java/com/lxy/lserialport/MainActivity.kt)

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

<br>

## 工程开发环境信息
NDK ：23.1.7779620 <br>
C++ ：17 <br>
Android Gradle Plugin ：7.4.1<br>
Gradle ：7.5 <br>
Android Studio ：Android Studio Electric Eel | 2022.1.1 Patch 1<br>

## 编译工程生成AAR
1. 导入工程配置后选择Android Studio 中的build -> Refresh Linked C++ Projects 等待Gradle build完成。
<img width="340" alt="63f048d3450c8e4f3a9e3f12ffdf325" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/9f77cbdfb78648deb638b2387dc2e484~tplv-k3u1fbpfcp-watermark.image?">


2. Gradle build完成后选择Rebuild Project 等待Gradle build完成。
<img width="340" alt="323f9c45804d06a432885f14fbbfb9c" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e523ff583af84dc698c39cb16e255fe9~tplv-k3u1fbpfcp-watermark.image?">

3. 完成后在LSerialPort/build/outputs/aar/目录下会看到LSerialPort-debug.aar文件
<img width="340" alt="f8eb60ecd662347259874a816ec7a11" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ce8a8dda2428475e9ef071b28c1dac03~tplv-k3u1fbpfcp-watermark.image?">

<br><br>

## 可能会遇到的问题
### 1. 调用函数是返回-1失败，我该如何查看是什么问题
返回错误码-1时，logcat中会打印错误信息，可以通过Error等级寻找"LSerialPortLog"关键字查看。
```
package:mine level:error LSerialPortLog
```
<img width="500" alt="2a4f6285874dfb001cb817e97924afc" src="https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/92ba70b5b2864402a6ef2d1dac39225b~tplv-k3u1fbpfcp-watermark.image?">

### 2. 新版Studio怎么下载某个版本的NDK or 新版的SDK Manager找不到想要的NDK版本
首先，打开Studio的Tools -> SDK Manage 
<br><br>
<img width="360" alt="a9b279b844028fea09238cca48c5a37" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/2cefd64ca7d2474e935327fa9cddca96~tplv-k3u1fbpfcp-watermark.image?">

然后，打开页面后先选择 SDK Tools选项
<br><br>
<img width="500" alt="d1fc5f8b0dfeb715c1390a92b00ba4c" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/dc096401dff0410ba4ce8f248ccf7c4a~tplv-k3u1fbpfcp-watermark.image?">

最后，勾选Show Package Details选项即可下载想要版本的NDK
<br><br>
<img width="500" alt="2f577bfdcdda4fd5b73c2d3eb7f6613" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/cd1c110081504238bd0d87b4c96e5c37~tplv-k3u1fbpfcp-watermark.image?">

### 3. 想要打包运行其他架构的包。如x86平台
打开LSerialPort库的Modele Gradle，在NDK内填写需要的架构，如下图内的x86
<br><br>
<img width="450" alt="07c2b43ad86f7ff01ed7dcfbeecd138" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/c1f0aa9b9caa4ecca73b951980e4d71a~tplv-k3u1fbpfcp-watermark.image?">

然后删除app以及LSerialPort中的build目录
<br><br>
<img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5f4b194411464a44998b1c6c28c22d14~tplv-k3u1fbpfcp-watermark.image?">

最后重新编译生成AAR即可
<br><br>
<img width="320" alt="cbf1a9173a7a205ca019501facc90ae" src="https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/efd6f2131f8f41568afa8484f876eea0~tplv-k3u1fbpfcp-watermark.image?">

### 4. 想要打包release版本的AAR
打开studio的Build Variants。app与LSerialPort都改为release后，创建release需要的签名文件并重新打包
<br>
<img width="320" alt="da0c9bb910a2cb306095cfdd40447ee" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ebde69799b784f439b0a484439d32f75~tplv-k3u1fbpfcp-watermark.image?">

### 5. 为什么有时候Refresh Linked C++ Projects以及Rebuild Project后没有AAR文件
先把app以及LSerialPort目录下的build删除，然后重新Refresh Linked C++ Projects以及Rebuild Project，如果还是没有生成多rebuild几次
<br>
<img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/3ce577810dc04c22950541a0171a7d66~tplv-k3u1fbpfcp-watermark.image?">
<br><br>
# 结语 
最近学了点C++，总感觉需要上手撸点东西。正好公司项目有使用串口的场景，但是发现大多数串口库一般都需要使用者自己在上层实现对串口多线程管理读写，有些不方便。在网上发现个优秀的C++底层实现多线程串口读写管理的库[MserialPort] https://github.com/flykule/MserialPort) 
，遂按照该库的代码思路写了LSerialPort。一方面巩固刚学习学习的C++知识，另一方面是重拾很久没用过的JNI、NDK。

# 特别感谢
### [MserialPort](https://github.com/flykule/MserialPort)

### [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)
