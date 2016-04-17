#include "Face.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#ifndef DEFAULT_FACE_H
#define DEFAULT_FACE_H
using namespace cv;
using namespace std;
class DefaultFace : public Face
{
	public:
		DefaultFace();
		virtual int numFaces();
		virtual void update(Mat &frame);	
		virtual void getFace(Mat &disp, Rect & rect);

	private:
		void detectFace(Mat& frame, vector<Rect_<int> > &faces) ;
		CascadeClassifier face_cascade, eyes_cascade;
		vector<Rect_<int> > faces;
		
};
	
#endif
