/***********************************
* 
* CPRE575 Project
* March XX, 2016	
************************************/

// OpenCV
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

// C libraries
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

// Matrix for ImgPrc
Mat ballGray;
Mat cntrIn;
Mat frame;
Mat se, display, backGround;
Mat frmGray, bkgdGray, diffGray;
Mat notDiff, xorFrame;
Mat faceFrame, noiseBkgd;

// Var Def
int largestContr = 0;
double i, k, alpha, beta;
double polyTest, radius, perOn;
bool ret = false;
char strCount[200];
const String& input_1 = "brian_test.avi";
const String& input_2 = "brian_test.mp4";
const String& input_3 = "Brian_test2.mp4";
const String& out1 = "output1.mp4";
const String& out2 = "output2.mp4";
const String& out3 = "output3.mp4";

Rect bounding_face;
Point2f ctr;
vector<vector<Point> > contr, contrR, contrG, contrB;
vector<Vec4i> hierarchy;
vector<Point2f>center;

int main(int argc, char *argv[])
{	
	// Windows
	String videoName="../Testing/Input/";
	videoName+=argv[1];
	//namedWindow("Capture Vid", WINDOW_AUTOSIZE);
	namedWindow("Output Vid", WINDOW_AUTOSIZE);
	
	// Read Image
	VideoCapture camFeed(videoName);
	
   if (!camFeed.isOpened())  // if not success, exit program
   {
   	cout << "Cannot open the video file" << endl;
   	return -1;
   }

	// Capture frame-by-frame
    for(i=1;;i++){
		ret = camFeed.read(frame);
		
		if (!ret) //if not success, break loop
		{
     		cout << "Cannot read the frame from video file" << endl;
     		break;
		}
		
		//imshow("Capture Vid", frame);

		// Set first frame
		if (backGround.empty()){
			backGround = frame.clone();
			faceFrame = frame.clone();
		}

		// Use weighted frames to maintain background
		if (i >= 100)
		{
			k = 100;
		}
		else
		{
			k = i;
		}
		k = i * 100;
		alpha = ((k-1)/k);
		beta  = (1/k);
		addWeighted(backGround, alpha, frame, beta, 0.0, backGround);

		cvtColor(frame, frmGray, COLOR_BGR2GRAY);
		cvtColor(backGround, bkgdGray, COLOR_BGR2GRAY);

		se = getStructuringElement( MORPH_RECT, Size( 1, 4 ), //remove bkgd noise
				Point( -1, -1 ) );
		erode (bkgdGray, bkgdGray, se, Point(-1, -1), 1, 1, 1);
		dilate(bkgdGray, bkgdGray, se, Point(-1, -1), 1, 1, 1);

		absdiff(bkgdGray, frmGray, diffGray);
		
		threshold(diffGray, notDiff, 20, 255, THRESH_BINARY);

		// Only run if motion is present (ie > 15% of frame)
		perOn = (double)(countNonZero(notDiff)) / (double)((notDiff.cols * notDiff.rows));
		if (perOn > 0.14)
		{
			erode (diffGray, diffGray, se, Point(-1, -1), 1, 1, 1);
			dilate(diffGray, diffGray, se, Point(-1, -1), 3, 1, 1);

			se = getStructuringElement( MORPH_RECT, Size( 4, 1 ), //remove bkgd noise
				   Point( -1, -1 ) );
			erode (diffGray, diffGray, se, Point(-1, -1), 1, 1, 1);
			dilate(diffGray, diffGray, se, Point(-1, -1), 3, 1, 1);

			//bitwise_xor(diffGray, noiseBkgd, diffGray);
		
			threshold(diffGray, notDiff, 20, 255, THRESH_BINARY);
		
			xorFrame = notDiff.clone();

			// Track Person
			cntrIn = xorFrame.clone();
			findContours(cntrIn, contrR, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		
			for(int c = 0; c < contrR.size(); c++)
			{
				if(contrR[c].size() >= largestContr)
				{
					bounding_face = boundingRect(contrR[c]);
					//drawContours( frame, contrR, c, Scalar(0,255,0), 1, 8, hierarchy, 0, Point() );
					//rectangle(frame, bounding_face, Scalar(0,255,0),2, 8,0);
					//polyTest = pointPolygonTest(contrR[c], ctr, false);
					largestContr = contrR[c].size();
				}
			}
			rectangle(frame, bounding_face, Scalar(0,255,0),2, 8,0);
			largestContr = 0;

		/*/ Debug *******************************
		namedWindow("xorFrame", WINDOW_AUTOSIZE);
		imshow("xorFrame", xorFrame);
		// Debug *******************************/
		}
		
		/*/ Debug *******************************
		namedWindow("notDiff", WINDOW_AUTOSIZE);
		imshow("notDiff", notDiff);
		// Debug *******************************/
		/*/ Debug *******************************
		namedWindow("No Bkgd", WINDOW_AUTOSIZE);
		imshow("No Bkgd", diffGray);
		// Debug *******************************/
		
		sprintf(strCount, "%f", i);
		putText(frame, strCount, Point2f(10, 40), CV_FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);
		imshow("Output Vid", frame);

		//video_out.write(frame);
		
        if(waitKey(30) >= 0) break;
	}
	
    camFeed.release();
    //destroyAllWindows();
	return 0;
}
