
// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
using namespace cv;


#include<iostream>
#include<vector>
using namespace std;

int imageMatrix[1920][1080];
int modifiedMatrix[1920][1080];

int imageGrayMatrix[1920][1080];
int modifiedGrayMatrix[1920][1080];


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



int dilation(int imageRows, int imageClos) {

  int maskRows = 5;
  int maskCols = 5;

  int mask[maskRows][maskCols] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}};

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<imageRows-x; i++) {
    for(int j=y; j<imageClos-y; j++) {
      int sum = 0;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sum = sum || (imageMatrix[i-x+k][j-y+l] && mask[k][l]);
          //cout<<imageMatrix[i-x+k][j-y+l]<<" "<<mask[k][l]<<"   ";
        }
      }
      //cout<<endl<<sum<<endl;
      modifiedMatrix[i][j] = sum;

    }
  }
}


int grayDilation(int imageRows, int imageClos) {

  int maskRows = 5;
  int maskCols = 5;

  int mask[maskRows][maskCols] = {{50,50,50,50,50},{50,50,50,50,50},{50,50,50,50,50},{50,50,50,50,50},{50,50,50,50,50}};
  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<imageRows-x; i++) {
    for(int j=y; j<imageClos-y; j++) {
      int temp1 = 0;
      int temp2;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          temp2 = (imageGrayMatrix[i-x+k][j-y+l] + mask[k][l]);
          if(temp2 > temp1) {
            temp1 = temp2;
          }
          //cout<<imageMatrix[i-x+k][j-y+l]<<" "<<mask[k][l]<<"   ";
        }
      }
      //cout<<endl<<sum<<endl;
      modifiedGrayMatrix[i][j] = absloutePixelValue(temp1);

    }
  }
}

int grayerosion(int imageRows, int imageClos) {

  int maskRows = 5;
  int maskCols = 5;

  int mask[maskRows][maskCols] = {{180,180,180,180,180},{180,180,180,180,180},{180,180,180,180,180},{180,180,180,180,180},{180,180,180,180,180}};

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<imageRows-x; i++) {
    for(int j=y; j<imageClos-y; j++) {
      int temp1 = 255;
      int temp2;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          temp2 = (imageGrayMatrix[i-x+k][j-y+l] - mask[k][l]);
          if(temp2 < temp1) {
            temp1 = temp2;
          }
          //cout<<imageMatrix[i-x+k][j-y+l]<<" "<<mask[k][l]<<"   ";
        }
      }
      //cout<<endl<<sum<<endl;
      modifiedGrayMatrix[i][j] = absloutePixelValue(temp1);

    }
  }
}


int erosion(int imageRows, int imageClos) {

  int maskRows = 5;
  int maskCols = 5;

  int mask[maskRows][maskCols] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}};

  int x = maskRows/2;
  int y = maskCols/2;

  for(int i=x; i<imageRows-x; i++) {
    for(int j=y; j<imageClos-y; j++) {
      int sum = 1;

      for(int k=0; k<maskRows; k++) {
        for(int l=0; l<maskCols; l++) {
          sum = sum && (imageMatrix[i-x+k][j-y+l] && mask[k][l]);
          //cout<<imageMatrix[i-x+k][j-y+l]<<" "<<mask[k][l]<<"   ";
        }
      }
      //cout<<endl<<sum<<endl;
      modifiedMatrix[i][j] = sum;

    }
  }
}


int find_threshold(int thresholdValue, int originalIntensityValues[]){

  int g1 = 0;
  int g2 = 0;
  int g1_denom = 0;
  int g2_denom= 0;


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


int main (int argc, char **argv) {

  // Reading the image file from the main argument
  Mat originalSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat binarySample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat modifiedSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat erosionSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat dilationSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat openingSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat greyScaleErosionSample = imread (argv[1], IMREAD_GRAYSCALE);
  Mat greyScaleOpeningSample = imread (argv[1], IMREAD_GRAYSCALE);



  //for storing the count of intensity value in the image
  int originalIntensityValues[257];
  int modifiedIntensityValues[257];

  for(int i=0; i<1920; i++) {
    for(int j=0; j<1080; j++) {
      imageMatrix[i][j] = 0;
      modifiedMatrix[i][j] =0;
    }
  }

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

  threshold( originalSample, modifiedSample, thresholdValue+30, 255, THRESH_BINARY);


//converting the pixels to 0 to 255 to 0,1
  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
      if(modifiedSample.at<uint8_t>(i,j) == 255) {
        imageMatrix[i][j] = 1;
      }
      else{
        imageMatrix[i][j] = 0;
      }
    }
  }

  erosion(modifiedSample.rows, modifiedSample.cols);

//copying the modified image matrix to image matrix
  for(int i=0; i<1920; i++) {
    for(int j=0; j<1080; j++) {
      imageMatrix[i][j] = modifiedMatrix[i][j];
      modifiedMatrix[i][j] = 0;
    }
  }

  for(int i=0; i< erosionSample.rows; i++){
    for(int j=0; j< erosionSample.cols; j++){
      if(imageMatrix[i][j] == 1) {
        erosionSample.at<uint8_t>(i,j) = 255;
      }
      else if(imageMatrix[i][j] == 0){
        erosionSample.at<uint8_t>(i,j) = 0;
      }
    }
  }

  dilation(modifiedSample.rows, modifiedSample.cols);

  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
      if(modifiedMatrix[i][j] == 1) {
        modifiedSample.at<uint8_t>(i,j) = 255;
      }
      else if(modifiedMatrix[i][j] == 0){
        modifiedSample.at<uint8_t>(i,j) = 0;
      }
    }
  }


  threshold( originalSample, binarySample, thresholdValue, 255, THRESH_BINARY);

  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
        imageGrayMatrix[i][j] = originalSample.at<uint8_t>(i,j);
    }
  }

  grayerosion(modifiedSample.rows, modifiedSample.cols);

  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
        greyScaleErosionSample.at<uint8_t>(i,j) = modifiedGrayMatrix[i][j];
        imageGrayMatrix[i][j] = modifiedGrayMatrix[i][j];
        modifiedGrayMatrix[i][j] = 0;
    }
  }

  grayDilation(modifiedSample.rows, modifiedSample.cols);

  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
      greyScaleOpeningSample.at<uint8_t>(i,j) = modifiedGrayMatrix[i][j];
    }
  }

  imshow ("original Image", originalSample);
  imshow ("binary Image", binarySample);
  imshow ("opening Image", modifiedSample);
  imshow ("erosion Image", erosionSample);
  imshow ("grayscale erosion Image", greyScaleErosionSample);
  imshow ("grayscale opening Image", greyScaleOpeningSample);


  waitKey (0);

  //destory the windows
  destroyWindow ("original Image");
  destroyWindow ("binary Image");
  destroyWindow ("opening Image");
  destroyWindow ("erosion Image");
  destroyWindow ("grayscale erosion Image");
  destroyWindow ("grayscale opening Image");

}
