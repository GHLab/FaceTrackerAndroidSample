package com.ghlab.facetracker.jni;

import android.graphics.Point;
import android.graphics.Rect;

import java.util.List;

public class FaceTrackerResult {
    public Rect faceRect;
    public List<Point> landmarks;
    public double yaw;
    public double pitch;
    public double roll;

    public FaceTrackerResult(Rect faceRect, List<Point> landmarks, double yaw, double pitch, double roll) {
        this.faceRect = faceRect;
        this.landmarks = landmarks;
        this.yaw = yaw;
        this.pitch = pitch;
        this.roll = roll;
    }
}
