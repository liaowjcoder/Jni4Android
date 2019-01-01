package com.liaowj.jni.thread;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.lang.reflect.Method;

/**
 * @author liaowj
 * @time 2019/1/1 12:24 AM
 * @desc 创建第一个 C++ 线程
 **/
public class JniThreadDemo {
    private Context mContext = null;

    public JniThreadDemo(Context context) {
        this.mContext = context;
    }

    public static final String TAG = JniThreadDemo.class.getSimpleName();

    static {
        System.loadLibrary("native-thread-lib");
    }

    public native void createThread();


    /**
     * 在主线程中调用，然后在主线程中会掉 onSuccess 函数
     */
    public native void callJavaMethodOnCPPMainThread();

    public void onSuccess(String msg) {
        //System.out.println("onSuccess invoke.."+msg);
        Toast.makeText(mContext, "JNI 回调 Java层 onSuccess 方法:" + msg, Toast.LENGTH_SHORT).show();
    }


    public static void main(String[] args) {

        try {
            Class<JniThreadDemo> clz = JniThreadDemo.class;

            Method method = clz.getMethod("onSuccess", String.class);

            JniThreadDemo jniThreadDemo = clz.newInstance();

            method.invoke(jniThreadDemo, "反射");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
