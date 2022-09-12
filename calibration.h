#ifndef CALIBRATION_H
#define CALIBRATION_H

#include<iostream>
#include<opencv2/calib3d.hpp>
#include<opencv2/core/types.hpp>
#include<opencv2/core/persistence.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core/types_c.h>

class CameraCalibrator
{
public:
    CameraCalibrator(std::string dir,int dev):PATH(dir),device(dev)
    {
        NumImages = 20;
    }

    void setFilename();
    void addPoints();
    void doCalibration(cv::Mat &cameraMatrix, cv::Mat &dist);

private:
    std::vector<std::string> m_filenames;
    std::vector<std::vector<cv::Point2f>> m_srcPoints;
    std::vector<std::vector<cv::Point3f>> m_dstpoints;
    cv::Size m_imageSize;
    std::string PATH;
    int device;
    int NumImages;
};

#endif // CALIBRATION_H
