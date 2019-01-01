
#ifndef JNIEXAMPLE_JAVALISTENER_H
#define JNIEXAMPLE_JAVALISTENER_H


class JavaListener {


public:
    JavaVM *vm;
    JNIEnv *env;
    jobject jobj;
    jmethodID jmethod;

public:
    JavaListener(JavaVM *vm, JNIEnv *env, jobject jobj);

    //析构函数
    ~JavaListener();

    void onSuccess(const char *msg);
};


#endif //JNIEXAMPLE_JAVALISTENER_H
