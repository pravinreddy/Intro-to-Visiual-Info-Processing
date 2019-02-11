// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
using namespace cv;


#include<iostream>
using namespace std;


// main function
int main(int argc, char** argv)
{

  // Reading the image file from the main argument
  Mat originalSample = imread(argv[1], IMREAD_GRAYSCALE);
  Mat modifiedSample = imread(argv[1], IMREAD_GRAYSCALE);


  //Check that the images loaded
  if(!originalSample.data ) {
    cout << "ERROR: Could not load image data." << endl;
    return -1;
  }

  //for storing the count of intensity value in the image
  int originalIntensityValues[255];
  int modifiedIntensityValues[255];

  for(int i=0; i<=255; i++){
    originalIntensityValues[i] = 0;
    modifiedIntensityValues[i] = 0;
  }

  int originalMaxIntensity = 0;

  //pixel intensity counting for Histogram
  for(int i=0; i< originalSample.rows; i++){
    for(int j=0; j< originalSample.cols; j++){
      originalIntensityValues[originalSample.at<uint8_t>(i,j)]++;
      if(originalMaxIntensity <originalIntensityValues[originalSample.at<uint8_t>(i,j)]){
        originalMaxIntensity = originalIntensityValues[originalSample.at<uint8_t>(i,j)];
      }
    }
  }

  //to store the propability of each pixel intensity
  float originalPropabilityValues[255];
  float originalCummulativePropabilityValues[255];


  //storing the default propability values as zero
  for(int i=0; i<=255; i++){
    originalPropabilityValues[i] = 0;
    originalCummulativePropabilityValues[i] = 0;
  }

  int totalNumberOfPixels = originalSample.rows * originalSample.cols;

  for(int i=0; i<=255; i++){
    originalPropabilityValues[i] = (float)originalIntensityValues[i] / totalNumberOfPixels;
  }

  float temp = 0;
  for(int i=0; i<=255; i++){
    temp = temp + originalPropabilityValues[i];
    originalCummulativePropabilityValues[i] = temp;
  }

  int improvedIntensityValues[255];

  for(int i=0; i<=255; i++){
    improvedIntensityValues[i] = 0;
  }

  for(int i=0; i<=255; i++){
    originalCummulativePropabilityValues[i] *= 255;
    improvedIntensityValues[i] = (int)(originalCummulativePropabilityValues[i]);
  }


  for(int i=0; i<modifiedSample.rows; i++){
    for(int j=0; j<modifiedSample.cols; j++){
      modifiedSample.at<uint8_t>(i,j) = improvedIntensityValues[originalSample.at<uint8_t>(i,j)];
    }
  }

  int modifiedMaxIntensity = 0;

  //pixel intensity counting for Histogram
  for(int i=0; i< modifiedSample.rows; i++){
    for(int j=0; j< modifiedSample.cols; j++){
      modifiedIntensityValues[modifiedSample.at<uint8_t>(i,j)]++;
      if(modifiedMaxIntensity < modifiedIntensityValues[modifiedSample.at<uint8_t>(i,j)]){
        modifiedMaxIntensity = modifiedIntensityValues[modifiedSample.at<uint8_t>(i,j)];
      }
    }
  }

  float scale1 = 300.00/(float)originalMaxIntensity;
  float scale2 = 300.00/(float)modifiedMaxIntensity;

  //histogram image values storing
  Mat originalHistogram(originalMaxIntensity*scale1, 255, CV_32F);
  Mat modifiedHistogram(modifiedMaxIntensity*scale2, 255, CV_32F);
  originalHistogram.setTo(255);
  modifiedHistogram.setTo(255);

  //drawing the histograms on the screen
  for(int i=0; i<=255; i++){
    Point p1,p2;
    p1 = Point(i, originalMaxIntensity*scale1);
    p2 = Point(i, (originalMaxIntensity - originalIntensityValues[i])*scale1);
    line(originalHistogram, p1, p2, Scalar(0, 0, 0), CV_AA);
  }

  //histogram plotting for the negitive image
  for(int i=0; i<=255; i++){
    Point p1,p2;
    p1 = Point(i, modifiedMaxIntensity*scale2 );
    p2 = Point(i, (modifiedMaxIntensity - modifiedIntensityValues[i])*scale2);
    line(modifiedHistogram, p1, p2, Scalar(0, 0, 0), CV_AA);
  }



  namedWindow("original Image");
  namedWindow("modified Image");
  namedWindow("original Histogram");
  namedWindow("modified Histogram");

  imshow("original Image", originalSample);
  imshow("modified Image", modifiedSample);
  imshow("original Histogram", originalHistogram);
  imshow("modified Histogram", modifiedHistogram);


  waitKey(0);

  //destory the windows
  destroyWindow("original Image");
  destroyWindow("modified Image");
  destroyWindow("original Histogram");
  destroyWindow("modified Histogram");

}
