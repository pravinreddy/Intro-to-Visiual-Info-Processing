// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include<string.h>
using namespace cv;


#include<iostream>
using namespace std;

int originalImageMatrix1[1920][1080];
int originalImageMatrix2[1920][1080];
int modifiedImageMatrixM1[1920][1080];
int modifiedImageMatrixM2[1920][1080];

int rowSize;
int colSize;

struct subImageClassInfoStruct
{
	int classLable;
  int blockRowNumber;
  int blockColNumber;
  int averageIntensityValue;
};

subImageClassInfoStruct subImageClassInfo1[4080];
subImageClassInfoStruct subImageClassInfo2[4080];
int nSubImages1 = 0;  //number of sub images classinfo stored in the struct
int nSubImages2 = 0;  //number of sub images classinfo stored in the struct

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

void findClassinfo1(int r, int c) {

	subImageClassInfo1[nSubImages1].blockRowNumber = r;
	subImageClassInfo1[nSubImages1].blockColNumber = c;
	int sum = 0;
	for(int i=r; i< r+8; i++){
    for(int j=c; j< c+8; j++) {
      sum= sum + originalImageMatrix1[i][j];
    }
  }
	sum = sum/64;
	subImageClassInfo1[nSubImages1].averageIntensityValue = sum;
	subImageClassInfo1[nSubImages1].classLable = findClassLable(sum);
	int intesityValue1 = findClassLable(sum);

	for(int i=r; i< r+8; i++){
    for(int j=c; j< c+8; j++) {
      modifiedImageMatrixM1[i][j] = intesityValue1;
    }
  }
	nSubImages1++;
}

void findClassinfo2(int r, int c) {

	subImageClassInfo2[nSubImages2].blockRowNumber = r;
	subImageClassInfo2[nSubImages2].blockColNumber = c;
	int sum = 0;
	for(int i=r; i< r+8; i++){
    for(int j=c; j< c+8; j++) {
      sum= sum + originalImageMatrix2[i][j];
    }
  }
	sum = sum/64;
	subImageClassInfo2[nSubImages2].averageIntensityValue = sum;
	subImageClassInfo2[nSubImages2].classLable = findClassLable(sum);
	int intesityValue2 = findClassLable(sum);

	for(int i=r; i< r+8; i++){
    for(int j=c; j< c+8; j++) {
      modifiedImageMatrixM2[i][j] = intesityValue2;
    }
  }
	nSubImages2++;
}

void calculateM1() {
	for(int i=0; i<rowSize; i = i+8){
    for(int j=0; j<colSize; j = j+8) {
			findClassinfo1(i,j);
    }
  }
}

void calculateM2() {
	for(int i=0; i<rowSize; i = i+8){
    for(int j=0; j<colSize; j = j+8) {
			findClassinfo2(i,j);
    }
  }
}

int main(int argc, char** argv) {

	// Reading the image file from the main argument
 	Mat originalSample1 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat originalSample2 = imread(argv[2], IMREAD_GRAYSCALE);
	Mat imageM1 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat imageM2 = imread(argv[1], IMREAD_GRAYSCALE);

	rowSize = originalSample1.rows;
	colSize = originalSample1.cols;

  for(int i=0; i<originalSample1.rows; i++){
    for(int j=0; j<originalSample1.cols; j++) {
      originalImageMatrix1[i][j] = (int)originalSample1.at<uint8_t>(i,j);
    }
  }

	for(int i=0; i<originalSample2.rows; i++){
    for(int j=0; j<originalSample2.cols; j++) {
      originalImageMatrix2[i][j] = (int)originalSample2.at<uint8_t>(i,j);
    }
  }

	calculateM1();
	calculateM2();

	for(int i=0; i<originalSample1.rows; i++){
		for(int j=0; j<originalSample1.cols; j++) {
			imageM1.at<uint8_t>(i,j) = modifiedImageMatrixM1[i][j];
		}
	}
	for(int i=0; i<originalSample2.rows; i++){
    for(int j=0; j<originalSample2.cols; j++) {
      imageM2.at<uint8_t>(i,j) = modifiedImageMatrixM2[i][j];
    }
  }

	// Dispaly the the images in the windows
	imshow("original Image1", originalSample1);
	imshow("original Image2", originalSample2);
	imshow("M1", imageM1);
	imshow("M2", imageM2);


  waitKey(0);

  //destory the windows
  destroyAllWindows();
}
