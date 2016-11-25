#include"depthparam.h"
#include "depthcaculate.h"
//extern void getfram(VideoCapture& cap,Mat& frame);
extern VideoCapture cap,cap1;
extern int argcdep;
extern char **argvdep;
extern void getFrames(VideoCapture& capture1,Mat& frame);
void depthCaculateThread::show_Isaline(Mat In_left_img, Mat In_right_img, Size In_imagesize, Rect In_left, Rect In_right)
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

void depthCaculateThread::equalizeHist3ch(Mat In_img, Mat out_img)
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

void depthCaculateThread::read_E_I_Param(string In_intrinsic_filename, string In_extrinsic_filename, Mat *Out_M1, Mat *Out_M2, Mat *Out_D1, Mat *Out_D2, Mat *Out_R1, Mat *Out_R2, Mat *Out_P1, Mat *Out_P2, Mat *Out_Q, Mat *Out_R, Mat *Out_T)
{
    float scalel=1.0f;
    FileStorage fs(In_intrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("error");
        printf("Failed to open file %s\n", In_intrinsic_filename.c_str());

    }
    fs["M1"] >> *Out_M1;fs["D1"] >> *Out_D1;
    fs["M2"] >> *Out_M2; fs["D2"] >> *Out_D2;
    *Out_M1 *= scalel;*Out_M2 *= scalel;
    fs.open(In_extrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file %s\n", In_extrinsic_filename.c_str());
    }
    fs["R"] >> *Out_R; fs["T"] >> *Out_T; fs["Q"]>>*Out_Q;
    fs["R1"]>>*Out_R1; fs["R2"]>>*Out_R2;
    fs["P1"]>>*Out_P1;fs["P2"]>>*Out_P2;
 #if 0
   //  stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );
 #endif
}

int depthCaculateThread::getDisparityImage(Mat &disparity, Mat &disparityImage, bool isColor, int m_numberOfDisparies)
{
    cv::Mat disp8u;
    if (disparity.depth() != CV_8U)
    {
        if (disparity.depth() == CV_8S)
        {
            disparity.convertTo(disp8u, CV_8U);
        }
        else
        {
            disparity.convertTo(disp8u, CV_8U, 255 / (m_numberOfDisparies*16.));
        }
    }
    else
    {
        disp8u = disparity;
    }
    // 转换为伪彩色图像 或 灰度图像
    if (isColor)
    {
        if (disparityImage.empty() || disparityImage.type() != CV_8UC3 || disparityImage.size() != disparity.size())
        {
            disparityImage = cv::Mat::zeros(disparity.rows, disparity.cols, CV_8UC3);
        }
        for (int y = 0; y<disparity.rows; y++)
        {
            for (int x = 0; x<disparity.cols; x++)
            {
                uchar val = disp8u.at<uchar>(y, x);
                uchar r, g, b;
                if (val == 0)
                    r = g = b = 0;
                else
                {
                    r = 255 - val;
                    g = val < 128 ? val * 2 : (uchar)((255 - val) * 2);
                    b = val;
                }
                disparityImage.at<cv::Vec3b>(y, x) = cv::Vec3b(r, g, b);
            }
        }
    }
    else
    {
        disp8u.copyTo(disparityImage);
    }
    return 1;
}

int depthCaculateThread::getPointClouds(Mat &disparity, Mat &pointClouds, Mat &m_Calib_Mat_Q)
{
    if (disparity.empty())
    {
        return 0;
    }
    //计算生成三维点云
//  cv::reprojectImageTo3D(disparity, pointClouds, m_Calib_Mat_Q, true);
    reprojectImageTo3D(disparity, pointClouds, m_Calib_Mat_Q, true);
    pointClouds *= 1.6;
    for (int y = 0; y < pointClouds.rows; ++y)
    {
        for (int x = 0; x < pointClouds.cols; ++x)
        {
            cv::Point3f point = pointClouds.at<cv::Point3f>(y, x);
            point.y = -point.y;
            pointClouds.at<cv::Point3f>(y, x) = point;
        }
    }
    return 1;
}
void depthCaculateThread::detectDistance(Mat &pointCloud, int *pic_info)
{
    if (pointCloud.empty())
    {
        return;
    }
    // 提取深度图像
    vector<cv::Mat> xyzSet;
    split(pointCloud, xyzSet);
    cv::Mat depth;
    xyzSet[2].copyTo(depth);
    // 根据深度阈值进行二值化处理
    double maxVal = 0, minVal = 0;
    cv::Mat depthThresh = cv::Mat::zeros(depth.rows, depth.cols, CV_8UC1);
    cv::minMaxLoc(depth, &minVal, &maxVal);
    double thrVal = minVal * 1.5;
    threshold(depth, depthThresh, thrVal, 255, CV_THRESH_BINARY_INV);
    depthThresh.convertTo(depthThresh, CV_8UC1);
    //imageDenoising(depthThresh, 3);
    double  distance = depth.at<float>(pic_info[0], pic_info[1]);
  //  cout << "distance:" << distance << endl;
    printf("%f\n",distance);
}

int depthCaculateThread::stereo_init(VideoCapture &cap1, VideoCapture &cap2, int argc, char **argv)
{

      //  if(!(cap2.isOpened()))
    cap2.open(0);
       // if(!(cap1.isOpened()))
    cap1.open(1);
    usleep(5000);
  // usleep(5000);
qDebug("%d:cap2.isOpened()?",cap2.isOpened());
    //cap1>>img1;
    getFrames(cap1,img1);
    getFrames(cap2,img2);
  // cap2>>img2;
   qDebug("\nimg2:\n%d",img2.channels());
  //  getfram(cap2,img2);
    int cn = img1.channels();
    Size img_size = img1.size();
    cv::CommandLineParser parser(argc, argv,
        "{help h||}{algorithm||}{max-disparity|144|}{blocksize|7|}{no-display||}{scale|1|}{i|intrinsics.yml|}{e|extrinsics.yml|}{o|disparity.jpg|}{p|point_cloud|}");
qDebug("hello");
#if 0
    if(parser.has("0")&&parser.has("1"))
    {
    img1_filename = parser.get<std::string>(0);
    img2_filename = parser.get<std::string>(1);
    }
#endif
    if (parser.has("algorithm"))
    {
        std::string _alg = parser.get<std::string>("algorithm");
        alg = _alg == "bm" ? STEREO_BM :
            _alg == "sgbm" ? STEREO_SGBM :
            _alg == "hh" ? STEREO_HH :
            _alg == "var" ? STEREO_VAR :
            _alg == "sgbm3way" ? STEREO_3WAY : -1;
    }
    numberOfDisparities = parser.get<int>("max-disparity");
    SADWindowSize = parser.get<int>("blocksize");
    scale = parser.get<float>("scale");
    no_display = parser.has("no-display");
    if( parser.has("i") )
    {
        intrinsic_filename = parser.get<std::string>("i");
        intrinsic_filename="intrinsics.yml";
        qDebug("%s\n",intrinsic_filename);
    }
    if( parser.has("e") )
        extrinsic_filename = parser.get<std::string>("e");

  //  if( parser.has("o") )
   // {
  //      disparity_filename = parser.get<std::string>("o");
   //     printf("%s\n",disparity_filename);
    //}

    if( parser.has("p") )
    {
    point_cloud_filename = parser.get<std::string>("p");
    printf("%s",point_cloud_filename);
    }

    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    if (scale < 0)
    {
        printf("Command-line parameter error: The scale factor (--scale=<...>) must be a positive floating-point number\n");
        return -1;
    }
    if (SADWindowSize < 1 || SADWindowSize % 2 != 1)
    {
        printf("Command-line parameter error: The block size (--blocksize=<...>) must be a positive odd number\n");
        return -1;
    }
    if( img1_filename.empty() || img2_filename.empty() )
    {
        printf("Command-line parameter error: both left and right images must be specified\n");
        return -1;
    }
    if( (!intrinsic_filename.empty()) ^ (!extrinsic_filename.empty()) )
    {
        printf("Command-line parameter error: either both intrinsic and extrinsic parameters must be specified, or none of them (when the stereo pair is already rectified)\n");
        return -1;
    }

    if( extrinsic_filename.empty() && !point_cloud_filename.empty() )
    {
        printf("Command-line parameter error: extrinsic and intrinsic parameters must be specified to compute the point cloud\n");
        return -1;
    }

    if( !intrinsic_filename.empty() )
        read_E_I_Param(intrinsic_filename,extrinsic_filename,&M1,&M2,&D1,&D2,&R1,&R2,&P1,&P2,&Q,&R,&T);
    stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );
    int color_mode = alg == STEREO_BM ? 0 : -1;

    initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

    numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((img_size.width/8) + 15) & -16;
    bm->setROI1(roi1);
    bm->setROI2(roi2);
    bm->setPreFilterCap(31);
    bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
    bm->setMinDisparity(0);
    bm->setNumDisparities(numberOfDisparities);
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(15);
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);
    bm->setDisp12MaxDiff(1);
    sgbm->setPreFilterCap(63);
    int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
    sgbm->setBlockSize(sgbmWinSize);
    sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(15);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
   // alg = STEREO_BM;
    if(alg==STEREO_HH)
        sgbm->setMode(StereoSGBM::MODE_HH);
    else if(alg==STEREO_SGBM)
        sgbm->setMode(StereoSGBM::MODE_SGBM);
    else if(alg==STEREO_3WAY)
        sgbm->setMode(StereoSGBM::MODE_SGBM_3WAY);

  Mat img1_buf,img2_buf;
  remap(img1, img1_buf, map11, map12, INTER_LINEAR);
  remap(img2, img2_buf, map21, map22, INTER_LINEAR);
}

void depthCaculateThread::run()
{
    namedWindow("left", 1);
    namedWindow("right", 1);
    namedWindow("disparity", 1);
    namedWindow("point_cloud_filename", 1);
  //  namedWindow("ceshi",1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    cap1.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    stereo_init(cap,cap1,argcdep,argvdep);
 // show_Isaline(img1_buf,img2_buf,img1.size(),roi1,roi2);
  // ceshi=imread("disp80.jpg");
  // cvtColor(ceshi,ceshigray, CV_BGR2GRAY);
  // imshow("ceshi",ceshigray);

   //printf("ceshi ch:%d\n",ceshigray.channels());
   //setMouseCallback("disparity",on_mouse,0);
    while(true)
    {
        cap>>img2;
        cap1>>img1;
        img1.copyTo(image);
        if (img1.empty())
        {
            printf("Command-line parameter error: could not load the first input image file\n");

        }
        if (img2.empty())
        {
            printf("Command-line parameter error: could not load the second input image file\n");

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
            imshow("left",img1);
            usleep(8000);

            Mat img1r, img2r;
            remap(img1, img1r, map11, map12, INTER_LINEAR);
            remap(img2, img2r, map21, map22, INTER_LINEAR);

            img1 = img1r;
            img2 = img2r;
  //          show_Isaline(img1r,img2r,img1.size(),roi1,roi2);

            int64 t = getTickCount();
        if( alg == STEREO_BM )
            bm->compute(img1, img2, disp);
        else if( alg == STEREO_SGBM || alg == STEREO_HH || alg == STEREO_3WAY )
            sgbm->compute(img1, img2, disp);
        t = getTickCount() - t;
        if( alg != STEREO_VAR )
            disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
        else
            disp.convertTo(disp8, CV_8U);
        getDisparityImage(disp,disp8,false,numberOfDisparities);
        getPointClouds(disp,pointcloud,Q);
        imshow("disparity", disp8);
        usleep(50000);
        detectDistance(pointcloud,pic_info);
      }

    #if 0
       show_Isaline(img1,img2,img1.size(),roi1,roi2);
    #endif
        if( !no_display )
        {
        //    imshow("disparity", disp8);
            printf("press any key to continue...");
            fflush(stdout);
         //   waitKey(80);
            printf("\n");
        }
  }


