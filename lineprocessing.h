// Declarations of functions for extraction of horizontal and vertical lines

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat horizontalProcessing(Mat,int); // to extract horizontal lines

Mat verticalProcessing(Mat,int); // to extract vertical lines