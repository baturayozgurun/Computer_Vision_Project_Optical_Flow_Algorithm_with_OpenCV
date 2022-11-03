//  Developer: Baturay Özgürün
//  Copyright © 2018 Baturay Özgürün
//  If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

//  Project Name: Computer Vision Project -- Optical Flow Algorithm with OpenCV
//  Project Desciption: This algorithm calculates the optical flow of two consecutive frames (collected from a video) using the Lucas-Kanade method.

//Define the libraries
#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double pi = 3.14;
Mat FirstFramex, SecondFramex;
void SplitVideox();
double square(int x){return x*x;}
void allocateOnDemand(IplImage **img,CvSize size,int depth,int channels){
    *img=cvCreateImage(size,depth,channels);
}

int main_Optical_Flow_Lucas_Kanade(){
    Mat I1, I2, ImageColor; //Define the Mat variables
    
    SplitVideox(); //Execute the function and get the two consecutive images
    FirstFramex.copyTo(I1); //Copy the first image to the I1
    cvtColor(I1,I1, CV_BGR2GRAY); //Convert the first image into the grayscale
    SecondFramex.copyTo(I2); //Copy the second image to the I2
    cvtColor(I2,I2, CV_BGR2GRAY); //Convert the second image into the grayscale
    SecondFramex.copyTo(ImageColor); //Copy the second image to the SecondFrame
    
    CvSize FrameSize; //Get the size of the frame
    FrameSize.height = I1.rows;
    FrameSize.width = I1.cols;
    
    static IplImage *Frame1=NULL, *Frame2=NULL, *Frame=NULL, *Blank=NULL;
    allocateOnDemand(&Frame1,FrameSize,IPL_DEPTH_8U,1);
    allocateOnDemand(&Frame2,FrameSize,IPL_DEPTH_8U,1);
    allocateOnDemand(&Frame,FrameSize,IPL_DEPTH_8U,3);
    allocateOnDemand(&Blank,FrameSize,IPL_DEPTH_8U,1);
    
    IplImage ipltemp1=I1;
    IplImage ipltemp2=I2;
    IplImage ipltemp3=ImageColor;

    cvConvertImage(&ipltemp1,Frame1);
    cvConvertImage(&ipltemp2,Frame2);
    cvConvertImage(&ipltemp3,Frame);
    
    CvPoint2D32f Frame1_Features[400]; // It will contain the features found in frame 1.
    CvPoint2D32f Frame2_Features[400]; // It will contain the locations of the points from frame 1 in frame 2.
    
    int Number_of_Features=400;
    static IplImage *EigImage = NULL, *TempImage = NULL, *Pyramid1 = NULL, *Pyramid2 = NULL;
    allocateOnDemand(&EigImage,FrameSize,IPL_DEPTH_32F,1);
    allocateOnDemand(&TempImage,FrameSize,IPL_DEPTH_32F,1);
    allocateOnDemand(&Pyramid1,FrameSize,IPL_DEPTH_8U,1);
    allocateOnDemand(&Pyramid2,FrameSize,IPL_DEPTH_8U,1);
    
    // cvGoodFeaturesToTrack finds features of the given image (frame1).
    // INPUT - Image -> "frame1".
    // "EigImage" and "TempImage" are workspaces for the algorithm.
    // OUTPUT - Feature -> "Frame1_Features" will contain the feature points.
    // Number of features is 400.
    // The first ".01" specifies the minimum quality of the features.
    // The second ".01" specifies the minimum Euclidean distance between features.
    // "NULL" means use the entire input image.
    cvGoodFeaturesToTrack(Frame1, EigImage, TempImage, Frame1_Features, &Number_of_Features, .01, .01, NULL);
    
    char Optical_Flow_Found_Feature[400];
    float Optical_Flow_Feature_Error[400];
    CvSize Optical_Flow_Window=cvSize(3,3);
    
    // Define the criteria to terminate the optical flow. Stop when iteration is 20 or epsilon is better than 0.3
    CvTermCriteria Optical_Flow_Termination_Criteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3);
    
    // cvCalcOpticalFlowPyrLK finds the corresponding features on the second image (frame2).
    // "Frame1" is the first frame with the known features.
    // "Frame2" is the second frame where we want to find the first frame's features.
    // "Pyramid1" and "Pyramid2" are workspace for the algorithm.
    // "Frame1_Features" are the features from the first frame.
    // OUTPUT - Feature -> "Frame2_Features" will contain the feature points whose correspond to the features in the first frame.
    // "5" is the maximum number of pyramids to use. "0" means disable enhancements.
    cvCalcOpticalFlowPyrLK(Frame1,Frame2,Pyramid1,Pyramid2,Frame1_Features,Frame2_Features,Number_of_Features,Optical_Flow_Window,5,
                          Optical_Flow_Found_Feature,Optical_Flow_Feature_Error,Optical_Flow_Termination_Criteria,0);
    
    for(int i=0;i<Number_of_Features;i++){
        if (Optical_Flow_Found_Feature[i]== 0) continue; //When there is no feature, skip it.
        int Line_Thickness = 1;
        //Draw a line
        CvPoint p,q;
        p.x = (int) Frame1_Features[i].x;
        p.y = (int) Frame1_Features[i].y;
        q.x = (int) Frame2_Features[i].x;
        q.y = (int) Frame2_Features[i].y;
        
        double angle=atan2((double)p.y-q.y,(double)p.x-q.x);
        double hypotenuse=sqrt(square(p.y-q.y)+square(p.x-q.x));
        
        q.x=(int)(p.x-hypotenuse*cos(angle));
        q.y=(int)(p.y-hypotenuse*sin(angle));
        cvLine(Frame,p,q,Scalar(0,0,255),Line_Thickness,CV_AA,0);
        cvLine(Blank,p,q,Scalar(255,0,0),Line_Thickness,CV_AA,0);
        
        //Draw the tips of the arrow
        p.x=(int)(q.x+9*cos(angle+pi/4));
        p.y=(int)(q.y+9*sin(angle+pi/4));
        cvLine(Frame,p,q,Scalar(0,0,255),Line_Thickness,CV_AA,0);
        cvLine(Blank,p,q,Scalar(255,0,0),Line_Thickness,CV_AA,0);
        p.x=(int)(q.x+9*cos(angle-pi/4));
        p.y=(int)(q.y+9*sin(angle-pi/4));
        cvLine(Frame,p,q,Scalar(0,0,255),Line_Thickness,CV_AA,0);
        cvLine(Blank,p,q,Scalar(255,0,0),Line_Thickness,CV_AA,0);
    }
    
    cvShowImage("Lucas Kanade Method - Optical Flow on the Original Image",Frame);
    cvSaveImage("results/Lucas Kanade Method - Optical Flow on the Original Image.jpg",Frame);
    cvShowImage("Lucas Kanade Method - Optical Flow on the Blank Image",Blank);
    cvSaveImage("results/Lucas Kanade Method - Optical Flow on the Blank Image.jpg",Blank);
    
    waitKey(1000); //wait to show images
    return 0;
}

void SplitVideox(){
    VideoCapture cap("data/video.mp4"); //Read the viode
    Mat Frame; //Define the variable
    int FPS=floor(cap.get(CV_CAP_PROP_FPS)); //Store frame rate per second
    
    for(int FrameNum = 0; FrameNum < cap.get(CV_CAP_PROP_FRAME_COUNT);FrameNum++){
        cap >> Frame; // get the next frame from video
        if(FrameNum==FPS){ //In the first second
            Frame.copyTo(FirstFramex);
        }
        if(FrameNum==1.1*FPS){ //In the second second
            Frame.copyTo(SecondFramex);
        }
    }
}
