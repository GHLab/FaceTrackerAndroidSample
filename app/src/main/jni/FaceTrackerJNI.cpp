#include "FaceTrackerJNI.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <fstream>

#include "AssetIstream.h"
#include "FaceTracker.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_ghlab_facetracker_jni_FaceTrackerJNI_init(JNIEnv *env, jobject instance, jobject am)
{
    AAssetManager *assetManager = AAssetManager_fromJava(env, am);

    FaceTracker::instance()->init(assetManager);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_ghlab_facetracker_jni_FaceTrackerJNI_track(JNIEnv *env, jobject instance, jobject bitmap)
{
    AndroidBitmapInfo info;
    void *pixels = 0;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0 || AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0)
        return nullptr;

    cv::Mat frame(info.height, info.width, CV_8UC4, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);

    cv::Rect faceRect;

    std::vector<cv::Point> landmarks;

    double yaw = 0, pitch = 0, roll = 0;

    if (!FaceTracker::instance()->track(frame, faceRect, landmarks, yaw, pitch, roll))
        return NULL;

    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListInitMId = env->GetMethodID(arrayListClass, "<init>", "(I)V");
    jmethodID arrayListAddMId = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jclass faceTrackerResultClass = env->FindClass("com/ghlab/facetracker/jni/FaceTrackerResult");
    jmethodID faceTrackerResultInitMId = env->GetMethodID(faceTrackerResultClass, "<init>", "(Landroid/graphics/Rect;Ljava/util/List;DDD)V");

    jclass rectClass = env->FindClass("android/graphics/Rect");
    jmethodID rectInitMId = env->GetMethodID(rectClass, "<init>", "(IIII)V");

    jclass pointClass = env->FindClass("android/graphics/Point");
    jmethodID pointInitMId = env->GetMethodID(pointClass, "<init>", "(II)V");

    jobject jFaceRect = env->NewObject(rectClass, rectInitMId, (float)faceRect.x, (float)faceRect.y, (float)(faceRect.x + faceRect.width), (float)(faceRect.y + faceRect.height));

    jobject jLandmarks = env->NewObject(arrayListClass, arrayListInitMId, landmarks.size());

    for (auto &landmark : landmarks) {
        jobject pt = env->NewObject(pointClass, pointInitMId, landmark.x, landmark.y);
        env->CallBooleanMethod(jLandmarks, arrayListAddMId, pt);
        env->DeleteLocalRef(pt);
    }

    jobject faceTrackerResult = env->NewObject(faceTrackerResultClass, faceTrackerResultInitMId, jFaceRect, jLandmarks, yaw, pitch, roll);

    env->DeleteLocalRef(jLandmarks);
    env->DeleteLocalRef(jFaceRect);

    env->DeleteLocalRef(pointClass);
    env->DeleteLocalRef(rectClass);
    env->DeleteLocalRef(faceTrackerResultClass);
    env->DeleteLocalRef(arrayListClass);

    return faceTrackerResult;
}