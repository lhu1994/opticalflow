#ifndef DEPTHCACULATETHREAD
#define DEPTHCACULATETHREAD
//#include "mainwindow.h"
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include"opencv.hpp"
#include<QThread>
//#include"depthparam.h"
using namespace cv;
using namespace std;
class depthCaculateThread:public QThread
{
    Q_OBJECT
public:
   void show_Isaline(Mat In_left_img,Mat In_right_img,Size In_imagesize,Rect In_left,Rect In_right);
   void equalizeHist3ch(Mat In_img,Mat out_img);
   void read_E_I_Param(std::string In_intrinsic_filename,std::string In_extrinsic_filename,
                       Mat *Out_M1,Mat* Out_M2,Mat *Out_D1,Mat *Out_D2,
                       Mat *Out_R1,Mat *Out_R2,Mat *Out_P1,Mat *Out_P2,
                       Mat *Out_Q,Mat *Out_R,Mat *Out_T);
 //  void on_mouse(int event, int x, int y, int flags, void* ustc);
   int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor,int m_numberOfDisparies);
   int getPointClouds(cv::Mat& disparity, cv::Mat& pointClouds,Mat& m_Calib_Mat_Q);
   void detectDistance(cv::Mat& pointCloud,int *pic_info);
   int stereo_init(VideoCapture& cap1,VideoCapture& cap2,int argc,char **argv);
   void run();
};

#endif // DEPTHCACULATE_H
