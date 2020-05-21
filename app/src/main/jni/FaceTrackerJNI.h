#pragma once

#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_ghlab_facetracker_jni_FaceTrackerJNI_init(JNIEnv *env, jobject instance, jobject am);

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ghlab_facetracker_jni_FaceTrackerJNI_track(JNIEnv *env, jobject instance, jobject bitmap);