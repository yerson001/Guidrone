#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QIntValidator>
#include <QThread>
#include <QMessageBox>
#include <ctello.h>
#include "lanedetection.h"
#include "calibration.h"
#include "functions.h"

using namespace cv;
using namespace std;

const char* const TELLO_STREAM_URL{"udp://0.0.0.0:11111"};

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool RUNNING = false;
    int width;
    int height;
    int camPort = 0;
    ctello::Tello tello;
    funciones fun;

    //===================
    int sliderValue = 0;
    VideoCapture laneVideo;
    Mat videoFrame; // Video Frame.
    Mat videoFrameUndistorted; // Video Frame (after calibration).
    Mat videoFramePerspective; // Video Frame (after perspective transform).
    Mat _videoFrameUndistorted;

    Size videoSize; // Input Variable Size.
    Mat cameraMatrix, dist; //Calibration Matrix.
    Mat perspectiveMatrix; //Homography Matrix.
    String coordinatetext = "";
    Mat debugWindowROI;




private slots:
    void runCamera();
private:
    Ui::MainWindow *ui;
    void draw_lines(Mat frame);

};
#endif // MAINWINDOW_H
