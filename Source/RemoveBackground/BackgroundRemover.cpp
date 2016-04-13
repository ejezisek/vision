#include <stdexcept>
#include<string>
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
//C
#include <stdio.h>
//C++
#include "./Points.hpp"
#include <iostream>
#include <sstream>
using namespace cv;
using namespace std;
// Global variables
Mat frame; //current frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard; //input from keyboard
void processVideo(char* videoFilename);
Mat MomentMat;
int defaultErosionSize=5;
int numDilations=1;
int backgroundFilterAmmount=1000;
int numFramesToRemember=100;
int waitAmmount=1;
bool getNextFrame(int frameNumber)
{
	return frameNumber<30;
}
void drawContours(Mat &source, Mat& dest)
{
		Mat source_gray;
		cvtColor(source, source_gray, CV_BGR2GRAY);
		const int max_BINARY_value=255;
		adaptiveThreshold(source_gray, source_gray, max_BINARY_value, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
	    std::vector<std::vector<Point> > oaoa;
	    findContours(source_gray, oaoa, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
		int i=0;
		for( std::vector<std::vector<Point> >::iterator iterator=oaoa.begin(); iterator!=oaoa.end(); iterator++, i++)
    {
			std::vector<Point> p=*iterator;
			const Point *points[1]= {&p[0]};
			const int lineType=8;
			int npt[]={(int)p.size()};
			fillPoly(dest, points, npt, 1, CV_RGB(255*abs(sin(i*i*.0174533)),255*abs(cos(i*i*i*.0174533)),i), lineType);	
		}
	
}
int main(int argc, char* argv[])
{
    //check for the input parameter correctness
    //create GUI windows
    namedWindow("Frame");
    namedWindow("FG Mask MOG 2");
    //create Background Subtractor objects
    pMOG2 = createBackgroundSubtractorMOG2(numFramesToRemember, backgroundFilterAmmount, false); //MOG2 approach
    //pMOG2 = createBackgroundSubtractorMOG2();
    //pMOG2 = createBackgroundSubtractorKNN(5000, 16, false); //MOG2 approach
    processVideo(argv[1]);
    //destroy GUI windows
    destroyAllWindows();
    return EXIT_SUCCESS;
}
void processVideo(char* videoFilename) {
    //create the capture object
		String s("../Testing/Input/");
		s+=videoFilename;
    VideoCapture capture(s);
    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file: " << videoFilename << endl;
        exit(EXIT_FAILURE);
    }
				Mat display;
    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        //read the current frame
        if(!capture.read(frame)) {
						break;
        }
        //get the frame number and write it on the current frame
        stringstream ss;
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        //update the background model
				if(getNextFrame(atoi(frameNumberString.c_str())))
				{
        	pMOG2->apply(frame, fgMaskMOG2);
				}
				else
				{ 
        	pMOG2->apply(frame, fgMaskMOG2, 0);
				}
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
				//find the contours of fgMaskMOG2
				std::vector<std::vector<Point> > oaoa;
				
				int i=1;
        //show the current frame and the fg masks
				Mat masked=fgMaskMOG2.clone();
				cvtColor(masked, masked, CV_GRAY2BGR, 3);
				Mat blueBackground=~masked.clone();
				display=frame & masked;
				display=display.clone();
				display.setTo(Scalar(255, 0, 0), ~fgMaskMOG2);
				//Mat temp=frame.clone();
				//temp.convertTo(temp, CV_32S);
				//Mat labels(temp.size(), CV_32S);
				//connectedComponents(temp, labels, 8, CV_32S);
//				Mat frameConts=frame.clone();
//				drawContours(frame, frameConts);
//				imshow("Contours", frameConts);
//				drawContours(display, display);
        imshow("FG Mask MOG 2", display);
		const int max_BINARY_value=255;
			Mat bw;
		cvtColor(frame, bw, CV_BGR2GRAY);
//		adaptiveThreshold(bw, bw, max_BINARY_value, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
        imshow("Frame", frame);
//        imshow("Black and White", bw);
				moveWindow("Frame", 0, 500);
        //get the input from the keyboard
        keyboard = waitKey( waitAmmount );
    }
		cout<<"Writing display!"<<endl;
		string f(videoFilename);
		f.append("_display.jpg");
		cout<<f<<endl;
		imwrite(f, display);
    //delete capture object
    capture.release();
}
