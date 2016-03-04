	
// Declarations for preprocessing functions
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <algorithm>


using namespace cv;
using namespace std;

Mat image_preprocessing(Mat);
Mat pointReduction(Mat);