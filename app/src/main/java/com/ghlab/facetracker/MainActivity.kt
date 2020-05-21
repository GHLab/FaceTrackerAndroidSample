package com.ghlab.facetracker

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.ghlab.facetracker.jni.FaceTrackerJNI

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        savedInstanceState ?: supportFragmentManager.beginTransaction()
            .replace(R.id.container, CameraBasicFragment.newInstance())
            .commit()


        System.loadLibrary("FaceTrackerLib")
        FaceTrackerJNI.init(this.assets);
    }
}
