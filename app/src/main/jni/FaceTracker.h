#pragma once

#include <android/asset_manager.h>
#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>
#include <FaceTracker/Tracker.h>

class FaceTracker
{
public:
    FaceTracker(FaceTracker const&) = delete;
    FaceTracker& operator=(FaceTracker const&) = delete;

    static std::shared_ptr<FaceTracker> instance()
    {
        static std::shared_ptr<FaceTracker> s { new FaceTracker };
        return s;
    }

private:
    FaceTracker();

public:
    void init(AAssetManager *assetManager);
    bool track(const cv::Mat &frame, /*out*/cv::Rect &faceRect, /*out*/std::vector<cv::Point> &landmarks, /*out*/double &yaw, /*out*/double &pitch, /*out*/double &roll);

private:
    void __generateResult(const int imgWidth, const int imgHeight, cv::Mat &shape, /*out*/cv::Rect &faceRect, /*out*/std::vector<cv::Point> &landmarks, /*out*/double &yaw, /*out*/double &pitch, /*out*/double &roll);

private:
    bool m_trackingFailed = false, m_fcheck = false;

    std::vector<int> m_wSize1, m_wSize2;

    int m_nIter = 5;

    double m_clamp = 3, m_fTol = 0.01;

    std::shared_ptr<FACETRACKER::Tracker> m_trackerPtr;

    bool m_init = false;

    cv::Mat m_pose;
};