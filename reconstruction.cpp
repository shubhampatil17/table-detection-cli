#include "reconstruction.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>    // std::sort
#include <map>

using namespace std;
using namespace cv;
// This defines a binary predicate that,
// taking two values of the same type of those
// contained in the list, returns true if the first
// argument goes before the second argument

struct comparison_struct {

    bool operator() (Point p, Point q) {
    	if(p.x == q.x)
    		return p.y < q.y;
    	return (p.x < q.x);
	}

} comparison_object;

struct comparison_xstruct {

    bool operator() (Point p, Point q) {
    	if(p.y == q.y)
    		return p.x < q.x;
    	return (p.y < q.y);
	}

} comparison_x;






Mat reconstruction(Mat img,Mat &output, int ACH){

	map<int, vector<Point> > pointsx,pointsy;

	for( int i = 0; i < img.rows-1; i++ ){

		for( int j = 0; j < img.cols-1; j++ ){

			if(img.at<uchar>(i,j)!=0){

					//int s=(int)img.at<uchar>(y,x);
					//cout<<s<<endl;
				Point P(j,i);

				pointsy[i].push_back(P);
				pointsx[j].push_back(P);
	  	  	
		  	}
		}
	}

	Mat output_image=Mat :: zeros(img.rows,img.cols,img.type());

	//=================================================
	//Draw horizontal lines

	vector <Point> xpoints;

	Mat Dataset = Mat :: zeros(img.rows,img.cols,img.type());



	for (map<int, vector <Point> >::iterator it=pointsx.begin(); it!=pointsx.end(); ++it)
	{

		int count = 0 ,i=1;
		

		while( i<it->second.size() && count<3){



			if(abs(it->second[i].y - it->second[i-1].y)>ACH)


				count ++;

			i++;


		}

		

		if(count>=2){

			xpoints.insert(xpoints.end(), it->second.begin(), it->second.end());
		}
	}

	sort(xpoints.begin(), xpoints.end(), comparison_x);





	for(int i=1; i< xpoints.size(); i++){
		int s=(int)img.at<uchar>(xpoints[i-1]);

		//=========================================================
		//Remove squares

		if(s==70 && (int)img.at<uchar>(xpoints[i])==40){


			continue;
		}


		else if(s==10 && (int)img.at<uchar>(xpoints[i])==20){

			continue;
		}



		else if(s==200 && (int)img.at<uchar>(xpoints[i])==210){


			continue;
		}



	    if(s!=20 && s!=40 && s!=60 && s!=210 && s!=220 && s!=230 && xpoints[i-1].y==xpoints[i].y){

	    	line( output_image,xpoints[i-1],xpoints[i],Scalar(255,255,255), 1, 8 , 0);

			Dataset.at<uchar>(xpoints[i])=255;
			Dataset.at<uchar>(xpoints[i-1])=255;

	    }
	
	}




	//===================================================================================================================
	//Draw Vertical lines


	vector <Point> ypoints;
	for (map<int, vector <Point> >::iterator it=pointsy.begin(); it!=pointsy.end(); ++it)
	{
		
		int count = 0 ,i=1;

		

		while( i<it->second.size() && count<3){

			if(abs(it->second[i].x - it->second[i-1].x)>ACH)

				count ++;

			i++;


		}

		

		if(count>=2){
			ypoints.insert(ypoints.end(), it->second.begin(), it->second.end());

		}
	}
	sort(ypoints.begin(), ypoints.end(), comparison_object);



	for(int i=1; i< ypoints.size(); i++){


		int s=(int)img.at<uchar>(ypoints[i-1]);

		//=========================================================
		//Remove squares


		if(s==10 && (int)img.at<uchar>(ypoints[i])==70){


			continue;

		}


		else if(s==20 && (int)img.at<uchar>(ypoints[i])==40){


			continue;

		}

		else if(s==220 && (int)img.at<uchar>(xpoints[i])==230){

			continue;
		}

		//================================================================================================
		//Draw lines
		//cout<<s<<endl;
		if(s!=110 && s!=70 && s!=40 && s!=230 && s!=200 && s!=210 && ypoints[i-1].x==ypoints[i].x){
			line( output_image,ypoints[i-1],ypoints[i],Scalar(255,0,0), 1, 8 , 0);

			Dataset.at<uchar>(ypoints[i])=255;
			Dataset.at<uchar>(ypoints[i-1])=255;

		}
	}




	output_image=(output_image==0);
	output_image=output_image*255;
	output=output_image;


	return Dataset;

}
