
#ifndef INSTERSECTION_H_
#define INSTERSECTION_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat hitmiss(Mat src, Mat kernel);	//perform hit and miss tranfrom

Mat intersection(Mat img,int ACH);				//gives intersection points;

Mat pattern(int top_bottom,int left_right,int ACH);			//function to create kernel matrix;




#endif /* INSTERSECTION_H_ */
