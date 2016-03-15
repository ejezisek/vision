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
double i, k, alpha, beta;
double polyTest, radius;
bool ret = false;
char strCount[200];
const String& input_1 = "brian_test.avi";
const String& input_2 = "TestInput2.avi";
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
	namedWindow("Capture Vid", WINDOW_AUTOSIZE);
	namedWindow("Output Vid", WINDOW_AUTOSIZE);
	
	// Read Image
	VideoCapture camFeed(videoName);

	/*/ Set up output
	VideoWriter video_out(out2, camFeed.get(CV_CAP_PROP_FOURCC), 
		camFeed.get(CV_CAP_PROP_FPS), 
		Size(camFeed.get(CV_CAP_PROP_FRAME_WIDTH), camFeed.get(CV_CAP_PROP_FRAME_HEIGHT)));
		*/
	
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
		
		imshow("Capture Vid", frame);

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
		
		// Use weighted frames to show face (testing feature..not really used)
		if (i >= 300)
		{
			if (i == 300)
			{
				faceFrame = frame.clone();
			}
		
			if (i >= 300 + 50)
			{
				k = 50;
			}
			else
			{
				k = i - 300;
			}
		//k = i * 100;
		alpha = ((k-5)/k);
		beta  = (5/k);
		addWeighted(faceFrame, alpha, frame, beta, 0.0, faceFrame);
		}

		/*/ Debug *******************************
		namedWindow("Background", WINDOW_AUTOSIZE);
		imshow("Background", backGround);
		// Debug *******************************/
		
		//compare(faceFrame, backGround, tBall, CMP_GE);
		/*/ Debug *******************************
		namedWindow("face", WINDOW_AUTOSIZE);
		imshow("face", tBall);
		// Debug *******************************/

		cvtColor(frame, frmGray, COLOR_BGR2GRAY);
		cvtColor(backGround, bkgdGray, COLOR_BGR2GRAY);

		absdiff(bkgdGray, frmGray, diffGray);

		// Only run if motion is present
		if (sum(diffGray)[0] < 10000)
		{
			se = getStructuringElement( MORPH_RECT, Size( 2, 6 ), //fill
               Point( -1, -1 ) );
			erode (diffGray, noiseBkgd, se, Point(-1, -1), 2, 1, 1);
			dilate(noiseBkgd, noiseBkgd, se, Point(-1, -1), 2, 1, 1);

			noiseBkgd = diffGray.clone();
		}
		bitwise_xor(diffGray, noiseBkgd, diffGray);
		
		threshold(diffGray, notDiff, 20, 255, THRESH_BINARY);
		
		xorFrame = notDiff.clone();
		if (sum(notDiff)[0] > 300)
		{

		// Track Person
		cntrIn = xorFrame.clone();
		findContours(cntrIn, contrR, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		
		for(int c = 0; c < contrR.size(); c++){
			bounding_face = boundingRect(contrR[c]);
			drawContours( frame, contrR, c, Scalar(0,255,0), 1, 8, hierarchy, 0, Point() );
			polyTest = pointPolygonTest(contrR[c], ctr, false);
		}
		/*/ Debug *******************************
		namedWindow("xorFrame", WINDOW_AUTOSIZE);
		imshow("xorFrame", xorFrame);
		// Debug *******************************/
		}

		//bitwise_not(diffGray, notDiff);
		//bitwise_xor(notDiff, frame, xorFrame);
		
		// Debug *******************************
		namedWindow("notDiff", WINDOW_AUTOSIZE);
		imshow("notDiff", notDiff);
		// Debug *******************************/
		// Debug *******************************
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
