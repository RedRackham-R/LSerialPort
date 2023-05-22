# LSerialPort 一个C++实现多线程管理读写串口Android库

*   \[项目地址] <https://github.com/RedRackham-R/LSerialPort>)

该库采用C++实现异步线程读写串口。上层使用无需过多关心异步线程读写问题。<br><br>
同时也支持非异步读写串口。<br>

<img width="480" alt="12e2d4e30b495362d7ffaf3a1b63719" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8962cb0e03b74b6da38d743405cdd77d~tplv-k3u1fbpfcp-watermark.image?">
<br>

[下载demo](https://github.com/RedRackham-R/LSerialPort/releases)
（平台：arm64-v8a、armeabi-v7a）

# 框架引入

目前工程已经上传至Maven Central，可以直接通过依赖方式引入。

## maven central引入<br>

**Module:build.gradle**

```kotlin
dependencies {
    ...
    implementation("io.github.RedRackham-R:LSerialPort:2.0.2")
}
```

## aar包引入<br>

1.  下载 [AAR包](https://github.com/RedRackham-R/LSerialPort/releases)放入工程内libs目录下<br> <img width="309" alt="d1bd64379531d24b56db04b24a30bc9" src="https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b8d78c7ef78c4b19ba95e430adb36d59~tplv-k3u1fbpfcp-watermark.image?">

2.  在build.gradle中dependencies内添加引用声明

```kotlin
implementation fileTree(dir: 'libs', include: ['*.jar','*.aar'])
```

<img width="488" alt="12fde479a9d566889521909bd8f4d10" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/d989a028987b4ae08a3af511447d7292~tplv-k3u1fbpfcp-watermark.image?">

<br><br>

# 开始使用



> 注意！ 该文档更新于2023年05月11日，当前文档使用版本为【**v2**】。如果查看旧版v1文档请看这里：[V1 文档](https://github.com/RedRackham-R/LSerialPort/blob/v1/README.md)

## 异步操作串口

LSerialPort提供了异步操作串口的类[LSerialPortClient](https://github.com/RedRackham-R/LSerialPort/blob/v2/LSerialPort/src/main/java/com/redrackham/client/LSerialPortClient.kt)。通过该类对串口进行异步开关以及读写操作。下面是一些简单的调用例子。<br><br>
完整使用可以查看 [**>参考代码<**](https://github.com/RedRackham-R/LSerialPort/blob/v2/app/src/main/java/com/lxy/lserialport/guide/LSerialPortClientExample.kt) <br><br>

### 打开串口

```kotlin
//简单创建串口，默认波特率:9600 数据位:8 校验位:无 停止位:1
val simpleClient = LSerialPortClient.Builder("/dev/ttysWK0").build()

//当然，你可以通过建造类类自由的配置各种参数
val customClient = LSerialPortClient.Builder("/dev/ttysWK0")
    .baudrate(BaudRate.B_115200)//波特率
    .dataBits(DataBits.EIGHT)//数据位
    .parity(Parity.NONE)//校验位
    .stopBits(StopBits.ONE)//停止位
    .checkIntervalWaitMills(0)//循环查询间隔等待时间，等待时间越长一次返回数据量越多，单位：ms
    .build()
    
//用client参数打开串口，当返回result == 0 时串口开启成功
val result = simpleClient.open()
if(result == 0){
  //串口开启成功...
}
```

### 发送数据

```kotlin
//消息数据（byte数组类型）
val msg = byteArrayOf(0xFF.toByte(),0x00.toByte(),0x01.toByte(),0x02.toByte(),0x03.toByte(),0x04.toByte(),0x05.toByte(),0x06.toByte(),0xFE.toByte())
//发送消息
client.sendMsg(msg)

//当然你也可以在子线程中发送消息，无需关心线程问题
Thread{
   client.sendMsg(msg)
}.start()
```

### 接收数据

```kotlin
//设置回调监听器，多次设置会替换前一个监听器
client.setListener { data ->
    Log.e("LSerialPortClientExample", "异步线程接收到数据，数据大小：${data.size}")
}
```

### 关闭串口

```kotlin
//关闭串口根据返回result判断是否关闭成功，当result == 0 关闭成功
val result = client.close()
if(result == 0){
  //串口关闭成功...
}
```

<br><br>

## 非异步操作串口

LSerialPort同时也提供了非异步操作串口的类[LSerialPortSyncClient](https://github.com/RedRackham-R/LSerialPort/blob/v2/LSerialPort/src/main/java/com/redrackham/client/LSerialPortSyncClient.kt)，给想要自己控制串口读写线程的同学。下面是一些简单调用说明。<br>

具体使用可以查看 [**>参考代码<**](https://github.com/RedRackham-R/LSerialPort/blob/v2/app/src/main/java/com/lxy/lserialport/guide/LSerialPortSyncClientExample.kt) <br><br>

### 打开串口

**LSerialPortSyncClient**

```kotlin
//创建clientBuilder，该client操作串口：ttysWK0。通过建造者我们可以选择定制参数
val client = LSerialPortSyncClient.Builder("/dev/ttysWK0")
    .baudrate(BaudRate.B_115200)//波特率
    .dataBits(DataBits.EIGHT)//数据位
    .parity(Parity.NONE)//校验位
    .stopBits(StopBits.ONE)//停止位
    //读取数据超时时间，当传入
    //   -1:一直阻塞线程等待返回
    //    0:无论有没有数据，立即返回。如果没有数据，则返回一个空的byte数组
    //大于0:阻塞当前线程等待返回时间。如果没有数据，则返回一个空的byte数组  单位（ms）
    .readTimeoutMills(-1)
    .build()

//用client参数打开串口，当返回result == 0 时串口开启成功 
val result = simpleClient.open()
if(result == 0){
    //串口开启成功... 
}
```

### 发送数据

```kotlin
val data = byteArrayOf(0xFF.toByte(),0x00.toByte(),0x01.toByte(),0x02.toByte(),0x03.toByte(),0x04.toByte(),0x05.toByte(),0x06.toByte(),0xFE.toByte())

//阻塞式 发送一条数据
client.write(data)
```

### 接收数据

```kotlin
//阻塞式 读数据，阻塞时长根据初始化设置的超时时间决定
val data = client.read()
Log.d("LSerialPortSyncClientExample", "接收到数据，数据大小：${data.size}")
```

### 检查串口是否有数据可读取

```kotlin
//检查串口是否有数据回传
val dataAvailable = client.dataAvailable()
Log.d("LSerialPortSyncClientExample", "串口是否有数据可接收：${dataAvailable}")
```

### 关闭串口

```kotlin
//关闭串口根据返回result判断是否关闭成功，当result == 0 关闭成功
val result = client.close() 
if(result == 0){
  //串口关闭成功... 
}
```

<br><br>

# 工程开发环境信息

NDK ：23.1.7779620 <br>
C++ ：17 <br>
Android Gradle Plugin ：7.4.1<br>
Gradle ：7.5 <br>
Android Studio ：Android Studio Electric Eel | 2022.1.1 Patch 1 <br><br>

# 编译工程生成AAR

1.  导入工程配置后选择Android Studio 中的build -> Refresh Linked C++ Projects 等待Gradle build完成。<br> <img width="340" alt="63f048d3450c8e4f3a9e3f12ffdf325" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/9f77cbdfb78648deb638b2387dc2e484~tplv-k3u1fbpfcp-watermark.image?">

2.  Gradle build完成后选择Rebuild Project 等待Gradle build完成。<br> <img width="340" alt="323f9c45804d06a432885f14fbbfb9c" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e523ff583af84dc698c39cb16e255fe9~tplv-k3u1fbpfcp-watermark.image?">

3.  完成后在LSerialPort/build/outputs/aar/目录下会看到LSerialPort-debug.aar文件<br> <img width="340" alt="f8eb60ecd662347259874a816ec7a11" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ce8a8dda2428475e9ef071b28c1dac03~tplv-k3u1fbpfcp-watermark.image?">

<br>

## 可能会遇到的问题

### 1. 调用函数是返回-1失败，我该如何查看是什么问题

返回错误码-1时，logcat中会打印错误信息，可以通过Error等级寻找"LSerialPortLog"关键字查看。

    package:mine level:error LSerialPortLog

<img width="500" alt="2a4f6285874dfb001cb817e97924afc" src="https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/92ba70b5b2864402a6ef2d1dac39225b~tplv-k3u1fbpfcp-watermark.image?">

### 2. 新版Studio怎么下载某个版本的NDK or 新版的SDK Manager找不到想要的NDK版本

首先，打开Studio的Tools -> SDK Manage <br><br> <img width="360" alt="a9b279b844028fea09238cca48c5a37" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/2cefd64ca7d2474e935327fa9cddca96~tplv-k3u1fbpfcp-watermark.image?">

然后，打开页面后先选择 SDK Tools选项 <br><br> <img width="500" alt="d1fc5f8b0dfeb715c1390a92b00ba4c" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/dc096401dff0410ba4ce8f248ccf7c4a~tplv-k3u1fbpfcp-watermark.image?">

最后，勾选Show Package Details选项即可下载想要版本的NDK <br><br> <img width="500" alt="2f577bfdcdda4fd5b73c2d3eb7f6613" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/cd1c110081504238bd0d87b4c96e5c37~tplv-k3u1fbpfcp-watermark.image?">

### 3. 想要打包运行其他架构的包。如x86平台

打开LSerialPort库的Modele Gradle，在NDK内填写需要的架构，如下图内的x86 <br><br> <img width="450" alt="07c2b43ad86f7ff01ed7dcfbeecd138" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/c1f0aa9b9caa4ecca73b951980e4d71a~tplv-k3u1fbpfcp-watermark.image?">

然后删除app以及LSerialPort中的build目录 <br><br> <img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5f4b194411464a44998b1c6c28c22d14~tplv-k3u1fbpfcp-watermark.image?">

最后重新编译生成AAR即可 <br><br> <img width="320" alt="cbf1a9173a7a205ca019501facc90ae" src="https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/efd6f2131f8f41568afa8484f876eea0~tplv-k3u1fbpfcp-watermark.image?">

### 4. 想要打包release版本的AAR

打开studio的Build Variants。app与LSerialPort都改为release后，创建release需要的签名文件并重新打包 <br> <img width="320" alt="da0c9bb910a2cb306095cfdd40447ee" src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ebde69799b784f439b0a484439d32f75~tplv-k3u1fbpfcp-watermark.image?">

### 5. 为什么有时候Refresh Linked C++ Projects以及Rebuild Project后没有AAR文件

先把app以及LSerialPort目录下的build删除，然后重新Refresh Linked C++ Projects以及Rebuild Project，如果还是没有生成多rebuild几次 <br> <img width="320" alt="cbf363f3ca6e83f9805f5d05fa1b52a" src="https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/3ce577810dc04c22950541a0171a7d66~tplv-k3u1fbpfcp-watermark.image?"> <br>

# LSerialPort设计思路

串口操作实现，采用[CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)库，该库封装了对串口开关、读写功能。多线程串口操作部分，采用[MserialPort](https://github.com/flykule/MserialPort)库的思路进行开发实现，下面是多线程部分介绍以及思路讲解。<br><br>
首先，我们对串口读写操作设计三个线程任务，分别处理对串口进行读，写，以及检查数据并通知唤醒读线程进行数据读取。这里多一个检查通知线程的意义在于我们能自行定制读取数据时机，<br><br>
通过修改每次检查后的等待时间，可以减少读取次数，同时增加一次读取的数据量。<br><br>
其次，由于涉及到多串口，我们还要实现串口间的通信，以及实现安全关闭线程以及回收资源。避免内存泄漏问题。<br><br>
最后，还需要一个管理者，对每个打开的串口进行增删查改功能。。<br><br>
下面是每个功能具体实现的介绍。

## 1.写线程<br>

要实现非阻塞异步发送，我们这里采用异步线程非阻塞式提交消息到消息队列，写线程中轮循读取消息队列取出消息数据并发送的方式实现。项目中我们使用C++中的[std::queue](https://zh.cppreference.com/w/cpp/container/queue)来作为消息队列，但是[std::queue](https://zh.cppreference.com/w/cpp/container/queue)是线程不安全的队列，所以需要
一个锁来确保读写队列时的线程安全，这里使用C++中[std::mutex](https://zh.cppreference.com/w/cpp/thread/mutex)以及[std::lock\_guard](https://zh.cppreference.com/w/cpp/thread/lock_guard)创建互斥锁，通过互斥锁来实现异步发送消息并保证线程安全。这样我们发送消息的功能就大致完成。现在还差一个问题，就是如何在发送消息后通知读线程读取消息队列的内容并发送，这里涉及到异步线程通信。在C++标准库中，给我们提供了[std::condition\_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)，[std::condition\_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)可以阻塞一个或多个线程，直到其他线程修改共享变量或条件，并通知有意修改变量的线程。这样，最后一个线程异步通信的问题也解决了。 <br><br>
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

有了前面写线程的经验，读线程也可以利用前面提到的[std::mutex](https://zh.cppreference.com/w/cpp/thread/mutex)、[std::lock\_guard](https://zh.cppreference.com/w/cpp/thread/lock_guard)以及[std::condition\_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)来实现线程之间的通信以及保证共有数据的线程安全。线程问题现在已经解决了，那我们该怎么把读到的数据返回给上层呢？答案是JNI中[JNIEnv](https://blog.csdn.net/createchance/article/details/53783490)的GetObjectClass以及GetMethodID获取java回调函数把数据回传给上层。同时，我们也增加一个监听器锁\_mListenerMutex，实现安全设置监听器。

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

在设计思路中我们提到过，增加一个检查通知线程的目的是为了能自行定制读取数据的实际。满足一些需要减少读取次数同时时效性要求不怎么高，一次返回尽可能多数据的场景。这里我们通过获取到[std::condition\_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)对读线程进行通信。<br><br>
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

*   到此我们多线程对串口读写的功能已经完成，这里封装成一个名叫[ReadWriteWorker](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/include/LSerialPort/ReadWriteWorker.h) 的类来对功能进行管理。

## 4.安全关闭/退出 <br>

既然打开了这么多线程处理任务，当然如何安全的关闭线程以及串口也是我们需要考虑的功能。这里我们首先是需要考虑该如何关闭时通知到各个线程退出循环结束任务。在前面讲解各个线程设计时讲到，线程使用[std::condition\_variable](https://zh.cppreference.com/w/cpp/thread/condition_variable)进行唤醒后会根据共享内容条件判断是否继续阻塞，我们也可以利用这个特性增加判断机制。比如用[std::atomic](https://zh.cppreference.com/w/cpp/atomic/atomic)包装一个变量做原子操作进行条件判断。当然，LSerialPort里使用另外一种方式，那就是[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)以及[std::future](https://zh.cppreference.com/w/cpp/thread/future)来实现。[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)提供一种存储值或异常的机制，而[std::future](https://zh.cppreference.com/w/cpp/thread/future)是能访问异步操作结果。[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)可以创建[std::future](https://zh.cppreference.com/w/cpp/thread/future)作关联使用。简单来说就是[std::promise](https://zh.cppreference.com/w/cpp/thread/promise)允许设置一个值，或者异常，这个值或异常可以在未来的某个时刻被关联的[std::future](https://zh.cppreference.com/w/cpp/thread/future)对象访问到。多线程中也可以通过[std::future](https://zh.cppreference.com/w/cpp/thread/future)来实现异步参数获取。 <br><br>
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
下面是关闭销毁的功能函数ReadWriteWorker::interrupte()以及ReadWriteWorker::\~ReadWriteWorker()<br><br>
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

[ReadWriteWorker::\~ReadWriteWorker()](https://github.com/RedRackham-R/LSerialPort/blob/master/LSerialPort/src/main/cpp/ReadWriteWorker.cpp)

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

前面已经封装好对串口操作读写的对象ReadWriteWorker，现在只需要使用一个管理类，对每个串口操作对象进行管理。缓存我们使用[std::unordered\_map](https://zh.cppreference.com/w/cpp/container/unordered_map)以串口地址和串口操作对象作为键值对的形式进行保存，添加，删除等操作，使用[std::unordered\_map](https://zh.cppreference.com/w/cpp/container/unordered_map)的好处在于它内部实现结构采用对键值进行哈希保存数据，不做任何排序，能对map中的单一值进行快速访问，符合我们现在的实现功能要求。<br><br>
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

最近学习了C++基础，总感觉需要上手撸点东西。正好公司项目有使用串口的场景，但是发现大多数串口库一般都需要使用者自己在上层实现对串口多线程管理读写，有些不方便。网上冲浪时发现个优秀的C++底层实现多线程串口读写管理的库\[MserialPort] <https://github.com/flykule/MserialPort>)
，遂按照该库的代码思路写了LSerialPort。一方面巩固刚学习学习的C++知识，另一方面是重拾很久没用过的JNI、NDK。

# 特别感谢

### [MserialPort](https://github.com/flykule/MserialPort)

### [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)

# C++学习推荐

### [CppReference](https://zh.cppreference.com/w/%E9%A6%96%E9%A1%B5)

### [RUNNOOB](https://www.runoob.com/cplusplus/cpp-tutorial.html)
