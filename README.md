# LSerialPort 一个C++实现多线程管理读写串口Android库

* [项目地址] https://github.com/RedRackham-R/LSerialPort)


该库采用C++实现异步线程读写串口。上层使用无需过多关心异步线程读写问题。
<br><br>
<img width="480" alt="12e2d4e30b495362d7ffaf3a1b63719" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8962cb0e03b74b6da38d743405cdd77d~tplv-k3u1fbpfcp-watermark.image?">

<br><br>
arm64-v8a、armeabi-v7a
* [下载AAR](https://github.com/RedRackham-R/LSerialPort/releases/download/20230504/LSerialPort-20230504-debug.aar)
* [下载demo.apk](https://github.com/RedRackham-R/LSerialPort/releases/download/20230504/app-debug.apk)

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
    <td>对串口读写前必须先使用该函数打开串口。</td>   
  </tr>
  <tr>
    <td>LSerialPort.openSerialPortReadOnly</td>
    <td>以只读方式开启串口</td>
    <td>0：成功<br>1：失败</td>
    <td>使用只读方式打开串口。打开后无法主动发送数据，只能被动接收。</td>   
  </tr>
  <tr>
    <td>LSerialPort.openSerialPortWriteOnly</td>
    <td>以只写方式开启串口</td>
    <td>0：成功<br>1：失败</td>
    <td>使用写读方式打开串口。打开后无法接收数据，只能主动发送。</td>   
  </tr>
  <tr>
    <td>LSerialPort.sendMsg</td>
    <td>发送二进制数据</td>
    <td>0：成功<br>1：失败</td>
    <td>在读写或只写方式打开串口后，异步发送消息给串口。底层使用queue+mutex维护实现一个线程安全消息队列。上层可在任意线程内调用该函数。</td>
  </tr>
  <tr>
    <td>LSerialPort.setOnLSerialPortListener</td>
    <td>设置串口接收监听器</td>
    <td>0：成功<br>1：失败</td>
    <td>在读写或只读方式打开串口后，当接收到串口数据时会调用该回调<br><br>注意！！！该回调线程为底层读线程，不推荐在该回调线程做任何耗时操作，以免影响线程消息读取。</td>
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
    <td>LSerialPort.native_openSerialPortReadOnly</td>
    <td>只读方式开启串口</td>
    <td>0:成功<br>1:失败</td>
  </tr>
  <tr>
    <td>LSerialPort.native_openSerialPortWriteOnly</td>
    <td>只写方式开启串口</td>
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
1. 下载 [AAR包](https://github.com/RedRackham-R/LSerialPort/releases/download/20230504/LSerialPort-20230504-debug.aar)放入工程内libs目录下
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
    checkIntervalWaitMills = 10//设置等待时间10ms 
)

//只读方式打开串口
val result = LSerialPort.openSerialPortReadOnly(
    path = "/dev/ttysWK0", //串口地址
    baudrate = BaudRate.B_9600,//波特率
    dataBits = DataBits.EIGHT,//数据位
    parity = Parity.NONE,//校验位
    stopBits = StopBits.ONE//停止位
)

//只写方式打开串口
val result = LSerialPort.openSerialPortWriteOnly(
    path = "/dev/ttysWK0", //串口地址
    baudrate = BaudRate.B_9600,//波特率
    dataBits = DataBits.EIGHT,//数据位
    parity = Parity.NONE,//校验位
    stopBits = StopBits.ONE,//停止位
    checkIntervalWaitMills =0//设置等待时间0ms
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


2. Gradle build完成后选择Rebuild Project 等待Gradle build完成。<br>
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
<br>
# LSerialPort设计思路
首先，我们这里对串口设计三个线程任务，分别处理对串口进行读，写，以及检查数据并通知唤醒读线程进行数据读取。这里多一个检查通知线程的意义在于我们能自行定制读取数据时机，<br>
通过修改每次检查后的等待时间，可以减少读取次数，同时增加一次读取的数据量。<br><br>
其次，由于涉及到多串口，我们还要实现串口间的通信，以及实现安全关闭线程以及回收资源。避免内存泄漏问题。<br><br>
最后，还需要一个管理者，对每个打开的串口进行增删查改功能。<br><br>
下面是每个功能具体实现的介绍。

## 1.写线程<br>
实现异步发送，这里采用消息队列的机制。上层开放接口往队列发送消息，线程轮循读取消息队列获取发送的消息往串口发送。这里我们使用C++中的[std::queue](https://zh.cppreference.com/w/cpp/container/queue)来作为消息队列，但是[std::queue](https://zh.cppreference.com/w/cpp/container/queue)是线程不安全的队列，所以需要
一个锁来确保读写队列时的线程安全，这里使用[std::mutex](https://zh.cppreference.com/w/cpp/thread/mutex)以及[std::lock_guard](https://zh.cppreference.com/w/cpp/thread/lock_guard)来创建互斥锁来实现。现在只差一个问题，该如何做到发送消息给队列后通知写线程读取队列中的消息，实现线程之间的通信？。这里用到另一个C++标准库内的[std::condition_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)来解决，[std::condition_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)可以阻塞一个或多个线程，直到其他线程修改共享变量或条件，并通知有意修改变量的线程。这样就完美符合我们的要求。
<br><br>
下面分别是提供上层发送消息的接口函数doWork以及写线程函数readLoop。<br>

doWork-上层发送消息函数
```c++
void ReadWriteWorker::doWork(const std::vector<uint8_t> &msg) {
    //创建互斥锁保证线程安全
    std::lock_guard<std::mutex> lk(_mMsgMutex);
    //推送消息到消息队列_mMsgQueue
    _mMsgQueue.push(msg);
    //通知消息消息等待的线程（包含读写线程），检查执行条件
    _mMsgCond.notify_all();
}
```


writeLoop-写线程循环函数
```c++
//写循环
void ReadWriteWorker::writeLoop() {
    // 等待直至 main() 发送数据
    std::unique_lock<std::mutex> lk(_mMsgMutex);
    LOGE("start write loop");
    while (!isInterrupted()) {
        //这里判断当前线程没有标记退出，同时消息队列中有消息，继续运行后面的代码。
        _mMsgCond.wait(lk, [&] { return (isInterrupted() || !_mMsgQueue.empty()); });
        if (isInterrupted()) {
            break;
        }
        //从消息队列中获取消息
        std::vector<uint8_t> msg = std::move(_mMsgQueue.front());
        _serialPort->WriteBinary(msg);
        //消息出列
        _mMsgQueue.pop();
    }
    LOGE("write loop is interrupted!");
}
```


## 2.读线程<br>
有了前面写线程的经验，读线程也可以利用前面提到的[std::mutex](https://zh.cppreference.com/w/cpp/thread/mutex)、[std::lock_guard](https://zh.cppreference.com/w/cpp/thread/lock_guard)以及[std::condition_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)来实现线程之间的通信以及保证共有数据的线程安全。线程问题现在已经解决了，那我们该怎么把读到的数据返回给上层呢？答案是JNI中[JNIEnv](https://blog.csdn.net/createchance/article/details/53783490)的GetObjectClass以及GetMethodID获取java回调函数把数据回传给上层。同时，我们也增加一个监听器锁_mListenerMutex，实现安全设置监听器。

readLoop-写线程循环函数

```c++
void ReadWriteWorker::readLoop() {
    LOGE("start read loop");
    // 等待直至收到发送数据
    std::unique_lock<std::mutex> lk(_mMsgMutex);
    //C++线程首次访问JEnv函数需要先使用AttachCurrentThread把当前线程附加到JVM上下文才能使用JEnv的函数、
    //Jni的native函数不需要是因为是Jvm线程调用函数，所以不需要附加，C++创建线程不数据Jvm，所以要进行关联附加才能使用Jni的函数
    if (LSerialPortManager::jvm->AttachCurrentThread(&_env, nullptr) != JNI_OK) {
        return THROW_EXCEPT("unable to get jvm instance");
    }
    while (!isInterrupted()) {
        //这里判断当前线程没有标记退出，同时有数据的情况下停止阻塞，继续运行后面的代码。
        _mMsgCond.wait(lk, [&] { return (isInterrupted() || _dataAvailable.load()); });
        if (isInterrupted()) {
            break;
        }
        //读取二进制
        std::vector<uint8_t> msg_vec;
        //读取数据
        _serialPort->ReadBinary(msg_vec);
        //重置标记
        _dataAvailable.store(false);
        if (msg_vec.empty()) {
            //没数据视为超时，进行下一轮循环
            continue;
        }
        //设置监听器锁
        std::lock_guard<std::mutex> llk(_mListenerMutex);
        if (_prepListener != nullptr) {
            //如果之前已经设置过了，先回收删除上一个listener
            if (_curlistener != nullptr) {
                LOGE("there is currently a listener and it is being recycled...");
                _curlistener->recycle(_env);
                delete _curlistener;
            }
            _curlistener = _prepListener;
            _prepListener = nullptr;
        }

        if (_curlistener != nullptr) {
            _curlistener->onDataReceived(_env, msg_vec);
        }

    }
    if (_prepListener != nullptr) {
        _prepListener->recycle(_env);
    }

    if (_curlistener != nullptr) {
        _curlistener->recycle(_env);
    }
    //退出当前线程
    LSerialPortManager::jvm->DetachCurrentThread();
    LOGE("read loop is interrupted!");
}
```

onDataReceived-回调读取到的数据给java
```c++
void LSerialPortDataListener::onDataReceived(JNIEnv *env, const std::vector<uint8_t> &msg) {
    if (_onDataReceivedMethod == nullptr) {
        jclass listenerClass = env->GetObjectClass(_jListener);
        _onDataReceivedMethod = env->GetMethodID(listenerClass, "onDataReceived", "([B)V");
        env->DeleteLocalRef(listenerClass);
    }

    auto size = static_cast<uint32_t>(msg.size());
    jbyteArray jData;
    if (size > static_cast<uint32_t>(std::numeric_limits<jsize>::max())) {
        LOGE("data size is too big start splite send data!");
        // 数据过大，需要分段拷贝
        constexpr uint32_t chunkSize = std::numeric_limits<jsize>::max();
        jData = env->NewByteArray(chunkSize);
        uint32_t offset = 0;
        while (offset < size) {
            const uint32_t copySize = std::min(size - offset, chunkSize);
            env->SetByteArrayRegion(jData, 0, copySize,
                                    reinterpret_cast<const jbyte *>(msg.data() + offset));
            offset += copySize;
            env->CallVoidMethod(_jListener, _onDataReceivedMethod, jData);
            env->DeleteLocalRef(jData);
        }

    } else {
        // 直接拷贝到jbyteArray中
        if (env == nullptr) {
            LOGE("JNIEnv pointer is null!");
            return;
        }
        jData = env->NewByteArray(static_cast<jsize>(size));
        env->SetByteArrayRegion(jData, 0, static_cast<jsize>(size),
                                reinterpret_cast<const jbyte *>(msg.data()));
        env->CallVoidMethod(_jListener, _onDataReceivedMethod, jData);
        env->DeleteLocalRef(jData);
    }

}
```
## 3.检查通知线程<br>
在设计思路中我们提到过，增加一个检查通知线程的目的是为了能自行定制读取数据的实际。满足一些需要减少读取次数同时时效性要求不怎么高，一次返回尽可能多数据的场景。这里我们通过获取到[std::condition_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)对读线程进行通信。<br><br>
checkAvailableLoop-检查通知数据接收循环函数
```c++
void ReadWriteWorker::checkAvailableLoop() {
    LOGE("start check available Loop");
    while (!isInterrupted()) {
        //判断串口对象状态是已打开，同时串口有数据
        if (_serialPort->GetState() == State::OPEN && _serialPort->Available() > 0) {
            //修改读取数据标记
            _dataAvailable.store(true);
            //通知消息
            _mMsgCond.notify_all();
        }
        //这里设置毫秒等待时间 等待x毫秒后再进行下一次检查
        std::this_thread::sleep_for(std::chrono::milliseconds(_checkIntervalWaitMills));
    }
    LOGE("check available Loop is interrupted!");
}
```
<br>

* 到此我们多线程对串口读写的功能已经完成，这里封装成一个名叫[ReadWriteWorker](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/ReadWriteWorker.h) 的类来对功能进行管理。


## 4.安全关闭/退出 <br>
既然打开了这么多线程处理任务，当然如何安全的关闭线程以及串口也是我们需要考虑的功能。这里我们首先是需要考虑该如何关闭时通知到各个线程退出循环结束任务。在前面讲解各个线程设计时讲到，线程使用[std::condition_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)进行唤醒后会根据共享内容条件判断是否继续阻塞，在这里我们也可以利用这个特性增加判断机制。比如用[std::atomic](https://zh.cppreference.com/w/cpp/atomic/atomic)包装一个变量做原子操作进行条件判断。当然，LSerialPort里面我们使用另外一种方式，那就是[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)以及[std::future](https://zh.cppreference.com/w/cpp/thread/future)来实现。[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)提供一种存储值或异常的机制，而[std::future](https://zh.cppreference.com/w/cpp/thread/future)是能访问异步操作结果。[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)可以创建[std::future](https://zh.cppreference.com/w/cpp/thread/future)作关联使用。简单来说就是[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)允许设置一个值，或者异常，这个值或异常可以在未来的某个时刻被关联的[std::future](https://zh.cppreference.com/w/cpp/thread/future)对象访问到。多线程中也可以通过[std::future](https://zh.cppreference.com/w/cpp/thread/future)来实现异步参数获取。
<br><br>
这里我们把安全退出的判断抽出来作为一个抽象类IWorker，方便以后拓展。<br><br>
[IWorker.h](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/IWorker.h)
```c++
#ifndef LSERIALPORT_IWORKER_H
#define LSERIALPORT_IWORKER_H
#include "future"
/**
 * 工作类接口
 * 通过promise和future实现线程安全退出
 */
namespace LSerialPort {
    class IWorker {
    public:
        /**
         * 构造函数同时创建future
         */
        IWorker() : _interruptedFuture(_interruptSignal.get_future()) {
        }
        ~IWorker() {

        }
        //发送消息
        virtual void doWork(const std::vector<uint8_t> &msg) = 0;
        //退出
        virtual void interrupte() {
            _interruptSignal.set_value();
        }
        //是否退出
        bool isInterrupted() {
            //设置等待时间，如果当前状态非超时，则说明已经调用set_value发送数据，标记为退出状态
            return _interruptedFuture.wait_for(std::chrono::milliseconds(0)) !=
                   std::future_status::timeout;
        }
    private:
        std::promise<void> _interruptSignal;
        std::future<void> _interruptedFuture;
    };
}

#endif //LSERIALPORT_IWORKER_H
```

上面提到的ReadWriteWorker也是继承该类。在对象销毁时，先通知各个串口退出循环，然后使用[std::thread::join](https://zh.cppreference.com/w/cpp/thread/thread/join)阻塞并切换线程，等待每个线程结束后，销毁资源完成安全退出。<br>
下面是关闭销毁的功能函数ReadWriteWorker::interrupte()以及ReadWriteWorker::~ReadWriteWorker()<br><br>
[ReadWriteWorker::interrupte()](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/ReadWriteWorker.h)
```c++
/**
 * 退出任务
 */
void interrupte() override {
     //监听器锁
      std::lock_guard<std::mutex> llk(_mListenerMutex);
      //消息锁
      std::lock_guard<std::mutex> lk(_mMsgMutex);
      //发送退出信号
      IWorker::interrupte();
      _mMsgCond.notify_all();
}
```
[ReadWriteWorker::~ReadWriteWorker()](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/ReadWriteWorker.cpp)
```c++
ReadWriteWorker::~ReadWriteWorker() {
    LOGE("---finishing worker---");
    ReadWriteWorker::interrupte();
    LOGE("wait for checkAvaliable thread end");
    if ((_checkAvaliableThread != nullptr) && _checkAvaliableThread->joinable()) {
        _checkAvaliableThread->join();//等待检查线程结束
    }
    LOGE("wait for write thread end");
    if ((_writeThread != nullptr) && _writeThread->joinable()) {
        _writeThread->join();//等待写线程结束
    }
    LOGE("wait for read thread end");
    if ((_readThread != nullptr) && _readThread->joinable()) {
        _readThread->join();//等待读线程结束
    }
    LOGE("cleaning msgQueue");
    //清空队列
    while (!_mMsgQueue.empty()) {
       _mMsgQueue.pop();
    }
    LOGE("cleaning thread ptr");
    delete _checkAvaliableThread;
    delete _writeThread;
    delete _readThread;
    _checkAvaliableThread = nullptr;
    _writeThread = nullptr;
    _readThread = nullptr;
    LOGE("cleaning listener ptr");
    if (_prepListener != nullptr) {
        delete _prepListener;
        _prepListener = nullptr;
    }
    if (_curlistener != nullptr) {
         delete _curlistener;
        _curlistener = nullptr;
    }
    LOGE("close SerialPort");
    if (_serialPort != nullptr) {
        LOGE("closing...");
        _serialPort->Close();
         delete _serialPort;
         _serialPort = nullptr;
        LOGE("close done!");
    }
    LOGE("finish done!");
  }
}
```
## 5.多串口管理
前面已经封装好对串口操作读写的对象ReadWriteWorker，现在只需要使用一个管理类，对每个串口操作对象进行管理。存储这边我们使用[std::unordered_map](https://zh.cppreference.com/w/cpp/container/unordered_map)以串口地址和串口操作对象作为键值对的形式进行保存，添加，删除等操作，使用[std::unordered_map](https://zh.cppreference.com/w/cpp/container/unordered_map)的好处在于它内部实现结构采用对键值进行哈希保存数据，不做任何排序，能对map中的单一值进行快速访问，符合我们现在的实现功能要求。<br><br>
下面是多串口管理类LSerialPortManager声明定义<br><br>
[LSerialPortManager.h](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/LSerialPortManager.h)
```c++
#ifndef LSERIALPORT_LSERIALPORTMANAGER_H
#define LSERIALPORT_LSERIALPORTMANAGER_H
#include <unordered_map>
#include <jni.h>
#include "ReadWriteWorker.h"

using namespace mn::CppLinuxSerial;

namespace LSerialPort {
    class LSerialPortManager {
    public:
        static JavaVM *jvm;

        LSerialPortManager();

        ~LSerialPortManager();

        /**
         * 添加设备
         * @param path 串口地址
         * @param baudRate 波特率
         * @param dataBits 数据位
         * @param parity 校验位
         * @param stopBits 停止位
         * @param readIntervalTimeoutMills 循环读SerialPort超时时间，超过时间就继续下一次轮循
         * @param checkIntervalWaitMills 循环检查等待时间，如果想要回复多一些数据可以适当延长
         * @return
         */
        int addDevice(
                std::string &path,
                BaudRate &baudRate,
                NumDataBits &dataBits,
                Parity &parity,
                NumStopBits &stopBits,
                int32_t &readIntervalTimeoutMills,
                long &checkIntervalWaitMills);

        /**
         * 删除设备
         * @param path 串口地址
         * @return
         */
        int removeDevice(const std::string &path);

        /**
         * 检查串口是否被加载过
         * @param path 串口地址
         * @return
         */
        bool hasDevice(const std::string &path);

        /**
         * 发送二进制消息
         * @param path 串口地址
         * @param msg 二进制消息
         * @return
         */
        int sendMessage(const std::string &path, const std::vector<uint8_t> &msg);

        /**
         * 设置串口数据监听器
         * @param path 串口地址
         * @param listener 监听器
         * @return
         */
        int setLSerialPortDataListener(const std::string &path, jobject *listener);


    private:
        //已加载的串口设备
        std::unordered_map<std::string, std::unique_ptr<ReadWriteWorker>> _mDevices;
    };
}


#endif //LSERIALPORT_LSERIALPORTMANAGER_H
```

由于篇幅原因，这里不列出实现代码。如果要看具体实现可以查看[ReadWriteWorker.cpp](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/ReadWriteWorker.h) 。到此LSerialPort关键功能实现思路介绍完毕。



# 结语
最近学了点C++，总感觉需要上手撸点东西。正好公司项目有使用串口的场景，但是发现大多数串口库一般都需要使用者自己在上层实现对串口多线程管理读写，有些不方便。在网上发现个优秀的C++底层实现多线程串口读写管理的库[MserialPort] https://github.com/flykule/MserialPort)
，遂按照该库的代码思路写了LSerialPort。一方面巩固刚学习学习的C++知识，另一方面是重拾很久没用过的JNI、NDK。

# 特别感谢
### [MserialPort](https://github.com/flykule/MserialPort)
### [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)

# C++学习推荐
### [CppReference](https://zh.cppreference.com/w/%E9%A6%96%E9%A1%B5)
### [RUNNOOB](https://www.runoob.com/cplusplus/cpp-tutorial.html)
