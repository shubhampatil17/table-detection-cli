#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <streambuf>
#include <stdio.h>
#include <map>

using namespace std;
using namespace cv;

vector<Mat> cellExtraction(vector<Point>, Mat, int, int, String);

vector<vector<Mat>> multiTablePageSegmentation(Mat,Mat,Mat, String);

void tableRetention(vector<vector<Mat>>, int, String);

void generateXML(vector<vector<Mat>>);

vector<Point> extractIntersectionDataset(Mat);

int findRetentionRowCount(vector<Point>);
int findRetentionColCount(vector<Point>);