#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
class Face
{
	public:
		virtual int numFaces()=0;
		virtual void update(Mat &frame)=0;
    virtual void getFace(Mat &disp, Rect & rect)=0;

		virtual ~Face(){}
};
