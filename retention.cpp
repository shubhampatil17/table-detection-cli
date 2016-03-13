#include "retention.h"
using namespace std;

struct comparison_struct {

    bool operator() (Point p, Point q) {
    	if(p.x == q.x)
    		return p.y < q.y;
    	return (p.x < q.x);
	}

} sort_y;

struct comparison_xstruct {

    bool operator() (Point p, Point q) {
    	if(p.y == q.y)
    		return p.x < q.x;
    	return (p.y < q.y);
	}

} sort_x;




int findRetentionRowCount(vector<Point> dataPoints){
	sort(dataPoints.begin(), dataPoints.end(), sort_x);

	//cout<<dataPoints<<"\n\n";


	int rows=0;

	for(int i=1;i< dataPoints.size();i++){
		if( abs(dataPoints[i].y - dataPoints[i-1].y) > 2)
			rows++;
	}

	return rows+1;
}


int findRetentionColCount(vector<Point> dataPoints){
	sort(dataPoints.begin(), dataPoints.end(), sort_y);
	
	//cout<<dataPoints<<"\n";

	int cols=0;

	for(int i=1;i< dataPoints.size();i++){
		if( abs(dataPoints[i].x - dataPoints[i-1].x) > 2)
			cols++;
	}

	return cols+1;
}


vector<vector<Mat>> multiTablePageSegmentation(Mat reconstructImg, Mat pointReducedImage, Mat clrImg, String filename){

	reconstructImg = (255 - reconstructImg);

	Mat labels;
	Mat stats;
	Mat centroids;

	int totalLabels = connectedComponentsWithStats(reconstructImg, labels, stats, centroids, 8, CV_32S);

	vector<Mat> tableSegments;
	vector<Mat> pointSegments;

	int margin = 2;

	for(int i=1;i< totalLabels; i++){

		Point topLeft, bottomRight; 
		topLeft.x = stats.at<int>(i, CC_STAT_LEFT) - margin;
		topLeft.y = stats.at<int>(i, CC_STAT_TOP) - margin;

		bottomRight.x = topLeft.x + stats.at<int>(i, CC_STAT_WIDTH) + 2*margin;
		bottomRight.y = topLeft.y + stats.at<int>(i, CC_STAT_HEIGHT) + 2*margin;

		Mat croppedTableImage = clrImg(Rect(topLeft, bottomRight));
		Mat croppedPointImage = pointReducedImage(Rect(topLeft, bottomRight));

		//imshow(filename+"_ConnectedTable"+ to_string(i), croppedTableImage);
		//imshow("ConnectedPoints"+ to_string(i), croppedPointImage);

		tableSegments.push_back(croppedTableImage);
		pointSegments.push_back(croppedPointImage);
	}

	vector<vector<Mat>> segmentationData;

	segmentationData.push_back(tableSegments);
	segmentationData.push_back(pointSegments);


	return segmentationData;
}



void tableRetention(vector<vector<Mat>> segmentationData, int ACH, String filename){

	vector<Mat> tableSegments = segmentationData[0];
	vector<Mat> pointSegments = segmentationData[1];

	vector<vector<Mat>> segmentedCellsInDoc;


	for(int i=0; i< tableSegments.size(); i++){

		//imshow(filename+"_Table"+to_string(i), tableSegments[i]);
		//imshow(filename+"_Points"+to_string(i), pointSegments[i]);
		vector<Point> pointDataset = extractIntersectionDataset(pointSegments[i]);
		segmentedCellsInDoc.push_back(cellExtraction(pointDataset, tableSegments[i], ACH, i, filename)) ;
	}

	generateXML(segmentedCellsInDoc);
	
}


vector<Point> extractIntersectionDataset(Mat img){

	vector<Point> reducedIntersectionPts;

	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){

			if((int)img.at<uchar>(i,j) !=0 ){
				reducedIntersectionPts.push_back(Point(j,i));
			}
		}
	}

	return reducedIntersectionPts;
}


vector<Mat> cellExtraction(vector<Point> dataPoints, Mat img, int ACH, int tableId,String filename){

	
	int rows = findRetentionRowCount(dataPoints);
	int cols = findRetentionColCount(dataPoints);
	
	//cout<<rows<<"\n";
	//cout<<cols<<"\n";

	map<string,int> rowIndexer;
	map<string,int> colIndexer;

	vector<Mat> segmentedMats;

	Point retentionTable[rows][cols];

	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			retentionTable[i][j] = Point(-1,-1);
		}
	}

	for(int i=0; i<dataPoints.size();i++){

		int rowcount=0;//,colcount=0;

		for(int j=0; j<dataPoints.size();j++){

			if(dataPoints[i] == dataPoints[j])
				break;

			if(abs(dataPoints[j].y - dataPoints[j+1].y) > 2 ){
				rowcount++;
			}
		}

		rowIndexer.insert(pair<string, int>(to_string(dataPoints[i].x) + "," + to_string(dataPoints[i].y), rowcount));

	}



	sort(dataPoints.begin(), dataPoints.end(), sort_y);

	for(int i=0; i<dataPoints.size();i++){

		int colcount=0;

		for(int j=0; j<dataPoints.size();j++){

			if(dataPoints[i] == dataPoints[j])
				break;

			if(abs(dataPoints[j].x - dataPoints[j+1].x) > 2 ){
				colcount++;
			}
		}

		colIndexer.insert(pair<string, int>(to_string(dataPoints[i].x) + "," + to_string(dataPoints[i].y), colcount));

	}


	/*
	map<string,int>::iterator it;

	for(it = rowIndexer.begin(); it != rowIndexer.end();++it){
		cout<< "["<<it->first <<"]:"<<it->second<<"\n";
	}

	cout<<"\n\n\n";


	for(it = colIndexer.begin(); it != colIndexer.end();++it){
		cout<<"["<<it->first <<"]:"<<it->second<<"\n";
	}

	cout<<"\n\n\n";
	*/

	for(int i=0; i< dataPoints.size(); i++){

		int row = rowIndexer.at(to_string(dataPoints[i].x) + "," + to_string(dataPoints[i].y));
		int col = colIndexer.at(to_string(dataPoints[i].x) + "," + to_string(dataPoints[i].y));

		retentionTable[row][col] = dataPoints[i];
	}

	

	/*for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			cout<<retentionTable[i][j];
		}

		//cout<<"\n";
	}*/


	//cout<<"\n\n";

	//cout<<dataPoints;



	for(int i=0; i<rows-1;i++){
		for(int j=0; j<cols-1;j++){

			Point topLeft, topRight, bottomLeft, bottomRight;

			topLeft = retentionTable[i][j];

			if(topLeft != Point(-1,-1)){

				int row = i+1;
				int col = j+1;

				while(retentionTable[row][j] == Point(-1,-1)){
					if(row == rows-1)
						break;

					row++;
				}

				while(retentionTable[i][col] == Point(-1,-1)){
					if(col == cols-1)
						break;

					col++;
				}

				bottomLeft = retentionTable[row][j];
				topRight = retentionTable[i][col];

				bottomRight = Point(topRight.x, bottomLeft.y);

				while(bottomLeft == Point(-1,-1)){
					row = i+1;

					while(retentionTable[row][col] == Point(-1,-1)){

						if(row == rows-1)
							break;

						row++;
					}

					if(retentionTable[row][col] == Point(-1,-1)){
						if(col != cols-1)
							col++;
					}else{
						topRight = retentionTable[i][col];
						bottomRight = retentionTable[row][col];

						bottomLeft.x = topLeft.x;
						bottomLeft.y = bottomRight.y;
					}
				}

				while(topRight == Point(-1,-1)){
					col = j+1;

					while(retentionTable[row][col] ==Point(-1,-1)){
						if(col == cols-1)
							break;

						col++;
					}

					if(retentionTable[row][col] == Point(-1,-1)){

						if(row != rows-1)
							row++;
					}else{

						bottomLeft = retentionTable[row][j];
						bottomRight =retentionTable[row][col];

						topRight.x = bottomRight.x;
						topRight.y = topLeft.y;
					}
				}


				//cout<<topLeft<<":"<<bottomLeft<<":"<<topRight<<":"<<bottomRight<<"\n";



				if( (bottomLeft.y - topLeft.y) > ACH && (topRight.x - topLeft.x) > ACH){
					Mat croppedimage = img(Rect(topLeft, bottomRight));
					
					segmentedMats.push_back(croppedimage);

					string imgname = filename+ "_Table"+to_string(tableId)+"_part_" + to_string(i+1)+"x"+to_string(j+1);
					imshow(imgname,croppedimage);
				}

			}

		}
	}

	return segmentedMats;
	
}

void generateXML(vector<vector<Mat>> segmentedCellsInDoc){
	/*
	string username = getenv("USER");
	string fileInitial = "/home/" + username + "/BE_Project/tes";

	ofstream finalFile;
	finalFile.open(filename+".txt", ios::out|ios::trunc);

	for(int i=0;i< segmentedMats.size();i++){

		cout<< segmentedMats[i].rows<<",";
		cout<< segmentedMats[i].cols<<"\n";


		resize(segmentedMats[i],segmentedMats[i], Size(), 10, 10, INTER_LINEAR);

		cout<< segmentedMats[i].rows<<",";
		cout<< segmentedMats[i].cols<<"\n";

		string wfilename = fileInitial + to_string(i)+"_temp.bmp";
		imwrite(wfilename, segmentedMats[i]);		

		string command = "tesseract "+wfilename+" out";
		system(command.c_str());

		ifstream tempFile;
		tempFile.open("out.txt",ios::in);

		string data((istreambuf_iterator<char> (tempFile)), istreambuf_iterator<char>());

		cout<<data;

		finalFile << data + "\n";

		remove(wfilename.c_str());
		tempFile.close();
	}

	finalFile.close();*/	
}
