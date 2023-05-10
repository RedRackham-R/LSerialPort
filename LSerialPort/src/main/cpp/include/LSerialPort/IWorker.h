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

        virtual ~IWorker() = default;

        //发送消息
        virtual void doWork(const std::vector<uint8_t> &msg) = 0;

        //串口是否打开
        virtual bool isOpened() = 0;

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
