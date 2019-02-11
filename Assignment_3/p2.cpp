#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

using namespace cv;

#define PI 3.14159265

int f[1920][1080];
double c[1920][1080];
double d1[1920][1080];
double d2[1920][1080];
double r1[1920][1080];


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

double calculateDct( int x1, int y1, int u, int v, int N) {
  double au,av;
  u= u-x1-8;
  v= v-y1-8;
  if(u==0){
    au = sqrt(1.00/N);
  } else {
    au = sqrt(2.00/N);
  }

  if(v==0){
    av = sqrt(1.00/N);
  } else {
    av = sqrt(2.00/N);
  }

  double sum =0;

  for(int x=x1-8; x<x1; x++) {
    for(int y=y1-8 ; y<y1; y++) {
      sum =sum + ((f[x][y])-128)*(cos(((2*(x-x1-8)+1)*u*PI)/(2.0*N)))*(cos(((2.0*(y-y1-8)+1)*v*PI)/(2.0*N)));
    }
  }
  return au*av*sum;
}

double calculateIdct1( int x1, int y1, int u, int v, int N) {
  double au,av;
  double sum =0;

  for(int x=x1-8; x<x1; x++) {
    for(int y=y1-8 ; y<y1; y++) {
      if(x-x1-8==0){
        au = sqrt(1.00/N);
      } else {
        au = sqrt(2.00/N);
      }

      if(y-y1-8==0){
        av = sqrt(1.00/N);
      } else {
        av = sqrt(2.00/N);
      }
      sum =sum + au*av*(((d1[x][y]))*(cos(((2*u+1)*(x-x1-8)*PI)/(2*N)))*(cos(((2*v+1)*(y-y1-8)*PI)/(2*N))));;
    }
  }
  r1[u][v] = sum+128;
  //cout<<"sum = "<<sum<<"\t";
  return sum+128;
}

double calculateIdct2( int x1, int y1, int u, int v, int N) {
  double au,av;
  double sum =0;

  for(int x=x1-8; x<x1; x++) {
    for(int y=y1-8 ; y<y1; y++) {
      if(x-x1-8==0){
        au = sqrt(1.00/N);
      } else {
        au = sqrt(2.00/N);
      }

      if(y-y1-8==0){
        av = sqrt(1.00/N);
      } else {
        av = sqrt(2.00/N);
      }
      sum =sum + au*av*(((d2[x][y]))*(cos(((2*u+1)*(x-x1-8)*PI)/(2*N)))*(cos(((2*v+1)*(y-y1-8)*PI)/(2*N))));;
    }
  }

  return sum+128;
}


void convertToDct1(Mat &sample) {
  for(int i=8; i<sample.rows; i=i+8){
    for(int j=8; j<sample.cols; j=j+8){

      for(int k=i-8; k<i; k++) {
        for(int l=j-8; l<j; l++) {
          c[k][l]  = calculateDct(i,j,k,l,8);
        }
      }
    }

  }
}


int main( int argc, char** argv ) {


  Mat originalSample = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  Mat intensitySample = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat sampleD1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat sampleD2 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat sampleR1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat sampleR2 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);


  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){

      float b = originalSample.at<Vec3b>(i, j)[0];
      float g = originalSample.at<Vec3b>(i, j)[1];
      float r = originalSample.at<Vec3b>(i, j)[2];
      float in;

      in = (b + g + r) / 3;

      intensitySample.at<uint8_t>(i, j) = in;
    }
  }

  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){
      f[i][j] = intensitySample.at<uint8_t>(i, j);
    }
  }

  convertToDct1(sampleD1);


  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){
      sampleD1.at<uint8_t>(i, j) =0;
      sampleD2.at<uint8_t>(i, j) =0;
      sampleR1.at<uint8_t>(i, j) =0;
      sampleR2.at<uint8_t>(i, j) =0;
      d1[i][j] = 0;
      d2[i][j]  =0;
    }
  }

  for(int i=0; i<originalSample.rows; i=i+8){
    for(int j=0; j<originalSample.cols; j=j+8){
      sampleD1.at<uint8_t>(i, j) = absloutePixelValue(int(c[i][j]));
      //cout<<i<<"\t"<<j<<"\t"<<absloutePixelValue(int(c[i][j]))<<endl;
    }
  }

  for(int i=0; i<originalSample.rows; i=i+8){
    for(int j=0; j<originalSample.cols; j=j+8){
      sampleD1.at<uint8_t>(i, j) = absloutePixelValue(int(c[i][j]));
      d1[i][j] = c[i][j];
      //cout<<i<<"\t"<<j<<"\t"<<absloutePixelValue(int(c[i][j]))<<endl;
    }
  }

  for(int i=0; i<originalSample.rows; i=i+8){
    for(int j=0; j<originalSample.cols; j=j+8){
      sampleD2.at<uint8_t>(i, j) = absloutePixelValue(int(c[i][j]));
      d2[i][j] = c[i][j];
      sampleD2.at<uint8_t>(i+1, j+1) = absloutePixelValue(int(c[i+1][j+1]));
      d2[i+1][j+1] = c[i+1][j+1];
      sampleD2.at<uint8_t>(i+1, j) = absloutePixelValue(int(c[i+1][j]));
      d2[i+1][j] = c[i+1][j];
      sampleD2.at<uint8_t>(i, j+1) = absloutePixelValue(int(c[i][j+1]));
      d2[i][j+1] = c[i][j+1];
      sampleD2.at<uint8_t>(i+2, j) = absloutePixelValue(int(c[i+2][j]));
      d2[i+2][j] = c[i+2][j];
      sampleD2.at<uint8_t>(i+1, j+2) = absloutePixelValue(int(c[i+1][j+2]));
      d2[i+1][j+2] = c[i+1][j+2];
      sampleD2.at<uint8_t>(i, j+3) = absloutePixelValue(int(c[i][j+3]));
      d2[i][j+3] = c[i][j+3];
      sampleD2.at<uint8_t>(i+1, j+2) = absloutePixelValue(int(c[i+1][j+2]));
      d2[i+1][j+2] = c[i+1][j+2];
      sampleD2.at<uint8_t>(i+2, j+1) = absloutePixelValue(int(c[i+2][j+1]));
      d2[i+2][j+1] = c[i+2][j+1];
      //cout<<i<<"\t"<<j<<"\t"<<absloutePixelValue(int(c[i][j]))<<endl;
    }
  }
/*
  for(int i=0; i<18; i++){
    for(int j=0; j<18; j++){
      cout<<d1[i][j]<<"\t\t";
    }
    cout<<endl;
  }
*/

  for(int i=8; i<sampleR1.rows; i=i+8){
    for(int j=8; j<sampleR1.cols; j=j+8){

      for(int k=i-8; k<i; k++) {
        for(int l=j-8; l<j; l++) {
          sampleR1.at<uint8_t>(k, l)  = absloutePixelValue(calculateIdct1(i,j,k,l,8));
        }
      }
    }

  }


  for(int i=8; i<sampleR2.rows; i=i+8){
    for(int j=8; j<sampleR2.cols; j=j+8){

      for(int k=i-8; k<i; k++) {
        for(int l=j-8; l<j; l++) {
          sampleR2.at<uint8_t>(k, l)  = absloutePixelValue(calculateIdct2(i,j,k,l,8));
        }
      }
    }

  }

  imshow("original Image", originalSample);
  imshow("intensity Image", intensitySample);
  imshow("D1 Image", sampleD1);
  imshow("D2 Image", sampleD2);
  imshow("R1 Image", sampleR1);
  imshow("R2 Image", sampleR2);
  //Wait for any key to press in window
  waitKey(0);

  //destory the windows
  destroyWindow("original Image");
  destroyWindow("intensity Image");
  destroyWindow("D1 Image");
  destroyWindow("D2 Image");
  destroyWindow("R1 Image");
  destroyWindow("R2 Image");

}
