#include "DefaultFace.hpp"
#include<iostream>
using namespace std;
DefaultFace::DefaultFace(){
	face_cascade.load("../haarcascade_frontalface_alt.xml"); // load face classifiers
}
void DefaultFace::update(Mat & frame)
{
	detectFace(frame, faces);
	return;
}
int DefaultFace::numFaces()
{
	return faces.size();
}
void DefaultFace::getFace(Mat &disp, Rect & rect)
{
	Mat faceImg=disp.clone();
    faceImg.setTo(Scalar(0, 0, 0));
	Mat display=disp.clone();
	if(faces.size()>0)
	{
		Rect face = faces[0];
		rect=face;
    int width=face.width*1.2;
    int height=face.height*2.7;
		int x=face.x;
		int y=face.y;
		int diff=x+width-disp.cols;
		bool first=false;
		diff=y+height-disp.rows;
			cout<<diff<<endl;
		if(diff>0)
		{
			first=true;
			y-=diff/3;	
			height-=diff*2/3;
		}
		diff=y+height-disp.rows;
			cout<<diff<<endl;
		if(diff>0)
		{
			height-=diff;	
		}
		diff=height/2-y;
			cout<<diff<<endl;
		if(diff>0)
		{
			if(first)
			{
				y=0;
				height=disp.rows-2;
			}
			else{
			y+=(diff/3);	
			height-=diff*2/3;
			diff=height/2-y;
			cout<<diff<<endl;
			if(diff>0)
			{
				height-=diff;	
			}
			}
		}
		first=false;
		diff=x+width-disp.cols;
		if(diff>0)
		{
			first=true;
			x-=diff/3;	
			width-=diff*2/3;
		}
		diff=x+width-disp.cols;
		if(diff>0)
		{
			width-=diff;	
		}
		diff=width/2-x;
		if(diff>0)
		{
			if(first)
			{
				x=0;
				width=disp.cols-2;
			}
			else{
			x+=(diff/3);	
			width-=diff*2/3;
			diff=height/2-x;
			if(diff>0)
			{
				height-=diff;	
			}
			}
		}
		rect=Rect(Point(x, y), Size(width,height));
	}
/*		Point vertices[5];
		Point2f vert[4];
		rrect.points(vert);
		for(int i=0; i<4; i++)
			vertices[i]=vert[i];
		vertices[4]=vert[0];


    const Point *points[1]= {vertices};
    const int lineType=8;
		int npt[] = {4};

		fillPoly(faceImg, points, npt, 1, Scalar(255,255,255), lineType);
		Mat finalImg=faceImg&display;
		return finalImg;
	}
	else
		return faceImg;
*/
}
void DefaultFace::detectFace(Mat& frame, vector<Rect_<int> > &faces) {
	face_cascade.detectMultiScale(frame,faces, 1.15, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
}
