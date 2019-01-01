package com.liaowj.jni.thread;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

/**
 * @author liaowj
 * @time 2019/1/1 4:22 PM
 * @desc
 **/
public class MainActivity extends AppCompatActivity {
    private Context mContext = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;
    }

    public void createThread(View view) {
        JniThreadDemo jniThreadDemo = new JniThreadDemo(mContext);

        jniThreadDemo.createThread();

    }

    /**
     * 在主线程调用 Java 函数
     *
     * @param view
     */
    public void callJavaMethodInSameThread(View view) {
        JniThreadDemo jniThreadDemo = new JniThreadDemo(mContext);
        jniThreadDemo.callJavaMethodOnCPPMainThread();
    }
}
