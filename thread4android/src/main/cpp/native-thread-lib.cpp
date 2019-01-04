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

#include "JavaListener.h"

pthread_t childThread;

void *childCallback(void *data) {

    JavaListener *javaListener = (JavaListener *) data;
    javaListener->onSuccess("hello from child thread");
    pthread_exit(&childThread);
}


//定义一个全局 java vm 实例
JavaVM *jvm;
//在加载动态库时回去调用 JNI_Onload 方法，在这里可以得到 JavaVM 实例对象
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    jvm = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

//在 c++ 子线程调用 Java 方法。
extern "C"
JNIEXPORT void JNICALL
Java_com_liaowj_jni_thread_JniThreadDemo_callJavaMethodOnCppChildThread(JNIEnv *env,
                                                                        jobject instance) {
    JavaListener *javaListener = new JavaListener(jvm, env, env->NewGlobalRef(instance));
    pthread_create(&childThread, NULL, childCallback, javaListener);

}


//生产者与消费者模型

#include "pthread.h"
#include "queue"
//sleep
#include "unistd.h"

//定义一个队列
std::queue<int> queue;

//生产者线程
pthread_t product;
//消费者线程
pthread_t consumer;
//线程锁
pthread_mutex_t pthread_mutex;
//线程条件对象
pthread_cond_t pthread_cond;

bool isProcessing = false;

//生产者回调处理
void *productCallback(void *data) {

    while (isProcessing) {
        //死循环生产

        //加锁
        pthread_mutex_lock(&pthread_mutex);
        //生产一条数据
        queue.push(1);
        LOGI("生产者生产一个产品，目前还有%d个产品", queue.size());
        //通知消费者去消费
        pthread_cond_signal(&pthread_cond);
        //解锁
        pthread_mutex_unlock(&pthread_mutex);
        //每生产一个产品睡眠5秒
        sleep(5);

    }
    //退出线程---暂时不会执行这段代码
    pthread_exit(&product);

}

//消费者回调处理
void *consumerCallback(void *data) {

    //消费不断的消费
    while (isProcessing) {

        //加锁
        pthread_mutex_lock(&pthread_mutex);

        if (queue.size() > 0) {
            queue.pop();
            LOGI("消费者消费一个产品，还有%d个产品", queue.size());
        } else {
            LOGI("当前没产品，等待生产者生产");
            pthread_cond_wait(&pthread_cond, &pthread_mutex);
        }

        //解锁
        pthread_mutex_unlock(&pthread_mutex);
        //睡眠500ms
        usleep(500 * 1000);
    }
    //退出线程---暂时不会执行这段代码
    pthread_exit(&consumer);
}

//开启线程
extern "C"
JNIEXPORT void JNICALL
Java_com_liaowj_jni_thread_JniThreadDemo_mutex(JNIEnv *env, jobject instance) {

    if (isProcessing) {
        return;
    }
    isProcessing = true;

    //预先往队列中添加10个数据
    for (int i = 0; i < 10; i++) {
        queue.push(i);
    }

    //创建锁
    pthread_mutex_init(&pthread_mutex, NULL);
    //创建条件
    pthread_cond_init(&pthread_cond, NULL);

    //创建对应的线程
    pthread_create(&product, NULL, productCallback, NULL);

    pthread_create(&consumer, NULL, consumerCallback, NULL);

}

//停止线程
extern "C"
JNIEXPORT void JNICALL
Java_com_liaowj_jni_thread_JniThreadDemo_stopMutex(JNIEnv *env, jobject instance) {

    isProcessing = false;

    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
}