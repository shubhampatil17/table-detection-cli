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

void tableRetention(vector<Point>, Mat, int);
vector<Point> extractIntersectionDataset(Mat);

int findRetentionRowCount(vector<Point>);
int findRetentionColCount(vector<Point>);
//Point findPixelPosition(Point);