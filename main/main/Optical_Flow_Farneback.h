//  Developer: Baturay Özgürün
//  Copyright © 2018 Baturay Özgürün
//  If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

//  Project Name: Computer Vision Project -- Optical Flow Algorithm with OpenCV
//  Project Desciption: This algorithm calculates the optical flow of two consecutive frames (collected from a video) using the Farneback method (calcOpticalFlowFarneback).

//Define the libraries
#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//Define global variables and a function
Mat FirstFrame, SecondFrame;
void SplitVideo();

int main_Optical_Flow_Farneback(){
    Mat I1, I2, Flow, Blank, ImageColor; //Define the Mat variables
    
    SplitVideo(); //Execute the function and get the two consecutive images
    FirstFrame.copyTo(I1); //Copy the first image to the I1
    cvtColor(I1,I1, CV_BGR2GRAY); //Convert the first image into the grayscale
    SecondFrame.copyTo(I2); //Copy the second image to the I2
    cvtColor(I2,I2, CV_BGR2GRAY); //Convert the second image into the grayscale
    SecondFrame.copyTo(ImageColor); //Copy the second image to the SecondFrame

    Blank=Mat::ones(I1.rows,I1.cols,CV_8UC3)*255; //Create a full of white image that size of
    
    calcOpticalFlowFarneback(I1, I2, Flow, 0.4, 1, 30, 2, 8, 1.5, 0); //Calculate the optical flow - values are window size, interation size and etc

    for (int i=0;i<I2.rows;i+=10){
        for (int j=0;j<I2.cols;j+=10){
            const Point2f flowatxy = Flow.at<Point2f>(i,j); //Read the flow parameters at ith and jth locations
            line(ImageColor, Point(j, i), Point(cvRound(j + flowatxy.x), cvRound(i + flowatxy.y)), Scalar(0,0,255)); //Draw the vector on the
            line(Blank, Point(j, i), Point(cvRound(j + flowatxy.x), cvRound(i + flowatxy.y)), Scalar(0,0,255)); //Draw the vector on a blank
        }
    }
    
    imshow("Farneback Method - Optical Flow on the Original Image", ImageColor); //Show the image when the optical flows on the original image
    imwrite("results/Farneback Method - Optical Flow on the Original Image.jpg", ImageColor); //Write the image when the optical flows on the original image

    imshow("Farneback Method - Optical Flow on the Blank Image", Blank); //Show the image when the optical flows on a blank image
    imwrite("results/Farneback Method - Optical Flow on the Blank Image.jpg", Blank); //Write the image when the optical flows on a blank image
    waitKey(1000);
    return 0;
}

void SplitVideo(){
    VideoCapture cap("data/video.mp4"); //Read the viode
    Mat Frame; //Define the variable
    int FPS=floor(cap.get(CV_CAP_PROP_FPS)); //Store frame rate per second

    for(int FrameNum = 0; FrameNum < cap.get(CV_CAP_PROP_FRAME_COUNT);FrameNum++){
        cap >> Frame; // get the next frame from video
        if(FrameNum==FPS){ //The First Frame Number
            Frame.copyTo(FirstFrame);
            imshow("The First Frame",FirstFrame);
            imwrite("results/The First Frame.jpg",FirstFrame); //Write the first frame
        }
        if(FrameNum==1.1*FPS){ //The Second Frame Number
            Frame.copyTo(SecondFrame);
            imshow("The Second Frame",SecondFrame);
            imwrite("results/The Second Frame.jpg",SecondFrame); //Write the second frame
        }
    }

}
