#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;

void getGrab(Mat &source, Mat &dest, Rect rect)
{
	Mat bgM, fgModel;
	grabCut(source, dest, rect, bgM, fgModel, 3);
}
