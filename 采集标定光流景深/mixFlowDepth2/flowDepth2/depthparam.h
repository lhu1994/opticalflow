#ifndef DEPTHPARAM_H
#define DEPTHPARAM_H
#include<stdio.h>
#include<opencv.hpp>

using namespace cv;
void on_mouse(int event, int x, int y, int flags, void* ustc);
int argcdep;
char **argvdep;
std::string img1_filename="left.jpg";
std::string img2_filename="right.jpg";
std::string intrinsic_filename;
std::string extrinsic_filename;
std::string disparity_filename ;//= "";
std::string point_cloud_filename;// = "point_cloud";
Mat M1, D1, M2, D2,R, T, R1, P1, R2, P2,Q;
Mat img1,img2;
uchar data_buf=0;
Mat disp, disp8,ceshi,ceshigray,pointcloud,image;
int pic_info[4];
bool left_mouse = false;
Mat map11, map12, map21, map22;
int SADWindowSize, numberOfDisparities;
bool no_display;
float scale;
Rect roi1, roi2;
enum { STEREO_BM=0, STEREO_SGBM=1, STEREO_HH=2, STEREO_VAR=3, STEREO_3WAY=4 };
int alg = STEREO_SGBM;
VideoCapture cap;
VideoCapture cap1;
Ptr<StereoBM> bm = StereoBM::create(16,9);
Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,16,3);
#endif // DEPTHPARAM

