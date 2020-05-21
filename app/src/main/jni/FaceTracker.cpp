#include "FaceTracker.h"

#include <android/log.h>

#include "AssetIstream.h"

FaceTracker::FaceTracker()
{
}

void FaceTracker::init(AAssetManager *assetManager)
{
    AssetIstream aistream(assetManager, "face2.tracker");

    m_trackerPtr = std::make_shared<FACETRACKER::Tracker>();
    m_trackerPtr->Read(aistream);

    m_pose = m_trackerPtr->_clm._pglobl;

    m_wSize1.push_back(7);
    m_wSize2.push_back(11);
    m_wSize2.push_back(9);
    m_wSize2.push_back(7);

    m_init = true;
}


bool FaceTracker::track(const cv::Mat &frame, /*out*/cv::Rect &faceRect, /*out*/std::vector<cv::Point> &landmarks, /*out*/double &yaw, /*out*/double &pitch, /*out*/double &roll)
{
    if (!m_init)
        return false;

    faceRect.x = 0;
    faceRect.y = 0;
    faceRect.width = 0;
    faceRect.height = 0;

    landmarks.clear();

    yaw = 0;
    pitch = 0;
    roll = 0;

    cv::Mat gray;
    cv::cvtColor(frame, gray, CV_BGRA2GRAY);

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    std::vector<int> wSize = m_trackingFailed ? m_wSize2 : m_wSize1;

    if (m_trackerPtr->Track(gray, wSize, -1, m_nIter, m_clamp, m_fTol, true) == 0)
    {
        int idx = m_trackerPtr->_clm.GetViewIdx();
        __android_log_print(ANDROID_LOG_DEBUG, "FaceTracker", "viewIdx : %d", idx);
        m_trackingFailed = false;

        __generateResult(frame.cols, frame.rows, m_trackerPtr->_shape, faceRect, landmarks, yaw, pitch, roll);
    }
    else
    {
        __android_log_print(ANDROID_LOG_DEBUG, "FaceTracker", "tracking failed");
        m_trackerPtr->FrameReset();
        m_trackingFailed = true;
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    __android_log_print(ANDROID_LOG_DEBUG, "FaceTracker", "tracking duraction - %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return !m_trackingFailed;
}

void FaceTracker::__generateResult(const int imgWidth, const int imgHeight, cv::Mat &shape, /*out*/cv::Rect &faceRect, /*out*/std::vector<cv::Point> &landmarks, /*out*/double &yaw, /*out*/double &pitch, /*out*/double &roll)
{
    int n = shape.rows / 2;

    for(int i = 0; i < n; i++)
    {
        landmarks.push_back(cv::Point(shape.at<double>(i, 0), shape.at<double>(i + n, 0)));
    }

    double top, left, bottom, right;

    if (shape.at<double>(0, 0) < 20.5)
    {
        if (shape.at<double>(0, 0) < 0)
            left = 0;
        else
            left = shape.at<double>(0, 0);
    }
    else
    {
        left = shape.at<double>(0, 0) - 20;
    }

    if (shape.at<double>(16, 0) + 20 > imgWidth - 0.5)
    {
        if (shape.at<double>(16,0) > imgWidth)
            right = imgWidth;
        else
            right = shape.at<double>(16, 0);
    }
    else
    {
        right = shape.at<double>(16, 0) + 20;
    }

    if (shape.at<double>(8 + n, 0) > imgHeight - 0.5)
    {
        if (shape.at<double>(8 + n, 0) > imgHeight)
            bottom = imgHeight;
        else
            bottom = shape.at<double>(8 + n, 0);
    }
    else
    {
        bottom = shape.at<double>(8 + n, 0) + 20;
    }

    if (shape.at<double>(19 + n, 0) < 10.5)
    {
        if (shape.at<double>(19 + n, 0) < 0)
            top = 0;
        else
            top = shape.at<double>(19 + n, 0);
    }
    else
    {
        top = shape.at<double>(19 + n, 0) - 10;
    }

    faceRect.x = left;
    faceRect.y = top;
    faceRect.width = right - left;
    faceRect.height = bottom - top;

    pitch = m_pose.at<double>(1, 0) * 180 / 3.14;
    yaw = m_pose.at<double>(2, 0) * 180 / 3.14;
    roll = m_pose.at<double>(3, 0) * 180 / 3.14;
}