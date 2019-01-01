#include <jni.h>
#include <string>


#include "android/log.h"

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"liaowejian",FORMAT,##__VA_ARGS__);

#include "pthread.h"

//声明一个线程
pthread_t pthread;

//定义一个线程的回调
void *threadCallback(void *data) {

    LOGI("Hello From C++ Thread")

    //执行线程完毕之后，退出线程
    pthread_exit(&pthread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liaowj_jni_thread_JniThreadDemo_createThread(JNIEnv *env, jobject instance) {

    //创建 thread 对象
    pthread_create(&pthread, NULL, threadCallback, NULL);

}


//在 c++ 主线程调用 Java 方法。
extern "C"
JNIEXPORT void JNICALL
Java_com_liaowj_jni_thread_JniThreadDemo_callJavaMethodOnCPPMainThread(JNIEnv *env,
                                                                       jobject jobj) {
    //1. 得到 jclass
    jclass jclz = env->GetObjectClass(jobj);

    //2. 得到 jmethod
    jmethodID jmethod = env->GetMethodID(jclz, "onSuccess", "(Ljava/lang/String;)V");


    //得到 jstring
    char *msg = "Msg From C++ Thread";
    jstring jmsg = env->NewStringUTF(msg);


    //3. 调用函数
    env->CallVoidMethod(jobj, jmethod, jmsg);

    env->DeleteLocalRef(jmsg);

}