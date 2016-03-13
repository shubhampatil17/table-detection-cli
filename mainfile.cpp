#include <iostream>
#include <fstream>
#include "preprocessing.h"
#include "lineprocessing.h"
#include "ach.h"
#include "connect_broken.h"
#include "intersection.h"
#include "reconstruction.h"
#include "reconstructSinglePts.h"
#include "retention.h"
#include <sys/stat.h>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>


using namespace cv;
using namespace std;

void processImage(Mat, Mat, String);

int main(int argc, char* argv[]){

	// Image reading
	string path; 

	path="/home/shubham/dataset/10.1.1.1.2035_4.bmp";
	//path = argv[1];

	vector<String> filenames;

	String folder = "/home/shubham/DatasetAndTesting/TestData";

	glob(folder,filenames);


	for(int i=0;i<filenames.size();i++){
		Mat clrImg = imread(filenames[i], IMREAD_COLOR);
		Mat ipImg = imread(filenames[i], IMREAD_GRAYSCALE);

		if(!ipImg.data){
			cout<<"Error in reading file, skipping : "<<filenames[i]<<"\n";
		}else{
			//imshow("Image", clrImg);
			imshow("Color Image :" + filenames[i],clrImg);
			processImage(ipImg, clrImg, filenames[i]);		
		}
	}


	//Mat clrImg = imread(path, IMREAD_COLOR);
	//Mat ipimg = imread(path, IMREAD_GRAYSCALE);
	/*if(!ipimg.data){
		cout<<"Error...";
		return 0;
	}*/

	//processImage(ipImg, clrImg, path);

	waitKey();

	return 0;
}


void processImage(Mat grayImg, Mat clrImg, String filename){
	//================================================================================

	// Preprocessing of image

	Mat binImg= image_preprocessing(grayImg);

	int avgCH = calculateACH(binImg);
	
	//function to join broken components


	Mat connectedImg = connect_broken_lines(binImg,avgCH);

	Mat horizProcessedImg = horizontalProcessing(connectedImg, 2*avgCH);
	Mat vertiProcessedImg = verticalProcessing(connectedImg, 2*avgCH);


	horizProcessedImg = horizontalProcessing(horizProcessedImg, 7*avgCH);
	vertiProcessedImg = verticalProcessing(vertiProcessedImg, 7*avgCH);
	
	//Merge horizProcessedImg and vertiProcessedImg

	Mat mergedImg = horizProcessedImg + vertiProcessedImg;

	//===================================================================================

	// Function to find intersection points and reconstruct tables

	Mat intersectPts = intersection(mergedImg,avgCH);

	Mat nonIntersectPts = findPts(mergedImg,avgCH);

	Mat reconstructImg = Mat :: zeros(binImg.rows,binImg.cols,binImg.type());

	Mat dataPoints = reconstruction(intersectPts + nonIntersectPts, reconstructImg, avgCH);

	Mat pointReducedImg = dataPoints.clone();

	pointReducedImg = pointReduction(pointReducedImg);


	vector<vector<Mat>> segmentationData = multiTablePageSegmentation(reconstructImg.clone(), pointReducedImg, clrImg, filename);

	tableRetention(segmentationData, avgCH, filename);

	//imshow("preprocessed Image",binImg);
	//imshow("Connected Image", connectedImg);
	//imshow("Horizontal Lines", horizProcessedImg);
	//imshow("Vertical Lines", vertiProcessedImg);
	//imshow("Merged Image", mergedImg);
	//imshow("Intersection Points", intersectPts);
	//imshow("Non Intersection Points", nonIntersectPts);
	//imshow("Data Set", dataPoints);
	//imshow("Point Reduced Image :"+filename, pointReducedImg);
	imshow("Reconstructed Image :"+filename, reconstructImg);
}