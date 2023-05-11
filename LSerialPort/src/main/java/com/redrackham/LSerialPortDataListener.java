package com.redrackham;

/**
 * LSerialPortDataListener 数据异步接收回调监听器
 */
public interface LSerialPortDataListener {
    void onDataReceived(byte[] data);
}
