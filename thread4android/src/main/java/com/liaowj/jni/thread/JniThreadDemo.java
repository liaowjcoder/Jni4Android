package com.liaowj.jni.thread;

/**
 * @author liaowj
 * @time 2019/1/1 12:24 AM
 * @desc 创建第一个 C++ 线程
 **/
public class JniThreadDemo {


    static {
        System.loadLibrary("native-thread-lib");
    }

    public native void createThread();

}
