// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
using namespace cv;


#include<iostream>
#include<vector>
using namespace std;

int find_threshold(int thresholdValue, int originalIntensityValues[]){

  int g1 = 0;
  int g2 = 0;
  int g1_denom = originalIntensityValues[0];
  int g2_denom= originalIntensityValues[0];


  for(int i=0; i<=thresholdValue; i++){
      g1 = g1 + originalIntensityValues[i]*i;
      g1_denom = g1_denom + originalIntensityValues[i];
  }

  for(int i=thresholdValue+1; i<=255; i++){
      g2 = g2 + originalIntensityValues[i]*i;
      g2_denom = g2_denom + originalIntensityValues[i];
  }

  int T = ((g1/g1_denom) + (g2/g2_denom))/2;

  if(T == thresholdValue){
    return T;
  }else{
    find_threshold(T, originalIntensityValues);
  }

}


int histogram(Mat &originalSample, int originalIntensityValues[]){
  //seting the default  to zero
  for(int i=0; i<=255; i++){
    originalIntensityValues[i] = 0;
  }

  //pixel intensity counting for Histogram
  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      originalIntensityValues[originalSample.at<uint8_t>(i,j)]++;
    }
  }
}

int matrix[1920][1080];

void four_connect_lableing(Mat &originalSample) {

  int counter = 0;

  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      matrix[i][j] = (int)originalSample.at<uint8_t>(i,j);
    }
  }

  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      if(matrix[i][j] == 255){
        if( i==0 && j==0) {
          counter++;
          matrix[i][j] = counter;
        }
        else if(i==0 &&  j>0) {
          if(matrix[i][j-1] != 0) {
            matrix[i][j] = matrix[i][j-1];
          }
          else {
            counter++;
            matrix[i][j] = counter;
          }
        }
        else if(j==0 && i>0) {
          if(matrix[i-1][j] != 0) {
            matrix[i][j] = matrix[i-1][j];
          }
          else {
            counter++;
            matrix[i][j] = counter;
          }
        }
        else {
          if(matrix[i][j-1] == 0 && matrix[i-1][j] == 0) {
            counter++;
            matrix[i][j] = counter;
          }
          else if (matrix[i][j-1] != 0 && matrix[i-1][j] == 0) {
            matrix[i][j] = matrix[i][j-1];
          }
          else if (matrix[i][j-1] == 0 && matrix[i-1][j] != 0) {
            matrix[i][j] = matrix[i-1][j];
          }
          else {
            int x = matrix[i][j-1];
            int y = matrix[i-1][j];

            if(x == y){
              matrix[i][j] = matrix[i][j-1];
            }
            else if( x > y){
              matrix[i][j] = matrix[i-1][j];
              //conflict
              for(int k = 0; k<=i; k++){
                for(int l = 0; l<originalSample.cols; l++){
                  if(matrix[k][l] == x) {
                    matrix[k][l] = y;
                  }
                }
              }
            }
            else {
              matrix[i][j] = matrix[i][j-1];
              //conflict
              for(int k = 0; k<=i; k++){
                for(int l = 0; l<originalSample.cols; l++){
                  if(matrix[k][l] == y) {
                    matrix[k][l] = x;
                  }
                }
              }
            }
          }

        }
      }
    }
  }

  int connectedIntensityValues[counter];

  //seting the default  to zero
  for(int i=0; i<=counter; i++){
    connectedIntensityValues[i] = 0;
  }

  //pixel intensity counting for Histogram
  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      connectedIntensityValues[matrix[i][j]]++;
    }
  }

  for(int k=1; k<=255; k++) {
    if(connectedIntensityValues[k] < 100 ) {
      for(int i=0; i<originalSample.rows; i++){
        for(int j=0; j<originalSample.cols; j++){
          if(matrix[i][j] == k){
            matrix[i][j] = 0;
          }
        }
      }
    }
  }

  int maxPixels = 0;
  int maxIntensity = 0;
  int minPixels = 100000;
  int minIntensity = 0;

  for(int i=1; i<=counter; i++) {
    if(connectedIntensityValues[i] > maxPixels ) {
      maxPixels = connectedIntensityValues[i];
      maxIntensity = i;
    }

    if(connectedIntensityValues[i] < minPixels && connectedIntensityValues[i] > 100 ) {
      minPixels = connectedIntensityValues[i];
      minIntensity = i;
    }
  }


  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){
      if(matrix[i][j] == maxIntensity){
        matrix[i][j] = 200;
      }
      else if(matrix[i][j] == minIntensity){
        matrix[i][j] = 60;
      }
      else {
        if(matrix[i][j] != 0){
          matrix[i][j] = 120;
        }
      }
    }
  }

  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      originalSample.at<uint8_t>(i,j) = matrix[i][j];
    }
  }


}


int main (int argc, char **argv) {

  // Reading the image file from the main argument
  Mat originalSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat binarySample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat modifiedSample = imread (argv[1], IMREAD_GRAYSCALE);


  //for storing the count of intensity value in the image
  int originalIntensityValues[257];
  int modifiedIntensityValues[257];
  //int connectedIntensityValues[257];

  //Check that the images loaded
  if (!originalSample.data)
    {
      cout << "ERROR: Could not load image data." << endl;
      return -1;
    }

  histogram(originalSample, originalIntensityValues);
  histogram(modifiedSample, modifiedIntensityValues);

  int thresholdValue = find_threshold(128,originalIntensityValues);


  cout << "Threshold value is " << thresholdValue << endl;

  threshold( originalSample, modifiedSample, thresholdValue, 255, THRESH_BINARY);
  threshold( originalSample, binarySample, thresholdValue, 255, THRESH_BINARY);

  four_connect_lableing(modifiedSample);

  namedWindow ("original Image");
  namedWindow ("binary Image");
  namedWindow ("modified Image");

  imshow ("original Image", originalSample);
  imshow ("binary Image", binarySample);
  imshow ("modified Image", modifiedSample);


  waitKey (0);

  //destory the windows
  destroyWindow ("original Image");
  destroyWindow ("binary Image");
  destroyWindow ("modified Image");


}
