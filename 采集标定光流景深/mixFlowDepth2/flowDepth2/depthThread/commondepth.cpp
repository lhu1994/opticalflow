//#include"depthparam.h"
/*void on_mouse(int event, int x, int y, int flags, void* ustc)
{
        Mat mouse_show;
        image.copyTo(mouse_show);
        if (event == CV_EVENT_LBUTTONDOWN)
        {
            pic_info[0] = x;
            pic_info[1] = y;
           // cout << "x:" << pic_info[0] << "y:" << pic_info[1] << endl;
            left_mouse = true;
            printf("%d  %d\n",x,y);
            //用于存储打印图片
            //imwrite(t, image);
        //  imwrite(t1, image1);
        //  num = num++;

        }
        else if (event == CV_EVENT_LBUTTONUP)
        {
            left_mouse = false;
        }
        else if ((event == CV_EVENT_MOUSEMOVE) && (left_mouse == true))
        {
        }
}*/
#include"opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include<QMutex>
using namespace cv;
extern VideoCapture cap,cap2;

QMutex mutex;
void initCap(VideoCapture& capture1,VideoCapture& capture2,Mat& frame)
{
   capture1.open(0);
   capture2.open(1);
    mutex.lock();
    cap>>frame;
    mutex.unlock();
}
void getFrames(VideoCapture& capture1,Mat& frame)
{
    mutex.lock();
    capture1>>frame;
    mutex.unlock();
}

