/*
 * reconstructSinglePts.h
 *
 *  Created on: 09-Jan-2016
 *      Author: vranka
 */

#ifndef RECONSTRUCTSINGLEPTS_H_
#define RECONSTRUCTSINGLEPTS_H_
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;

Mat findPts(Mat img,int ACH);

Mat hrpattern(int leftORight,int ACH);

Mat vtpattern(int topObottom,int ACH);

Mat h(Mat img, int ACH);

Mat t(Mat img, int ACH);



#endif /* RECONSTRUCTSINGLEPTS_H_ */
