#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include<cv.h>
#include<highgui.h>
#include<iostream>
//#include <linux/videodev2.h>
#include<common/test.h>
//#include <sys/time.h>
#include<signal.h>
//#include<pthread.h>
#include<QDebug>
#include<QThread>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include"opencv.hpp"
//#include"thread_cap.h"
#include<flowThread/opticalflow.h>
#include <time.h>
#include<sys/time.h>
#include"Python.h"
#include<stdlib.h>
#include<stdio.h>
extern VideoCapture cap;
extern VideoCapture cap1;
using namespace std;
using namespace std;
using namespace cv;

 inline float opticalFlowCaculate::square(float a)
{
    return a * a;
}
void opticalFlowCaculate::caculateOpticalFlow(VideoCapture& cap)
{
        Mat pre,next,frame,fram2;
       // VideoCapture cap2;
        struct timeval time_start,time_end;
        float time_dev;
        static const double pi = 3.14159265358979323846;
        struct fly_v_A flow_v_a;
        int  x_dex=0;
        float vx_sum_squ,vy_sum_squ;
    //    cap.open(1);
        cap.open(2);
        float angle_sum_squ=0;
        vector<Point2f> prepoint,nextpoint;
        vector<uchar> state;
        vector<float>err;
       // namedWindow("frame",0);
        if(!cap.isOpened())cout<<"摄像头未打开，请打开摄像头！！！"<<endl;
        qDebug()<<"hello";
        for(;;)
        {
             int count=1;
             cap>>frame;
             gettimeofday(&time_end,NULL);
             if(frame.empty())break;
             cvtColor(frame,next,CV_BGR2GRAY);
                if(!next.empty()&&!pre.empty())
                {
                        time_dev=((time_end.tv_sec-time_start.tv_sec)*10+(time_end.tv_usec-time_start.tv_usec)/100000.);
                        time_dev= time_dev>5000?5000:time_dev;
                        time_dev=time_dev<0.02?0.02:time_dev;
                        goodFeaturesToTrack(pre,prepoint,400,0.001,10,Mat(),3,false,0.04);
                    //    cornerSubPix(pre,prepoint,Size(10,10),Size(-1,-1),TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
                        calcOpticalFlowPyrLK(pre,next,prepoint,nextpoint,state,err,Size(31,31),3);
                        for(int i=0;i<state.size()&&state.size()>=90;i++)
                        {
                            int dvx=0, dvy=0;
                            double vx=0,vy=0,angle;
                            if(state[i]!=0)
                                {
                                    dvx=nextpoint[i].x-prepoint[i].x;
                                    dvy=nextpoint[i].y-prepoint[i].y;
                                    vx=dvx/time_dev;
                                    vy=dvy/time_dev;
                                    angle = atan2(dvy,dvx);
                                    if(fabs(dvx)>=200||fabs(dvy)>=200
                                            ||fabs(dvx)<=2||fabs(dvx)<=2)continue;
                                    flow_v_a.vx_sum+=vx;
                                    flow_v_a.vy_sum+=vy;
                                    flow_v_a.angle_sum+=angle;

                                    vx_sum_squ+=square(vx);
                                    vy_sum_squ+=square(vy);
                                    angle_sum_squ+=square(angle);
                                    count++;
                                    line(frame,Point((int)prepoint[i].x,(int)prepoint[i].y),Point((int)nextpoint[i].x,(int)nextpoint[i].y),CV_RGB(255,0,0));

                            }
                        }
                        flow_v_a.count_point=count;
                        flow_v_a.vx_vari=vx_sum_squ/flow_v_a.count_point-square(flow_v_a.vx_sum/flow_v_a.count_point);
                        flow_v_a.vy_vari=vy_sum_squ/flow_v_a.count_point-square(flow_v_a.vy_sum/flow_v_a.count_point);
                        flow_v_a.angle_vari= angle_sum_squ/flow_v_a.count_point-square(flow_v_a.angle_sum/flow_v_a.count_point);

                        flow_v_a.vx=flow_v_a.vx_sum/flow_v_a.count_point;
                        flow_v_a.vy=flow_v_a.vy_sum/flow_v_a.count_point;
                        flow_v_a.angle=flow_v_a.angle_sum/flow_v_a.count_point;
                         qDebug("vx:%f\n vy:%f\n count_point=%d\n anglevari%f\time_dev=%f\n",flow_v_a.vx,flow_v_a.vy,flow_v_a.count_point,flow_v_a.angle_vari,time_dev);
                         float vxy=sqrt(square(flow_v_a.vx)+square(flow_v_a.vy));
                         if(flow_v_a.angle_vari>=0&&flow_v_a.angle_vari<=2)
                           printf("v=%f\n\n",vxy);
                         else
                       //  {
                             printf("v=0\n");
                         if(flow_v_a.count_point>=10)
                         emit
                             this->update(vxy);

                         else this->update(0);
                         vx_sum_squ=0;vy_sum_squ=0;angle_sum_squ=0;count=1; flow_v_a.vx_sum=0;flow_v_a.vy_sum=0;flow_v_a.angle_sum=0;
                       //
                        imshow("frame",frame);
                        usleep(10000);
                }
                x_dex++;
                if(x_dex>=100)
                    x_dex=0;

              //  waitKey(1);
                time_start=time_end;
                next.copyTo(pre);
        }
}
void opticalFlowCaculate::run()
{
    namedWindow("frame",0);
    VideoCapture cap2;
    while(true)
   {
    caculateOpticalFlow(cap2);
    usleep(1000);
   }
}

