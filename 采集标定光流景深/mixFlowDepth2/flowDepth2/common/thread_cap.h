#ifndef THREAD_CAP
#define THREAD_CAP
#include <linux/videodev2.h>
#include<test.h>
#include <sys/time.h>
//#include<signal.h>
#include<pthread.h>
#include<QDebug>
#include<QThread>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include"QTimer"
#include<QObject>
#include"opencv.hpp"
//#include <QApplication.h>
//unsigned int count=0;
using namespace cv;
class Producer_Image0 : public QThread
{
public :IplImage* Image0;
       int num=0;
public:
     void run()
     {
         Image0=cvCreateImage(cvSize(IMAGEWIDTH,IMAGEHEIGHT),IPL_DEPTH_8U,3);
         IplImage *frame5 = NULL;
         while(true)
        {
          frame5 = getaFram0() ;//cvQueryFrame( capturee );
         if(frame5!=NULL)
         {
             Image0=frame5;
             num++;
         }
          usleep(1000);
     }
    }
};

class Producer_Image1 : public QThread
{
public :IplImage* Image1=NULL;

            int num=0;

public:
     void run()
     {
         IplImage *frame5 = NULL;
          Image1=cvCreateImage(cvSize(IMAGEWIDTH,IMAGEHEIGHT),IPL_DEPTH_8U,3);
         while(true)
        {
          frame5 = getaFram1() ;
          if(frame5!=NULL)
          {
             Image1=frame5;
             num++;
          }
          usleep(1000);
     }
  }
};
#endif // THREAD_CAP0

