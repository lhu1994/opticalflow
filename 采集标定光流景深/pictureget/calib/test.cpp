#include <opencv2/opencv.hpp>
#include "test.h"
#include"opencv.hpp"
#include"opencv/cv.h"
#include"opencv2/core.hpp"
using namespace cv;
 V4L2cap v4l2cap0 = V4L2cap("/dev/video0");
 V4L2cap v4l2cap1 = V4L2cap("/dev/video2");
void capInit(void)
{
    v4l2cap0.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2cap0.buf.memory = V4L2_MEMORY_MMAP;
   v4l2cap1.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   v4l2cap1.buf.memory = V4L2_MEMORY_MMAP;
    if(v4l2cap0.init_v4l2() == FALSE){
        printf("Init fail~~\n");
      //  exit(1);
    }
    if(v4l2cap1.init_v4l2() == FALSE){
        printf("Init fail~~\n");
        exit(1);
    }
    printf("second~~\n");
    if(v4l2cap0.v4l2_grab() == FALSE){
        printf("grab fail~~\n");
      exit(2);
    }

    if(v4l2cap1.v4l2_grab() == FALSE){
        printf("grab fail~~\n");
        exit(2);
    }
}

IplImage*  getaFram1()
{

     IplImage*img1;//=cvCreateImage(cvSize(IMAGEWIDTH,IMAGEHEIGHT),IPL_DEPTH_8U,3);
     CvMat cvmat1;
        ioctl(v4l2cap1.fd,VIDIOC_DQBUF,&v4l2cap1.buf);
       v4l2cap1.buf.index = 0;
        cvmat1 = cvMat(IMAGEHEIGHT,IMAGEWIDTH,CV_8UC3,(void*)v4l2cap1.buffer);//CV_8UC3
       img1 = cvDecodeImage(&cvmat1,1);
       if(!img1)    printf("No img\n");
       ioctl(v4l2cap1.fd,VIDIOC_QBUF,&v4l2cap1.buf);
   //    if((cvWaitKey(1)&255) == 27)    exit(0);
      return  img1;
}
IplImage*  getaFram0()
{

      IplImage* img0;//=cvCreateImage(cvSize(IMAGEWIDTH,IMAGEHEIGHT),IPL_DEPTH_8U,3);
      CvMat cvmat0;
      ioctl(v4l2cap0.fd,VIDIOC_DQBUF,&v4l2cap0.buf);
       v4l2cap0.buf.index = 0;
       cvmat0 = cvMat(IMAGEHEIGHT,IMAGEWIDTH,CV_8UC3,(void*)v4l2cap0.buffer);//CV_8UC3
      img0 = cvDecodeImage(&cvmat0,1);
       if(!img0)    printf("No img\n");
       ioctl(v4l2cap0.fd,VIDIOC_QBUF,&v4l2cap0.buf);
       return img0;

}
