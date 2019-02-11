// This header file will include all the other necessary header files for your application
#include <opencv2/opencv.hpp>
#include <stdint.h>
using namespace cv;


#include<iostream>
using namespace std;

int main(int argc, char** argv)
{

	// Reading the image file from the main argument
 	Mat originalSample = imread(argv[1], IMREAD_GRAYSCALE);
  Mat negitiveSample = imread(argv[1], IMREAD_GRAYSCALE);

  uint8_t originalValues[255];
  uint8_t negitiveValues[255];

  for(int i=0; i<=255; i++){
    originalValues[i] = 0;
    negitiveValues[i] = 0;
  }


	//Check that the images loaded
	if(!originalSample.data ) {
		cout << "ERROR: Could not load image data." << endl;
		return -1;
	}

  // Changing the original image to negitive image by inverting the greyscale values
  for(int i=0; i<negitiveSample.rows; i++){
    for(int j=0; j<negitiveSample.cols; j++){
      negitiveSample.at<uint8_t>(i,j) = 255 - negitiveSample.at<uint8_t>(i,j);
    }
  }

  int negitiveMaxIntensity = 0 ;
  //pixel intensity counting for originalHistogram
  for(int i=0; i<negitiveSample.rows; i++){
    for(int j=0; j<negitiveSample.cols; j++){
      negitiveValues[negitiveSample.at<uint8_t>(i,j)]++;
      if(negitiveMaxIntensity < negitiveValues[negitiveSample.at<uint8_t>(i,j)]){
        negitiveMaxIntensity = negitiveValues[negitiveSample.at<uint8_t>(i,j)];
      }
    }
  }

  int originalMaxIntensity = 0;
  //pixel intensity counting for negitiveHistogram
  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){
      originalValues[originalSample.at<uint8_t>(i,j)]++ ;
      if(originalMaxIntensity < originalValues[originalSample.at<uint8_t>(i,j)]){
        originalMaxIntensity = originalValues[originalSample.at<uint8_t>(i,j)];
      }
    }
  }

	//Create the display windows
  namedWindow("original Image");
	namedWindow("negitive Image");
  namedWindow("transformation curve");
  namedWindow("original Histogram", CV_WINDOW_AUTOSIZE);
  namedWindow("negitive Histogram", CV_WINDOW_AUTOSIZE);

  // Dispaly the the images in the windows
  imshow("original Image", originalSample);
  imshow("negitive Image", negitiveSample);

  float scale1 = 300.00/(float)originalMaxIntensity;
  float scale2 = 300.00/(float)negitiveMaxIntensity;

  //histogram image values storing
  Mat originalHistogram(originalMaxIntensity*scale1, 255, CV_32F);
  Mat negitiveHistogram(negitiveMaxIntensity*scale2, 255, CV_32F);
  originalHistogram.setTo(255);
  negitiveHistogram.setTo(255);

  //drawing the histograms on the screen
  for(int i=0; i<=255; i++){
    Point p1,p2;
    p1 = Point(i, originalMaxIntensity*scale1);
    p2 = Point(i, (originalMaxIntensity - originalValues[i])*scale1);
    line(originalHistogram, p1, p2, Scalar(0, 0, 0), CV_AA);
  }

  //histogram plotting for the negitive image
  for(int i=0; i<=255; i++){
    Point p1,p2;
    p1 = Point(i, negitiveMaxIntensity*scale2 );
    p2 = Point(i, (negitiveMaxIntensity - negitiveValues[i])*scale2);
    line(negitiveHistogram, p1, p2, Scalar(0, 0, 0), CV_AA);
  }

  //dispalying the histograms
  imshow("original Histogram", originalHistogram);
  imshow("negitive Histogram", negitiveHistogram);

  //transformation Curve plotting image
  Mat transformationCurve( 255, 255, CV_32F);
  transformationCurve.setTo(255);

  //drawing the curve on the image
  for(int i=0; i<negitiveSample.rows; i++){
    for(int j=0; j<negitiveSample.cols; j++){
       transformationCurve.at<uint8_t>(negitiveSample.at<uint8_t>(i,j), 255 - originalSample.at<uint8_t>(i,j)) = 0;
    }
  }

  //showing the transformation curve on the dispaly
  imshow("transformation curve", transformationCurve);


  //Wait for any key to press in window
  waitKey(0);

  //destory the windows
  destroyWindow("original Image");
  destroyWindow("negitive Image");
  destroyWindow("original Histogram");
  destroyWindow("negitive Histogram");
  destroyWindow("transformation curve");


}
