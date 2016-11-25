/*
 *  stereo_match.cpp
 *  calibration
 *
 *  Created by Victor  Eruhimov on 1/18/10.
 *  Copyright 2010 Argus Corp. All rights reserved.
 *
 */

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include <stdio.h>
#include<QString>
#include<stdlib.h>
#include<stdio.h>
#include<stereomatch.h>
#include"stereomatchparam.h"
using namespace cv;
using namespace std;
void show_Isaline(Mat In_left_img,Mat In_right_img,Size In_imagesize,Rect In_left,Rect In_right);
void equalizeHist3ch(Mat In_img,Mat out_img);
void read_E_I_Param(std::string In_intrinsic_filename,std::string In_extrinsic_filename,
                    Mat *Out_M1,Mat* Out_M2,Mat *Out_D1,Mat *Out_D2,
                    Mat *Out_R1,Mat *Out_R2,Mat *Out_P1,Mat *Out_P2,
                    Mat *Out_Q,Mat *Out_R,Mat *Out_T);
void on_mouse(int event, int x, int y, int flags, void* ustc);
int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor,int m_numberOfDisparies);
int getPointClouds(cv::Mat& disparity, cv::Mat& pointClouds,Mat& m_Calib_Mat_Q);
void detectDistance(cv::Mat& pointCloud,int *pic_info);
static void saveXYZ(const char* filename, const Mat& mat)
{
    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}

int main(int argc, char** argv)
{

    namedWindow("left", 1);
    namedWindow("right", 1);
    namedWindow("disparity", 1);
    namedWindow("point_cloud_filename", 1);
    namedWindow("ceshi",1);
    cap1.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    cap2.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap2.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    stereo_init(cap1,cap2,argc,argv);
 // show_Isaline(img1_buf,img2_buf,img1.size(),roi1,roi2);
 // ceshi=imread("disp80.jpg");
  //cvtColor(ceshi,ceshigray, CV_BGR2GRAY);
  //imshow("ceshi",ceshigray);

  printf("ceshi ch:%d\n",ceshigray.channels());
  setMouseCallback("disparity",on_mouse,0);
while(true)
{
    cap1>>img1;
    cap2>>img2;
    img1.copyTo(image);
    if (img1.empty())
    {
        printf("Command-line parameter error: could not load the first input image file\n");
        return -1;
    }
    if (img2.empty())
    {
        printf("Command-line parameter error: could not load the second input image file\n");
        return -1;
    }
    if (scale != 1.f)
    {
        Mat temp1, temp2;
        int method = scale < 1 ? INTER_AREA : INTER_CUBIC;
        resize(img1, temp1, Size(), scale, scale, method);
        img1 = temp1;
        resize(img2, temp2, Size(), scale, scale, method);
        img2 = temp2;
    }

        Mat img1r, img2r;
        remap(img1, img1r, map11, map12, INTER_LINEAR);
        remap(img2, img2r, map21, map22, INTER_LINEAR);
      //  show_Isaline(img1r,img2r,img1.size(),roi1,roi2);
        img1 = img1r;
        img2 = img2r;
        show_Isaline(img1r,img2r,img1.size(),roi1,roi2);
      // cvtColor(img1,img1,CV_BGR2GRAY);
     //   cvtColor(img2,img2,CV_BGR2GRAY);

        waitKey(5);
        int64 t = getTickCount();
    if( alg == STEREO_BM )
        bm->compute(img1, img2, disp);
    else if( alg == STEREO_SGBM || alg == STEREO_HH || alg == STEREO_3WAY )
        sgbm->compute(img1, img2, disp);
    t = getTickCount() - t;
  //  imshow("left",img1);imshow("right",img2);
   // printf("Time elapsed: %fms\n", t*1000/getTickFrequency());
    //disp = dispp.colRange(numberOfDisparities, img1p.cols);
    if( alg != STEREO_VAR )
        disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
    else
        disp.convertTo(disp8, CV_8U);
    getDisparityImage(disp,disp8,false,numberOfDisparities);
    getPointClouds(disp,pointcloud,Q);
    imshow("disparity", disp8);
    detectDistance(pointcloud,pic_info);
  //  printf("%d ,%d ,%d ,%d",disp8.at<uchar>(322,246),disp8.cols,disp8.rows,disp8.channels());
    // circle(disp8,cvPoint(320,240),10,CV_RGB(0,0,255),2,8,0);

 /*    uchar Maxx=2;uchar data_buf=0;
      for(int i=0;i<(disp8.rows)-40;i+=30)
          for(int j=0;j<(disp8.cols)-40;j+=30)
          {
             data_buf=disp8.at<uchar>(i,j);
              if(Maxx<=data_buf)
                 Maxx=data_buf;
          }

     int n=0;int sum_dis=0;
      for(int i=0;i<disp8.rows-10;i+=5)
          for(int j=0;j<disp8.cols-10;j+=5)
          {
              data_buf=disp.at<uchar>(i,j);
              if((Maxx-1)<=data_buf&&(Maxx>=data_buf))
              {
                  sum_dis+=data_buf;
                  n++;
              }
          }*/

     // printf("平均视差：%d ，检测数量：%d \n", sum_dis/n,n);

  }

#if 0
   show_Isaline(img1,img2,img1.size(),roi1,roi2);
#endif
    if( !no_display )
    {
    //    imshow("disparity", disp8);
        printf("press any key to continue...");
        fflush(stdout);
        waitKey(80);
        printf("\n");
    }
#if 0
    if(!disparity_filename.empty())
        imwrite(disparity_filename, disp8);
    if(!point_cloud_filename.empty())
    {
        printf("storing the point cloud...");
        fflush(stdout);
        Mat xyz;
        reprojectImageTo3D(disp, xyz, Q, true);
        saveXYZ(point_cloud_filename.c_str(), xyz);
        waitKey(60);
        imwrite("cloud.jpg",xyz);

        imshow("point_cloud_filename", xyz);
        waitKey(60);
        printf("\n");
    }
    printf("%d\n",point_cloud_filename.empty());
    waitKey();
    #endif
    return 0;
}

void show_Isaline(Mat In_left_img,Mat In_right_img,Size In_imagesize,Rect In_left,Rect In_right)
{
       Mat canvas;
       double sf;
       int w, h;
       sf = 600. / MAX(In_imagesize.width, In_imagesize.height);
       //printf()
       w = cvRound(In_imagesize.width * sf);
       h = cvRound(In_imagesize.height * sf);
      canvas.create(h, w * 2, CV_8UC3);
       /*左图像画到画布上*/
       Mat canvasPart = canvas(Rect(w*0, 0, w, h));								//得到画布的一部分
       resize(In_left_img, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);		//把图像缩放到跟canvasPart一样大小
       Rect vroiL(cvRound(In_left.x*sf), cvRound(In_left.y*sf),				//获得被截取的区域
                 cvRound(In_left.width*sf), cvRound(In_left.height*sf));
       rectangle(canvasPart,vroiL, Scalar(0, 0, 255), 3, 8);						//画上一个矩形
      // cout << "Painted ImageL" << endl;
       /*右图像画到画布上*/
       canvasPart = canvas(Rect(w, 0, w, h));										//获得画布的另一部分
       resize(In_right_img, canvasPart, canvasPart.size(), 0, 0, INTER_LINEAR);
       Rect vroiR(cvRound(In_right.x * sf), cvRound(In_right.y*sf),
                 cvRound(In_right.width * sf), cvRound(In_right.height * sf));
       rectangle(canvasPart, vroiR, Scalar(0, 255, 0), 3, 8);
     //  cout << "Painted ImageR" << endl;
       /*画上对应的线条*/
       for (int i = 0; i < canvas.rows;i+=16)
         line(canvas, Point(0, i), Point(canvas.cols, i), Scalar(0, 255, 0), 1, 8);
      imshow("rectified", canvas);
   //printf(grayImageL.)
   //cout << "wait key" << endl;
}
void equalizeHist3ch(Mat In_img,Mat out_img)
{
     vector<Mat> mv_left;
     split(In_img,mv_left);
     Mat R_img_left,G_img_left,B_img_left;
    equalizeHist(mv_left[0],R_img_left);
    equalizeHist(mv_left[1],G_img_left);
    equalizeHist(mv_left[2],B_img_left);
    vector<Mat> combined_left;
    combined_left.push_back(R_img_left);
    combined_left.push_back(G_img_left);
    combined_left.push_back(B_img_left);
    merge(combined_left, out_img);
}

void on_mouse(int event, int x, int y, int flags, void* ustc)
{
    Mat mouse_show;
        image.copyTo(mouse_show);
        if (event == CV_EVENT_LBUTTONDOWN)
        {
            pic_info[0] = y;
            pic_info[1] = x;
           // cout << "x:" << pic_info[0] << "y:" << pic_info[1] << endl;
            left_mouse = true;
            printf("%d  %d\n",x,y);
            //用于存储打印图片
            imwrite("mouse_show.jpg", image);
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
}


