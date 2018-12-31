package com.liaowj.jni.thread;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void createThread(View view) {
        JniThreadDemo jniThreadDemo = new JniThreadDemo();

        jniThreadDemo.createThread();

    }
}
