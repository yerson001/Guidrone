#ifndef LANEDETECTION_H
#define LANEDETECTION_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <Eigen/Dense>
#include "math.h"

using namespace cv;
using namespace std;
using namespace Eigen;
#define debug(x) cout<<#x<<": "<<x<<endl
#define tdebug(x,y) cout<<#x<<": "<<x<<" | "#y<<": "<<y<<endl

class laneDetection
{
private:
    Mat perspectiveMatrix;
    Mat oriImage;
    Mat edgeImage;
    Mat warpEdgeImage;
    Mat warpOriImage;
    vector<Mat> imageChannels;
    Mat RedBinary;
    Mat mergeImage;
    Mat mergeImageRGB;
    Mat histImage;
    Mat maskImage;
    Mat maskImageWarp;
    Mat finalResult;
    vector<int> histogram; //histograma
    vector<Point2f> laneL;
    vector<Point2f> laneR;
    vector<Point2f> curvePointsL;
    vector<Point2f> curvePointsR;
    int laneLcount;
    int laneRcount;
    int midPoint; //mid.
    int midHeight;
    int leftLanePos; //limite carril
    int rightLanePos; //limite carril
    short initRecordCount; // registrar los 5 primeros frames
    const int blockNum; //numero de blockes
    int stepY; //Window moving step.
    const int windowSize; //tamaño slide windows
    Vector3d curveCoefL; //coeficiente de curva derecha
    Vector3d curveCoefR; //ciefucionete curva izquierda).
    Vector3d curveCoefRecordL[5]; //ultimos 5 registros
    Vector3d curveCoefRecordR[5]; //
    int recordCounter;
    bool failDetectFlag; // indicar si las marcas de carreteras se detectan 1
    void calHist();
    void boundaryDetection();
    void laneSearch(const int &lanePos, vector<Point2f> &_line, int &lanecount, vector<Point2f> &curvePoints, char dir);
    bool laneCoefEstimate();
    void laneFitting();

public:
    int errorframe;
    int count();
    laneDetection(Mat _oriImage, Mat _perspectiveMatrix);
    ~laneDetection();
    void laneDetctAlgo(string);
    Mat getEdgeDetectResult();
    Mat getWarpEdgeDetectResult();
    Mat getRedChannel();
    Mat getRedBinary();
    Mat getMergeImage();
    Mat getHistImage();
    Mat getMaskImage();
    Mat getWarpMask();
    Mat getFinalResult();
    float getLaneCenterDist();
    void setInputImage(Mat &image);
};

#endif // LANEDETECTION_H
