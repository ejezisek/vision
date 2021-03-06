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
#include "opencv2/objdetect/objdetect.hpp"
//C
#include <stdio.h>
//C++
#include "Points.hpp"
#include <iostream>
#include <sstream>
using namespace cv;
using namespace std;

static void detectFaces(Mat&, vector<Rect_<int> >&, string);
// Global variables
Mat frame; //current frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard; //input from keyboard
void processVideo(char* videoFilename);
Mat MomentMat;
int defaultErosionSize = 5;
int numDilations = 1;
int backgroundFilterAmmount = 100;
int numFramesToRemember = 200;
int waitAmmount = 1;
string face_cascade_path;
bool getNextFrame(int frameNumber)
{
	return frameNumber<30;
}
int main(int argc, char* argv[])
{
	//check for the input parameter correctness
	//create GUI windows
	cout<<"-------------"<<endl<<"Hello world"<<endl;
	cout<<"-------------"<<endl<<argv[1]<<endl;
	namedWindow("Frame");
	namedWindow("FG Mask MOG 2");
	//create Background Subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(numFramesToRemember, backgroundFilterAmmount, false); //MOG2 approach
																								 //pMOG2 = createBackgroundSubtractorMOG2();
																								 //pMOG2 = createBackgroundSubtractorKNN(5000, 16, false); //MOG2 approach
	face_cascade_path = string("haarcascade_frontalface_alt.xml");

	processVideo(argv[1]);
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}
void processVideo(char* videoFilename) {
	cout<<videoFilename<<"Why does this not work?"<<endl;
	//create the capture object
	VideoCapture capture(videoFilename);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	Mat display;
	//read input data. ESC or 'q' for quitting
	while ((char)keyboard != 'q' && (char)keyboard != 27) {
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			break;
		}
		//get the frame number and write it on the current frame
		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		//update the background model
		if (getNextFrame(atoi(frameNumberString.c_str())))
		{
			pMOG2->apply(frame, fgMaskMOG2);
		}
		else
		{
			pMOG2->apply(frame, fgMaskMOG2, 0);
		}
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//find the contours of fgMaskMOG2
		std::vector<std::vector<Point> > oaoa;

		int erosion_size = defaultErosionSize;
/*		Mat element = getStructuringElement(0, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
		erode(fgMaskMOG2, fgMaskMOG2, element);
		for (int i = 0; i<numDilations; i++)
		{
			dilate(fgMaskMOG2, fgMaskMOG2, element);
		}
*/
		int i = 1;

		//show the current frame and the fg masks
		Mat masked = fgMaskMOG2.clone();
		cvtColor(masked, masked, CV_GRAY2BGR, 3);
		Mat blueBackground = ~masked.clone();

		display = frame & masked;
		display = display.clone();
		display.setTo(Scalar(232, 209, 181), ~fgMaskMOG2);

		vector<Rect_<int> > faces;
		detectFaces(frame, faces, face_cascade_path);

		Mat faceImg = frame.clone();
		Mat m=frame.clone();
		if(faces.size()>0)
		{
		Rect face = faces[0];
		faceImg.setTo(Scalar(0, 0, 0));
		int width=face.width;
		int height=face.height;
		ellipse(faceImg, RotatedRect(Point(face.x + (width/2), face.y + (height/2)), Size(width,height), 0),
			Scalar(255, 255, 255), -1, 4);
		ellipse(display, RotatedRect(Point(face.x + (width/2), face.y + (height/2)), Size(width,height), 0),
			Scalar(0, 0, 0), -1, 4);

		m=frame&faceImg;
		Mat disp=m|display;
		imshow("FG Mask MOG 2", disp);
		}

		imshow("Frame", frame);
		moveWindow("Frame", 0, 500);
		//get the input from the keyboard
		keyboard = waitKey(waitAmmount);
	}
	cout << "Writing display!" << endl;
	string f(videoFilename);
	f.append("_display.jpg");
	cout << f << endl;
	imwrite(f, display);
	//delete capture object
	capture.release();
}

static void detectFaces(Mat& img, vector<Rect_<int> >& faces, string cascade_path)
{
	CascadeClassifier face_cascade;
	face_cascade.load(cascade_path);

	face_cascade.detectMultiScale(img, faces, 1.15, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	return;
}
