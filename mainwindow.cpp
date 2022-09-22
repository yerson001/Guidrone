#include "mainwindow.h"
#include "./ui_mainwindow.h"

Mat dummy_1ch = Mat::zeros(Size(800,600),CV_8UC1);
Mat dummy_3ch = Mat::zeros(Size(800,600),CV_8UC1);

QImage dummy_img_1ch(dummy_1ch.data,dummy_1ch.cols,dummy_1ch.rows,QImage::Format_Grayscale8);
QImage dummy_img_3ch(dummy_3ch.data,dummy_3ch.cols,dummy_3ch.rows,QImage::Format_RGB888);
Mat debugWindow(540, 1280, CV_8UC3, Scalar(0,0,0)); //The Debug window.


Point2f perspectiveSrc[] = {
    Point2f(570,300),
    Point2f(1000,300),
    Point2f(170,698),
    Point2f(1300,698)
};

Point2f perspectiveDst[] = {
    Point2f(0,0),
    Point2f(1280,0),
    Point2f(0,720),
    Point2f(1280,720)
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIntValidator *portValidator = new QIntValidator(0,255);
    ui->camPort->setValidator(portValidator);
    connect(ui->PBTrun,SIGNAL(clicked()),this,SLOT(runCamera()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::runCamera()
{
    tello.say_hello();

    //------------------------------------------------
    //------------  start drone conection    ---------
    //------------------------------------------------
    //    if (!tello.Bind())
    //    {
    //        QMessageBox::critical(this,"ERROR","please check your DRONE",QMessageBox::Ok);
    //        return;
    //    }

    //    tello.SendCommand("streamon");
    //    while (!(tello.ReceiveResponse()));
    //----------------------------------------------
    //------------  end drone conection    ---------
    //----------------------------------------------


    if(this->RUNNING==true){
        ui->src->setPixmap(QPixmap::fromImage(dummy_img_3ch).scaled(ui->src->width(),ui->src->height(),Qt::KeepAspectRatio));
        ui->PBTrun->setText("RUN");
    }
    else if(this->RUNNING==false){

        //-----------------------------------------------------------
        //----------------  start camera calibracion  ---------------
        //-----------------------------------------------------------

        FileStorage fsRead;
        std::string PATH = "/home/y3rsn/Dev/cpp/CV_2022/ALD/camera_cal/";
        fsRead.open("Intrinsic.xml", FileStorage::READ);
        Mat src = imread(PATH+"calibration1.jpg");
        Mat dst;
        if (fsRead.isOpened() == false)
        {
            CameraCalibrator myCameraCalibrator("gg",0);
            myCameraCalibrator.doCalibration(cameraMatrix, dist);
            FileStorage fs;
            fs.open("Intrinsic.xml", FileStorage::WRITE);
            fs << "CameraMatrix" << cameraMatrix;
            fs << "Dist" << dist;
            fs.release();
            fsRead.release();
            cout << "There is no existing intrinsic parameters XML file." << endl;
            cout << "Start calibraton......" << endl;
        }
        else
        {
            fsRead["CameraMatrix"] >> cameraMatrix;
            fsRead["Dist"] >> dist;
            fsRead.release();
        }
        undistort(src, dst, cameraMatrix, dist);

        //----------------------------------------------------------
        //-----------    finish camera calibration -----------------
        //----------------------------------------------------------



        //----------------------------------------------------------
        //-----------    Set camera port ---------------------------
        //----------------------------------------------------------
        int port = ui->camPort->text().toInt();

        // <--------- to web cam ---->
        //VideoCapture cap(port);

        //  <-------- to drone ------>
        //VideoCapture cap{TELLO_STREAM_URL, CAP_FFMPEG};

        // <--------- to read file ------>
        VideoCapture cap;
        cap.open("/home/y3rsn/Dev/cpp/CV_2022/ALD/video/video_.mp4");


        //----------------------------------------------------------
        //-----------    End camera port  --------------------------
        //----------------------------------------------------------




        //----------------------------------------------------------
        //----------------  Check camera state ---------------------
        //----------------------------------------------------------
        if(!cap.isOpened()){
            QMessageBox::critical(this,"ERROR","please check your camera",QMessageBox::Ok);
            return;
        }
        this->camPort = port;
        ui->PBTrun->setText("STOP");
        this->RUNNING = true;
        //----------------------------------------------------------
        //----------------  END Check camera state -----------------
        //----------------------------------------------------------


        Mat frame;
        Mat mergeImage;
        Mat finalResult;



        //----------------------------------------------------------
        //----------------  Set Linealgo lib ---------------------
        //----------------------------------------------------------
        perspectiveMatrix = getPerspectiveTransform(perspectiveSrc,perspectiveDst);
        videoSize = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),(int)cap.get(CAP_PROP_FRAME_HEIGHT));

        cap.set(CAP_PROP_POS_FRAMES, 0);
        cap >> frame;

        undistort(frame, videoFrameUndistorted, cameraMatrix, dist);
        _videoFrameUndistorted = videoFrameUndistorted.clone();
        laneDetection LaneAlgoVideo(_videoFrameUndistorted, perspectiveMatrix);

        //----------------------------------------------------------
        //----------------  END LineAlgo lib  ----------------------
        //----------------------------------------------------------



        while(RUNNING==true or !frame.empty()){

            warpPerspective(_videoFrameUndistorted, videoFramePerspective, perspectiveMatrix, videoSize);
            LaneAlgoVideo.laneDetctAlgo(" ");
            finalResult = LaneAlgoVideo.getFinalResult();

            cap>>frame;
            if(frame.empty()) break;

            //Calibration
            undistort(frame, videoFrameUndistorted, cameraMatrix, dist);
            _videoFrameUndistorted = videoFrameUndistorted.clone();
            LaneAlgoVideo.setInputImage(_videoFrameUndistorted);

            //imshow("frame",frame);

            cvtColor(frame,frame,COLOR_BGR2RGB);
            QImage src_;
            src_ = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
            ui->src->setPixmap(QPixmap::fromImage(src_).scaled(ui->src->width(),ui->src->height(),Qt::KeepAspectRatio));

            waitKey(1);
        }
        cap.release();
    }
}



void draw_lines(Mat frame){
    circle(frame,perspectiveSrc[0], 3, Scalar(0,255,0), FILLED);
    circle(frame,perspectiveSrc[1], 3, Scalar(0,255,0), FILLED);
    circle(frame,perspectiveSrc[3], 3, Scalar(0,255,0), FILLED);
    circle(frame,perspectiveSrc[2], 3, Scalar(0,255,0), FILLED);
    line(frame, perspectiveSrc[0], perspectiveSrc[1], Scalar(255,0,0), 1);
    line(frame, perspectiveSrc[1], perspectiveSrc[3], Scalar(255,0,0), 1);
    line(frame, perspectiveSrc[3], perspectiveSrc[2], Scalar(255,0,0), 1);
    line(frame, perspectiveSrc[2], perspectiveSrc[0], Scalar(255,0,0), 1);
}
