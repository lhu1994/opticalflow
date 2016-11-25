#ifndef OPTICALFLOW
#define OPTICALFLOW
#include<stdlib.h>
#include<QThread>
#include"QCustomPlot/qcustomplot.h"
#include"videoio.hpp"

using namespace cv;
class opticalFlowCaculate:public QThread
{
    Q_OBJECT
public:
    inline float square(float a);
    void caculateOpticalFlow(VideoCapture& cap);
    void run();
signals:
    void update(float datax);
};
#endif // OPTICALFLOW

