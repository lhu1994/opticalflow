#ifndef STEREOMATCHPARAM
#define STEREOMATCHPARAM
#include "stereomatch.h"
void read_E_I_Param(string In_intrinsic_filename,string In_extrinsic_filename,
                    Mat *Out_M1,Mat *Out_M2,Mat *Out_D1,Mat *Out_D2,
                    Mat *Out_R1,Mat *Out_R2,Mat *Out_P1,Mat *Out_P2,Mat *Out_Q,
                    Mat *Out_R,Mat *Out_T);
static void print_help();
std::string img1_filename="left.jpg";
std::string img2_filename="right.jpg";
std::string intrinsic_filename;
std::string extrinsic_filename;
std::string disparity_filename = "";
std::string point_cloud_filename = "point_cloud";
Mat M1, D1, M2, D2,R, T, R1, P1, R2, P2,Q;
 Mat img1,img2;
VideoCapture cap1,cap2;
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

Ptr<StereoBM> bm = StereoBM::create(16,9);
Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,16,3);
#endif // STEREOMATCHPARAM

