//
// Created by 廖伟健 on 2018/11/9.
//

#include <jni.h>
#include "JavaListener.h"
#include "android/log.h"

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"liaowejian",FORMAT,##__VA_ARGS__);


JavaListener::JavaListener(JavaVM *vm, JNIEnv *env, jobject jobj) {
    this->vm = vm;
    this->env = env;
    this->jobj = jobj;

    jclass jclaz = env->GetObjectClass(jobj);

    jmethod = env->GetMethodID(jclaz, "onSuccess", "(Ljava/lang/String;)V");

}

void JavaListener::onSuccess(const char *msg) {

    JNIEnv *env;
    vm->AttachCurrentThread(&env, 0);

    jstring jmsg = env->NewStringUTF(msg);


    env->CallVoidMethod(jobj, jmethod, jmsg);

    env->DeleteLocalRef(jmsg);

    vm->DetachCurrentThread();

}

