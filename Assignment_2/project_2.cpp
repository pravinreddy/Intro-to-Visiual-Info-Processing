// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
using namespace cv;

#include<iostream>
#include<math.h>
using namespace std;


int matrix[1920][1080];

void smoothing(Mat &Sample) {

  int maskRows = 3;
  int maskCols = 3;

  int mask[maskRows][maskCols] = {{1,2,1},{2,4,2},{1,2,1}};

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<Sample.rows-x; i++){
    for(int j=y; j<Sample.cols-y; j++){

      int sum = 0;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sum = sum + ((int)Sample.at<uint8_t>(i-x+k ,j-y+l)) * mask[k][l];
        }
      }

      Sample.at<uint8_t>( i, j) = (int)((float)sum/16.0);

    }
  }

}

int imageMatrix[1920][1080];
int sobelX[1920][1080];
int sobelY[1920][1080];

int absloutePixelValue(int value) {
  if(value>255) {
    return 255;
  }
  else if(value < 0) {
    return 0;
  }
  else {
    return value;
  }
}

void sobelOperator(Mat &Sample) {
  int maskRows = 3;
  int maskCols = 3;

  int maskX[maskRows][maskCols] = {{-1,0,1},{-2,0,2},{-1,0,1}};
  int maskY[maskRows][maskCols] = {{-1,-2,-1},{0,0,0},{1,2,1}};

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=0; i<Sample.rows; i++){
    for(int j=0; j<Sample.cols; j++){

      imageMatrix[i][j] = Sample.at<uint8_t>( i, j);
      sobelX[i][j] = 0;
      sobelY[i][j] = 0;

    }
  }

  for(int i=x; i<Sample.rows-x; i++){
    for(int j=y; j<Sample.cols-y; j++){

      int sumX = 0;
      int sumY = 0;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sumX = sumX + imageMatrix[i-x+k][j-y+l] * maskX[k][l];
          sumY = sumY + imageMatrix[i-x+k][j-y+l] * maskY[k][l];

        }
      }

      sobelX[i][j] = sumX;
      sobelY[i][j] = sumY;

    }
  }

  int maxValue =0;

  for(int i=x; i<Sample.rows-x; i++){
    for(int j=y; j<Sample.cols-y; j++) {
      imageMatrix[i][j] = absloutePixelValue(sqrt(pow(sobelX[i][j],2) + pow(sobelY[i][j],2)));
      Sample.at<uint8_t>( i, j) = absloutePixelValue(sqrt(pow(sobelX[i][j],2) + pow(sobelY[i][j],2)));

      if(imageMatrix[i][j] > maxValue) {
        maxValue = absloutePixelValue(imageMatrix[i][j]);
      }
    }
  }

}


double mask7x7[8][8];
double mask11x11[12][12];


double logValue(double sigma, int x, int y) {
  double x1temp = 1.00/(sqrt(2*3.142857)*pow(sigma, 2));
  double x1 = pow( x1temp, 2);
  double x2temp = (pow(x,2)+pow(y,2))/pow(sigma,2);
  double x2 = x2temp - 2.0000;
  double x3temp = x2temp/2.0000;
  double x3 = pow(2.718281828,-x3temp);

  return (x1*x2*x3);

}


void logMask7x7(double sigma) {
  for(int i=-3; i<=3 ;i++) {
    for(int j=-3; j<=3; j++) {
      mask7x7[i+3][j+3] = round(logValue(sigma, i,j)*180);
    }
  }
}


void logMask11x11(double sigma) {
  for(int i=-5; i<=5 ;i++) {
    for(int j=-5; j<=5; j++) {
      mask11x11[i+5][j+5] = round(logValue(sigma, i,j)*25500);
    }
  }
}

void log7x7(Mat &Sample) {

  int maskRows = 7;
  int maskCols = 7;

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<Sample.rows-x; i++){
    for(int j=y; j<Sample.cols-y; j++){

      int sum = 0;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sum = sum + ((int)Sample.at<uint8_t>(i-x+k ,j-y+l)) * mask7x7[k][l];
        }
      }

      Sample.at<uint8_t>( i, j) = absloutePixelValue(round((float)sum));

    }
  }

}

void log11x11(Mat &Sample) {

  int maskRows = 11;
  int maskCols = 11;

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<Sample.rows-x; i++){
    for(int j=y; j<Sample.cols-y; j++){

      int sum = 0;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sum = sum + ((int)Sample.at<uint8_t>(i-x+k ,j-y+l)) * mask11x11[k][l];
        }
      }

      Sample.at<uint8_t>( i, j) = absloutePixelValue(round((float)sum));

    }
  }

}




int main(int argc, char** argv)
{

  logMask7x7(1.4);
  logMask11x11(5.0);

  cout<<"LOG kernal 11x11"<<endl;
  for(int i=0; i<11 ;i++) {
    for(int j=0; j<11; j++) {
      cout<<mask11x11[i][j]<<"\t";
    }
    cout<<endl;
  }

  cout<<"LOG kernal 7x7"<<endl;
  for(int i=0; i<7 ;i++) {
    for(int j=0; j<7; j++) {
      cout<<mask7x7[i][j]<<"\t";
    }
    cout<<endl;
  }

  Mat originalSample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat modifiedSample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat detailSample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat unsharpSample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat sobelSample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat log7x7Sample1 = imread(argv[1], IMREAD_GRAYSCALE);
  Mat log11x11Sample1 = imread(argv[1], IMREAD_GRAYSCALE);

  Mat originalSample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat modifiedSample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat detailSample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat unsharpSample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat sobelSample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat log7x7Sample2 = imread(argv[2], IMREAD_GRAYSCALE);
  Mat log11x11Sample2 = imread(argv[2], IMREAD_GRAYSCALE);

  //Check that the images loaded
  if(!originalSample1.data ) {
    cout << "ERROR: Could not load image 1 data." << endl;
    return -1;
  }

  //Check that the images loaded
  if(!originalSample2.data ) {
    cout << "ERROR: Could not load image 2 data." << endl;
    return -1;
  }

  //Create the display windows
  namedWindow("original ant_gray Image");
  namedWindow("unsharp ant_gray Image");
  namedWindow("sobel ant_gray Image");
  namedWindow("LOG 7x7 ant_gray Image");
  namedWindow("LOG 11x11 ant_gray Image");

  //Create the display windows
  namedWindow("original basel_gray Image");
  namedWindow("unsharp basel_gray Image");
  namedWindow("sobel basel_gray Image");
  namedWindow("LOG 7x7 basel_gray Image");
  namedWindow("LOG 11x11 basel_gray Image");

  smoothing(modifiedSample1);

  for(int i=0; i<modifiedSample1.rows; i++){
    for(int j=0; j<modifiedSample1.cols; j++){
      detailSample1.at<uint8_t>(i ,j) = absloutePixelValue(originalSample1.at<uint8_t>(i ,j) - modifiedSample1.at<uint8_t>(i ,j));
    }
  }

  for(int i=0; i<originalSample1.rows; i++){
    for(int j=0; j<originalSample1.cols; j++){
      unsharpSample1.at<uint8_t>(i ,j) = absloutePixelValue(originalSample1.at<uint8_t>(i ,j) + detailSample1.at<uint8_t>(i ,j));
    }
  }

  smoothing(modifiedSample2);

  for(int i=0; i<modifiedSample2.rows; i++){
    for(int j=0; j<modifiedSample2.cols; j++){
      detailSample2.at<uint8_t>(i ,j) = absloutePixelValue(originalSample2.at<uint8_t>(i ,j) - modifiedSample2.at<uint8_t>(i ,j));
    }
  }

  for(int i=0; i<originalSample2.rows; i++){
    for(int j=0; j<originalSample2.cols; j++){
      unsharpSample2.at<uint8_t>(i ,j) = absloutePixelValue(originalSample2.at<uint8_t>(i ,j) + detailSample2.at<uint8_t>(i ,j));
    }
  }

  sobelOperator(sobelSample1);

  log7x7(log7x7Sample1);
  log11x11(log11x11Sample1);

  sobelOperator(sobelSample2);

  log7x7(log7x7Sample2);
  log11x11(log11x11Sample2);

  // Dispaly the the images in the windows
  imshow("original ant_gray Image", originalSample1);
  imshow("unsharp ant_gray Image", unsharpSample1);
  imshow("sobel ant_gray Image", sobelSample1);
  imshow("LOG 7x7 ant_gray Image", log7x7Sample1);
  imshow("LOG 11x11 ant_gray Image", log11x11Sample1);

  // Dispaly the the images in the windows
  imshow("original basel_gray Image", originalSample2);
  imshow("unsharp basel_gray Image", unsharpSample2);
  imshow("sobel basel_gray Image", sobelSample2);
  imshow("LOG 7x7 basel_gray Image", log7x7Sample2);
  imshow("LOG 11x11 basel_gray Image", log11x11Sample2);

  //Wait for any key to press in window
  waitKey(0);

  destroyAllWindows();
}
