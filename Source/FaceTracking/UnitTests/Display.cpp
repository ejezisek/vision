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
#include <iostream>
#include <sstream>
#include "../DefaultFace.hpp"
using namespace cv;
using namespace std;
// Global variables
void processFrame(Mat & frame);
void processVideo(char* videoFilename);
void processImage(char* imageName);
int backgroundFilterAmmount=50;
int numFramesToRemember=100;
int waitAmmount=0;
Mat frame;
Mat lastProcessed;
void setRectInMask(Mat &mask, Rect &rect)
{
    CV_Assert( !mask.empty() );
    mask.setTo( GC_BGD );
    rect.x = max(0, rect.x);
    rect.y = max(0, rect.y);
    rect.width = min(rect.width, mask.cols-rect.x);
    rect.height = min(rect.height, mask.rows-rect.y);
    (mask(rect)).setTo( Scalar(GC_PR_FGD) );
}
void getBinMask( const Mat& comMask, Mat& binMask )
{
    if( comMask.empty() || comMask.type()!=CV_8UC1 )
        CV_Error( Error::StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
    if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols )
        binMask.create( comMask.size(), CV_8UC1 );
    binMask = comMask & 1;
}
void getGrab(Mat &source, Mat &dest, Rect& rect)
{
  Mat bgM, fgModel;
	dest.create(source.size(), CV_8UC1);
	if(!dest.empty())
	dest.setTo(Scalar::all(GC_BGD));
	rect.width=source.cols-5;
	rect.x=5;
	rect.height=source.rows-10;
	rect.y=10;
	setRectInMask(dest, rect);
	cout<<"THE RECT IS: "<<rect<<endl;
  grabCut(source, dest, rect, bgM, fgModel, 5);
	getBinMask(dest, dest);
}

void getImages(Mat & source, Mat &dst, Rect& r)
{

				Mat cropped=source.clone();
				Rect rect(r);

				Mat mask;
				getGrab(cropped, mask, rect);
				Mat face=mask.clone();
				Mat facecropped=cropped(rect);				

/*				rect.height/=3;
				getGrab(cropped, mask, rect);
				Mat hair=mask.clone();
				Mat haircropped=cropped(rect);				

				rect.y+=2*rect.height;
				getGrab(cropped, mask, rect);
				Mat shirt=mask.clone();
				//cropped.copyTo(shirt, mask);
*/
				Mat totalMask=face;	
				cropped.copyTo(dst, totalMask);
				Mat white=cropped.clone();
				white.setTo(Scalar(255, 255, 255));
				Mat black=cropped.clone();
				black.setTo(Scalar(0, 0, 0));

				white.copyTo(black, totalMask);
				black=~black;
        imshow("Black", black);

				Mat blue=cropped.clone();

				blue.setTo(Scalar(232, 209, 181));
				blue=black&blue;
        imshow("Blue", blue);
				dst|=blue;
		//		Mat shirtcropped=cropped(rect);				

		//		dst=face|shirt|hair;
}
void colorReduce(cv::Mat& image, int div=256)
{    
    int nl = image.rows;                    // number of lines
    int nc = image.cols * image.channels(); // number of elements per line

    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);

        for (int i = 0; i < nc; i++)
        {
            // process each pixel
            data[i] = data[i] / div * div + div / 2;
        }
    }
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
      const int lineType=1;
      int npt[]={(int)p.size()};
      fillPoly(dest, points, npt, 1, CV_RGB(255*abs(sin(i*i*.0174533)),255*abs(cos(i*i*i*.0174533)),i), lineType);
    }

}
Face *f;
int main(int argc, char* argv[])
{
    //create GUI windows
    namedWindow("Frame");
		f=new DefaultFace();
		try{
    processImage(argv[1]);
		}
		catch(...)
		{
			cout<<"Image processing failed.  Attempting video."<<endl;
			processVideo(argv[1]);
		}
    //destroy GUI windows
		string wrt(argv[1]);
		wrt+="_display.jpg";
		imwrite(wrt, lastProcessed);
    destroyAllWindows();
    return EXIT_SUCCESS;
}
void processFrame(Mat & frame)
{
	cout<<frame.size();
				f->update(frame);
				f->update(frame);
				Rect rect;
				//if(f->numFaces()!=0)
				{
				f->getFace(frame, rect);
				Mat res;
				getImages(frame, res, rect);
//				frame.copyTo(res, grabCut_m);
				//drawContours(newFace,newFace);
				//colorReduce(newFace, 64);
				Mat cropped=frame(rect);				
        //get the frame number and write it on the current frame
				//if(cropped.rows>0 && cropped.cols>0)
				{
				cout<<rect<<endl;
				cout<<frame.size()<<endl;
				waitKey( waitAmmount );
        //imshow("Frame", cropped);
					//if(res.rows>0 && res.cols>0)
					{
					cout<<"SHOWING GRABCUT";
       			imshow("Grab Cut", res);
						lastProcessed=res;
					}
				}
				moveWindow("Frame", 0, 500);
        //get the input from the keyboard
				//if(f->numFaces()!=0)
				}
        imshow("Frame", frame);
}
void processImage(char* imageName)
{
		Mat image;
		String s("../../Testing/Input/");
		s+=imageName;
    image = imread( s );
		processFrame(image);
				waitKey( 0 );
}
void processVideo(char* videoFilename) {
    //create the capture object
		String s("../../Testing/Input/");
		s+=videoFilename;
    VideoCapture capture(s);
    if(!capture.isOpened()){
    }
    //read input data. ESC or 'q' for quitting
    while( true ){
        //read the current frame
        if(!capture.read(frame)) {
						break;
        }
				processFrame(frame);
    }
    //capture.release();
}
