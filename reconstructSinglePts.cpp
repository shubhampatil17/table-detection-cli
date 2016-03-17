/*
 * reconstructSinglePts.cpp
 *
 *  Created on: 09-Jan-2016
 *      Author: vranka
 */
#include <iostream>
#include "intersection.h"
#include "reconstructSinglePts.h"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

void display(Mat img);

Mat findPts(Mat img, int ACH){
	Mat new_image = img.clone();	//copy image data



	Mat left= hitmiss(new_image, hrpattern(1,ACH));

	left=left*200;




	Mat right= hitmiss(new_image, hrpattern(0,ACH));

	right=right*210;





	Mat top= hitmiss(new_image, vtpattern(1,ACH));
	top=top*220;



	Mat bottom= hitmiss(new_image, vtpattern(0,ACH));
	bottom=bottom*230;

	//-----------------------------------------------------------------
	//align points in same line
	//if possible check if image is non empty

	Mat tb=Mat :: zeros(img.rows,img.cols,img.type());


	Mat tb_aligned = t(top+bottom, ACH);

	Mat lr=Mat :: zeros(img.rows,img.cols,img.type());


	Mat lr_aligned = h(left+right , ACH);


	Mat output_image=Mat :: zeros(img.rows,img.cols,img.type());

	//imshow("tb_aligned", tb_aligned);

	//imshow("lr_aligned", lr_aligned);



	output_image = tb_aligned + lr_aligned;

	//display(output_image);

	//imshow("modified",output_image);


	//output_image = left + right + top + bottom;



	imshow("NOn itersecting",output_image);


	return output_image;
}


//----------------------------------------------------------------------
//function to create kernel matrix for horizontal non itersecting point


Mat hrpattern(int leftORight,int ACH){

	Mat kernel_mat = Mat::zeros(ACH+2,3*ACH, CV_32S);

	//for(int k=0;k<3*ACH;k++)						//cols

		for(int i=0;i<3*ACH ; i++){		//cols

			kernel_mat.at<int>(0,i) = -1;
			kernel_mat.at<int>(ACH+1,i) = -1;

		}

		if( leftORight){

			for(int i=0;i<ACH ; i++){

				kernel_mat.at<int>(ACH/2+1,i) = -1;
			}


			for(int i=ACH;i<3*ACH ; i++){
				kernel_mat.at<int>(ACH/2+1,i) = 1;

			}

		}

		else{

			for(int i=0;i<2*ACH ; i++){

				kernel_mat.at<int>(ACH/2+1,i) = 1;
			}

			for(int i=2*ACH;i<3*ACH ; i++){

				kernel_mat.at<int>(ACH/2+1,i) = -1;
			}


		}


	return kernel_mat;
}



//----------------------------------------------------------------------
//function to create kernel matrix for vertical non itersecting point

Mat vtpattern(int topObottom,int ACH){

	Mat kernel_mat = Mat::zeros(3*ACH,ACH+2, CV_32S);

	for(int k=0;k<3*ACH;k++)

	//for(int i=0,j=2*ACH;i<ACH ; i++,j++)
	{

		kernel_mat.at<int>(k,0) = -1;
		kernel_mat.at<int>(k,ACH+1) = -1;

	}

	if( topObottom){
		//line ending on top

		for(int i=0;i<3*ACH/2 ; i++){

					kernel_mat.at<int>(i,ACH/2+1) = -1;
				}

		for(int i=3*ACH/2;i<3*ACH ; i++){
					kernel_mat.at<int>(i,ACH/2+1) = 1;

				}

	}
	else{

		for(int i=0;i<3*ACH/2 ; i++){
			kernel_mat.at<int>(i,ACH/2+1) = 1;
		}

		for(int i=3*ACH/2;i<3*ACH ; i++){
					kernel_mat.at<int>(i,ACH/2+1) = -1;

		}

		//line ending on bottom
	}

	return kernel_mat;
}



//------------------------------------------------------------------
// Function to set average x co-ordinate

Mat h(Mat lr, int ACH){

	Mat img=lr.clone();


	for(int i=0; i<img.cols; i=i+(ACH/2)){

		vector <Point> p;
		int avgx[ACH];

		for(int m=0; m<ACH; m++){

			avgx[m]=0;	

		}


		for(int j=i; j<i+ACH && j<img.cols; j++){

			for(int k=0; k<img.rows; k++){

				if(img.at<uchar>(k,j)!=0){

					p.push_back(Point(j,k));

					avgx[j%ACH]++;

				}
			}
		}

		

		int max=0;
		int x=-1;

		for(int m=0; m<ACH; m++){

			

			if(avgx[m]>max){

				max = avgx[m];

				x=m;

			}
		}

		int X;

		if(x!=-1){


			int temp = i%ACH;

			if (x < temp) 
			{
				/* code */
				X= x + (ACH-temp)+i;

			}

			else 

				X = x-temp+i;


			for(int j=0; j< p.size(); j++){


				if(p[j].x!=X){



					img.at<uchar>(p[j].y,X) = img.at<uchar>(p[j]);

					img.at<uchar>(p[j]) = 0;



				}


			}

		}

	}


	return img;

}




//------------------------------------------------------------------
// Function to set average y co-ordinate



//------------------------------------------------------------------
// Function to set average y co-ordinate

Mat t(Mat tb, int ACH){

	Mat img=tb.clone();

	for(int i=0; i<img.rows; i=i+(ACH/2)){

		vector <Point> p;
		int avgy[ACH];

		for(int m=0; m<ACH; m++){

			avgy[m]=0;

		}


		for(int j=i; j<i+ACH && j<img.rows; j++){

			for(int k=0; k<img.cols; k++){

				if(img.at<uchar>(j,k)!=0){


					p.push_back(Point(k,j));

					avgy[j%ACH]++;



				}
			}
		}



		int max=0;
		int y=-1;

		for(int m=0; m<ACH; m++){


			if(avgy[m]>max){

				max = avgy[m];

				y=m;

			}
		}


		int Y;

		if(y!=-1){


			int temp = i%ACH;

			if (y < temp)
			{

				Y= y + (ACH-temp)+i;

			}

			else

				Y = y-temp+i;



			for(int j=0; j< p.size(); j++){


				if(p[j].y!=Y){

					//cout<<p[j].x<<"\t"<<p[j].y<<"\t"<<Y<<endl;

					img.at<uchar>(Y,p[j].x) = img.at<uchar>(p[j]);

					img.at<uchar>(p[j]) = 0;



				}


			}


		}



	}

	return img;

}



void display(Mat img){

	cout<<"---------------------------"<<endl;
		for( int i = 0; i < img.rows-1; i++ ){

		for( int j = 0; j < img.cols-1; j++ ){

			if(img.at<uchar>(i,j)!=0){

					int s=(int)img.at<uchar>(i,j);	  	  	
		  	}
		}
	}
}


				

				









