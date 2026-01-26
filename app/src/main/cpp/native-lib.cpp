#include <jni.h>
#include <string>

// ✅ FIXED INCLUDE (case-correct & clean)
#include "Menu/ModMenu.hpp"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_moxxiemenu_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    std::string hello = "Hello from native-lib!";
    return env->NewStringUTF(hello.c_str());
}
