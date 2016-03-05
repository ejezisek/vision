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
#include <string>

#include "opencv2/core.hpp"
using namespace cv;
using namespace std;
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

string toString(int val)
{
	std::ostringstream Stream;
	Stream<<val;
	return Stream.str();
}
Point getMax(std::vector<Point> a)
{
  int yMAX=1;
  int xMAX=1;
  for(std::vector<Point>::iterator pointItem=a.begin(); pointItem!=a.end(); pointItem++)
  {

      Point point=*pointItem;
      if(point.y>yMAX)
    yMAX=point.y;
      if(point.x>xMAX)
    xMAX=point.x;
  }
  return Point(xMAX, yMAX);
}
Point getMin(std::vector<Point> a)
{
  int yMIN=100000;
  int xMIN=100000;
  for(std::vector<Point>::iterator pointItem=a.begin(); pointItem!=a.end(); pointItem++)
  {
      Point point=*pointItem;
      if(point.y<yMIN)
    yMIN=point.y;
      if(point.x<xMIN)
    xMIN=point.x;
  }
  if(yMIN==100000)
    yMIN=0;
  if(xMIN==100000)
    xMIN=0;

  return Point(xMIN, yMIN);
}

