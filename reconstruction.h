#ifndef RECONSTRUCTION_H_
#define RECONSTRUCTION_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<vector>

using namespace cv;
using namespace std;


Mat reconstruction(Mat img,Mat &output_image, int ACH);

#endif /* RECONSTRUCTION_H_ */
