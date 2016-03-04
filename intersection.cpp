#include "intersection.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * Hit-or-miss transform function
 *
 * Parameters:
 *   src     The source image, 8 bit single-channel matrix
 *   dst     The destination image
 *   kernel  The kernel matrix. 1=foreground, -1=background, 0=don't care
 */

Mat hitmiss(Mat src, Mat kernel){

	CV_Assert(src.type() == CV_8U && src.channels() == 1);

	Mat kernel_foregnd_pix = (kernel == 1) / 255;
	Mat kernel_backgnd_pix = (kernel == -1) / 255;
	
	normalize(src, src, 0, 1, cv::NORM_MINMAX);
	
	Mat match_mat, ignore_mat;

	erode(src, match_mat, kernel_foregnd_pix, Point(-1,-1), 1, BORDER_CONSTANT, Scalar(0));		//matches foreground pixels

	erode(1-src, ignore_mat, kernel_backgnd_pix, Point(-1,-1), 1, BORDER_CONSTANT, Scalar(0));		//matches background pixels

	return (match_mat & ignore_mat);
}


//-----------------------------------------------------------------
//Function gives kernel matrix for hit and miss transform

Mat pattern(int top_bottom, int left_right, int ACH){

	Mat kernel_mat = Mat::zeros(3*ACH, 3*ACH, CV_32S);
	
	if(top_bottom == 1){
	//---------------Bottom
		for(int m=0; m <= 3*ACH/2; m++)
					kernel_mat.at<int>(m, 3*ACH/2) = 1;
	}

	else{
	//--------------Top
		for(int m=3*ACH/2 ; m<3*ACH; m++)
					kernel_mat.at<int>(m,3*ACH/2) = 1;
		
	}

	if(left_right == 1){
	//--------------Right
		for(int m=0 ; m<=3*ACH/2; m++)
					kernel_mat.at<int>(3*ACH/2, m) = 1;

	}
	else{
	//---------------Left
		for(int m=3*ACH/2; m<3*ACH; m++)
					kernel_mat.at<int>(3*ACH/2,m) = 1;
		
	}

	kernel_mat.at<int>(0,0) = -1;
	kernel_mat.at<int>(3*ACH - 1,0) = -1;
	kernel_mat.at<int>(0,3*ACH - 1) = -1;
	kernel_mat.at<int>(3*ACH -1,3*ACH -1) = -1;

	return kernel_mat;
}


//-------------------------------------------------------------------
//function to find intersection points

Mat intersection(Mat img, int ACH){

	Mat new_image = img.clone();	//copy image data	

	
	Mat top_left= hitmiss(new_image, pattern(0,0,ACH));
	top_left=top_left*10;
	Mat bottom_left= hitmiss(new_image, pattern(1,1,ACH));
	bottom_left=bottom_left*70;
	Mat top_right= hitmiss(new_image, pattern(0,1,ACH));
	top_right=top_right*20;
	Mat bottom_right= hitmiss(new_image, pattern(1,0,ACH));
	bottom_right=bottom_right*40;

	Mat output_image=Mat :: zeros(img.rows,img.cols,img.type());

	output_image = top_left+bottom_left+top_right+bottom_right;
	//output_image = (output_image==0);
	//output_image = output_image*255;
	//imshow("Merged Image", output_image);

	return output_image;	
}

