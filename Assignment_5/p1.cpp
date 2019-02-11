// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include<string.h>
#include<math.h>
using namespace cv;


#include<iostream>
using namespace std;


int originalImageMatrix[1920][1080];
int modifiedImageMatrixM1[1920][1080];
int modifiedImageMatrixN1[1920][1080];
int modifiedImageMatrixN2[1920][1080];
int modifiedImageMatrixN3[1920][1080];
int modifiedImageMatrixN4[1920][1080];
int modifiedImageMatrixK1[1920][1080];

int rowSize;
int colSize;
int y,z;
int tempClassLable;
int tempi, tempj;

struct tPoint
{
    int val;     // Group of point
    int x, y;     // Co-ordinate of point
    int distance; // Distance from test point
		int avg;
};

tPoint points[5000];
int pointCounter;


struct subImageClassInfoStruct
{
	int classLable;
  int blockRowNumber;
  int blockColNumber;
  int averageIntensityValue;
};

subImageClassInfoStruct subImageClassInfo[4080];
int nSubImages = 0;  //number of sub images classinfo stored in the struct


int findClassLable(int average) {
	if(average>=0 && average<125){
		return 0;
	}
	else if(average>=125 && average<175){
		return 128;
	}
	else if(average>=175 && average<=255) {
		return 255;
	}
	else{
		return 128;
	}
}

void findClassinfo(int r, int c) {

	subImageClassInfo[nSubImages].blockRowNumber = r;
	subImageClassInfo[nSubImages].blockColNumber = c;
	int sum = 0;
	for(int i=r; i< r+4; i++){
    for(int j=c; j< c+4; j++) {
      sum= sum + originalImageMatrix[i][j];
    }
  }
	sum = sum/16;
	subImageClassInfo[nSubImages].averageIntensityValue = sum;
	subImageClassInfo[nSubImages].classLable = findClassLable(sum);
	int intesityValue = findClassLable(sum);

	for(int i=r; i< r+4; i++){
    for(int j=c; j< c+4; j++) {
      modifiedImageMatrixM1[i][j] = intesityValue;
    }
  }
	nSubImages++;
}


void calculateM1() {
	for(int i=0; i<rowSize; i = i+4){
    for(int j=0; j<colSize; j = j+4) {
			findClassinfo(i,j);
    }
  }
}

void swap (int x, int y) {
	int tempVal;
	int tempDistance;
	int tempX;
	int tempY;
	int tempAvg;

	tempVal = points[x].val;
	tempDistance = points[x].distance;
	tempX = points[x].x;
	tempY = points[x].y;
	tempAvg = points[x].avg;

	points[x].val = points[y].val;
	points[x].distance = points[y].distance;
	points[x].x = points[y].x;
	points[x].y = points[y].y;
	points[x].avg = points[y].avg;

	points[y].val = tempVal;
	points[y].distance = tempDistance;
	points[y].x = tempX;
	points[y].y = tempY;
	points[y].avg = tempAvg;
}

int sort1() {
	for(int i=0; i< pointCounter; i++) {
		for(int j=i+1; j< pointCounter; j++) {
			if(points[j].distance < points[i].distance )
				swap(j, i);
		}
	}

	int f1=0;
	int f2=0;
	int f3 =0;

	for( int i=0; i<50; i++ ) {
		if(points[i].val == 0){
			f1++;
		}
		else if(points[i].val == 128){
			f2++;
		}
		else if(points[i].val == 255) {
			f3++;
		}
	}

	if( f1>=f2 && f1>=f3 )
        return 0;

    if( f2>=f1 && f2>=f3 )
        return 128;

    if( f3>=f1 && f3>=f2 )
        return 255;

	return 0;
}

void calculateN1() {

	for(int i=0; i<rowSize/2; i++){
		for(int j=0; j<colSize; j++) {
			modifiedImageMatrixN1[i][j] = modifiedImageMatrixM1[i][j];
		}
	}

	for(int i=rowSize/2; i<rowSize; i = i+4){
		for(int j=0; j<colSize; j = j+4) {
			//int min = 255;
			int temp =0;
			pointCounter = 0;
			int sum = 0 ;
			for(int k=0; k< rowSize/2; k = k+4){
				for(int l=0; l< colSize; l = l+4) {
					for(int i1=0; i1< 4; i1++){
						for(int j1=0; j1< 4; j1++) {
							 sum = sum + pow(originalImageMatrix[k+i1][l+j1] - originalImageMatrix[i+i1][j+j1] , 2);
						}
					}

					sum = sqrt(sum)/16;
					points[pointCounter].distance = sum;
					points[pointCounter].x = i;
					points[pointCounter].y = j;
					points[pointCounter].val = subImageClassInfo[temp].classLable;
					pointCounter++;
					temp++;
				}
			}
			int temp1 = sort1();
			for(int i2=0; i2< 4; i2++){
				for(int j2=0; j2< 4; j2++) {
					modifiedImageMatrixN1[i+i2][j+j2] = temp1;
				}
			}
		}
	}
}


void sort2() {
	for(int i=0; i< pointCounter; i++) {
		for(int j=i+1; j< pointCounter; j++) {
			if(points[j].distance < points[i].distance )
				swap(j, i);
		}
	}

	int f1=0;
	int f2=0;
	int f3 =0;
	int max = 0;

	for( int i=0; i<50; i++ ) {
		if(points[i].val == 0){
			f1++;
		}
		else if(points[i].val == 128){
			f2++;
		}
		else if(points[i].val == 255) {
			f3++;
		}
	}

	if( f1>=f2 && f1>=f3 )
        max = 0;

    if( f2>=f1 && f2>=f3 )
        max = 128;

    if( f3>=f1 && f3>=f2 )
        max = 255;

	for(int i=0; i<pointCounter; i++) {
		if(max == points[i].val){
			tempi = points[i].x;
			tempj = points[i].y;
			break;
		}
	}
}


void calculateN2() {

	for(int i=0; i<rowSize/2; i++){
		for(int j=0; j<colSize; j++) {
			modifiedImageMatrixN2[i][j] = modifiedImageMatrixM1[i][j];
		}
	}

	for(int i=rowSize/2; i<rowSize; i = i+4){
		for(int j=0; j<colSize; j = j+4) {
			//int min = 255;
			int temp =0;
			pointCounter = 0;
			int sum = 0 ;
			for(int k=0; k< rowSize/2; k = k+4){
				for(int l=0; l< colSize; l = l+4) {
					for(int i1=0; i1< 4; i1++){
						for(int j1=0; j1< 4; j1++) {
							 sum = sum + pow(originalImageMatrix[k+i1][l+j1] - originalImageMatrix[i+i1][j+j1] , 2);
						}
					}

					sum = sqrt(sum)/16;
					points[pointCounter].distance = sum;
					points[pointCounter].x = k;
					points[pointCounter].y = l;
					points[pointCounter].val = subImageClassInfo[temp].classLable;
					pointCounter++;
					temp++;
				}
			}
			sort2();
			for(int i2=0; i2< 4; i2++){
				for(int j2=0; j2< 4; j2++) {
					modifiedImageMatrixN2[i+i2][j+j2] = originalImageMatrix[tempi + i2][tempj + j2];
				}
			}
		}
	}
}



int sort3() {
	for(int i=0; i< pointCounter; i++) {
		for(int j=i+1; j< pointCounter; j++) {
			if(points[j].distance < points[i].distance )
				swap(j, i);
		}
	}

	int f1=0;
	int f2=0;
	int f3 =0;
	int max = 0;

	for( int i=0; i<50; i++ ) {
		if(points[i].val == 0){
			f1++;
		}
		else if(points[i].val == 128){
			f2++;
		}
		else if(points[i].val == 255) {
			f3++;
		}
	}

	if( f1>=f2 && f1>=f3 )
        max = 0;

    if( f2>=f1 && f2>=f3 )
        max = 128;

    if( f3>=f1 && f3>=f2 )
        max = 255;

	for(int i=0; i<pointCounter; i++) {
		if(max == points[i].val){
			return points[i].avg;
		}
	}
}



void calculateN3() {

	for(int i=0; i<rowSize/2; i++){
		for(int j=0; j<colSize; j++) {
			modifiedImageMatrixN3[i][j] = modifiedImageMatrixM1[i][j];
		}
	}

	for(int i=rowSize/2; i<rowSize; i = i+4){
		for(int j=0; j<colSize; j = j+4) {
			//int min = 255;
			int temp =0;
			pointCounter = 0;
			int sum = 0 ;
			for(int k=0; k< rowSize/2; k = k+4){
				for(int l=0; l< colSize; l = l+4) {
					for(int i1=0; i1< 4; i1++){
						for(int j1=0; j1< 4; j1++) {
							 sum = sum + pow(originalImageMatrix[k+i1][l+j1] - originalImageMatrix[i+i1][j+j1] , 2);
						}
					}

					sum = sqrt(sum)/16;
					points[pointCounter].distance = sum;
					points[pointCounter].x = k;
					points[pointCounter].y = l;
					points[pointCounter].val = subImageClassInfo[temp].classLable;
					points[pointCounter].avg = subImageClassInfo[temp].averageIntensityValue;
					pointCounter++;
					temp++;
				}
			}
			int temp3 = sort3();
			for(int i2=0; i2< 4; i2++){
				for(int j2=0; j2< 4; j2++) {
					modifiedImageMatrixN3[i+i2][j+j2] = temp3;
				}
			}
		}
	}
}


int sort4() {
	for(int i=0; i< pointCounter; i++) {
		for(int j=i+1; j< pointCounter; j++) {
			if(points[j].distance < points[i].distance )
				swap(j, i);
		}
	}

	int f1=0;
	int f2=0;
	int f3 =0;
	int max = 0;

	for( int i=0; i<50; i++ ) {
		if(points[i].val == 0){
			f1++;
		}
		else if(points[i].val == 128){
			f2++;
		}
		else if(points[i].val == 255) {
			f3++;
		}
	}

	if( f1>=f2 && f1>=f3 )
        max = 0;

    if( f2>=f1 && f2>=f3 )
        max = 128;

    if( f3>=f1 && f3>=f2 )
        max = 255;


	int tempAverage = 0;
	int tempAverageCounter = 0;

	for(int i=0; i<pointCounter; i++) {
		if(max == points[i].val){
			tempAverage = tempAverage + points[i].avg;
			tempAverageCounter++;
		}
	}

	return (tempAverage/tempAverageCounter);
}



void calculateN4() {

	for(int i=0; i<rowSize/2; i++){
		for(int j=0; j<colSize; j++) {
			modifiedImageMatrixN4[i][j] = modifiedImageMatrixM1[i][j];
		}
	}

	for(int i=rowSize/2; i<rowSize; i = i+4){
		for(int j=0; j<colSize; j = j+4) {
			//int min = 255;
			int temp =0;
			pointCounter = 0;
			int sum = 0 ;
			for(int k=0; k< rowSize/2; k = k+4){
				for(int l=0; l< colSize; l = l+4) {
					for(int i1=0; i1< 4; i1++){
						for(int j1=0; j1< 4; j1++) {
							 sum = sum + pow(originalImageMatrix[k+i1][l+j1] - originalImageMatrix[i+i1][j+j1] , 2);
						}
					}

					sum = sqrt(sum)/16;
					points[pointCounter].distance = sum;
					points[pointCounter].x = k;
					points[pointCounter].y = l;
					points[pointCounter].val = subImageClassInfo[temp].classLable;
					points[pointCounter].avg = subImageClassInfo[temp].averageIntensityValue;
					pointCounter++;
					temp++;
				}
			}
			int temp4 = sort4();
			for(int i2=0; i2< 4; i2++){
				for(int j2=0; j2< 4; j2++) {
					modifiedImageMatrixN4[i+i2][j+j2] = temp4;
				}
			}
		}
	}
}

int errorRate() {

	float total = 0;
	float div = 0;

	for(int i=rowSize/2; i<rowSize; i=i+4) {
		for(int j=0; j<colSize; j=j+4) {


			for(int i2=0; i2< 4; i2++){
				for(int j2=0; j2< 4; j2++) {
					if(modifiedImageMatrixM1[i+i2][j+j2] !=  modifiedImageMatrixN1[i+i2][j+j2]) {
						total++;
					}
				}
			}
			div++;
		}
	}
	total = total/4.00;
	total = (total/div)*100.00;

	return total;
}

void calculateK1() {
  for(int i=0; i<rowSize; i++){
    for(int j=0; j<colSize; j++) {
      int a = 0;
      int b = 128;
      int c = 255;
      int tempK = originalImageMatrix[i][j];
      int result =0;

      if(tempK >=0 && tempK <=64) {
        result  = 0;

      }
      else if(tempK >64 && tempK<=192) {
        result = 128;
      }
      else{
        result = 255;
      }

      modifiedImageMatrixK1[i][j] = result;

    }
  }
}



int main(int argc, char** argv)
{

	// Reading the image file from the main argument
 	Mat originalSample = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageM1 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageN1 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageN2 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageN3 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageN4 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageT1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat imageK1 = imread(argv[1], IMREAD_GRAYSCALE);

	rowSize = originalSample.rows;
	colSize = originalSample.cols;

  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      originalImageMatrix[i][j] = (int)originalSample.at<uint8_t>(i,j);
    }
  }

	calculateM1();

	for(int i=0; i<originalSample.rows/2; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageM1.at<uint8_t>(i,j) = modifiedImageMatrixM1[i][j];
    }
  }

	calculateN1();

	for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageN1.at<uint8_t>(i,j) = modifiedImageMatrixN1[i][j];
    }
  }

	calculateN2();

	for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageN2.at<uint8_t>(i,j) = modifiedImageMatrixN2[i][j];
    }
  }

	calculateN3();

	for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageN3.at<uint8_t>(i,j) = modifiedImageMatrixN3[i][j];
    }
  }

	calculateN4();

	for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageN4.at<uint8_t>(i,j) = modifiedImageMatrixN4[i][j];
    }
  }

	for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageT1.at<uint8_t>(i,j) = modifiedImageMatrixM1[i][j];
    }
  }

  calculateK1();

  for(int i=0; i<originalSample.rows/2; i++){
    for(int j=0; j<originalSample.cols; j++) {
      imageK1.at<uint8_t>(i,j) = modifiedImageMatrixK1[i][j];
    }
  }


	//Check that the images loaded
	if(!originalSample.data ) {
		cout << "ERROR: Could not load image data." << endl;
		return -1;
	}

	int error = errorRate();
	cout<<"Error Rate : "<<error<<endl;

  // Dispaly the the images in the windows
  imshow("original Image", originalSample);
	imshow("M1", imageM1);
	imshow("N1", imageN1);
	imshow("N2", imageN2);
	imshow("N3", imageN3);
	imshow("N4", imageN4);
	imshow("T1", imageT1);
  imshow("K1", imageK1);

  waitKey(0);

  //destory the windows
  destroyAllWindows();

}
