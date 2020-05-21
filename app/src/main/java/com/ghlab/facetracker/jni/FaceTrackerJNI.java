package com.ghlab.facetracker.jni;

import android.content.res.AssetManager;
import android.graphics.Bitmap;

public class FaceTrackerJNI {
    native public static void init(AssetManager am);
    native public static FaceTrackerResult track(Bitmap bitmap);
}
