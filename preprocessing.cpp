//Definition of preprocessing functions

#include "preprocessing.h"



Mat image_preprocessing(Mat img){

	Mat binimg(img.rows,img.cols,img.type());

	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++)
			if((int)img.at<uchar>(i,j)>125 && (int)img.at<uchar>(i,j)<200)
				img.at<uchar>(i,j)=img.at<uchar>(i,j)-50;

	// Binarization using Otsu Method
	threshold(img,binimg,0,255,THRESH_BINARY_INV|THRESH_OTSU);

	return binimg;
}



Mat pointReduction(Mat img){

	for(int i=1;i<img.rows-1;i++){
		for(int j=1;j<img.cols-1;j++){

			int neighbours[8];

			neighbours[0]  = (int)img.at<uchar>(i,j-1); //top
			neighbours[1] = (int)img.at<uchar>(i,j+1); //bottom
			neighbours[2] = (int)img.at<uchar>(i-1,j); //left
			neighbours[3] = (int)img.at<uchar>(i+1,j); //right
			neighbours[4] = (int)img.at<uchar>(i-1,j-1); //topLeft
			neighbours[5] = (int)img.at<uchar>(i+1,j-1); //topRight
			neighbours[6] = (int)img.at<uchar>(i-1,j+1); //bottomLeft
			neighbours[7] = (int)img.at<uchar>(i+1,j+1); //bottomRight


			int maximum = neighbours[0];

			for(int i=1;i<8;i++){
				maximum = max(maximum,neighbours[i]);
			}

			if(maximum != 0){
				img.at<uchar>(i,j)=0;
			}
		}
	}

	return img;
}