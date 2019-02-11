#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

using namespace cv;
#define PI 3.14159265

float bin[1920][1080];
//float cir[1920][1080][1000];



//the below hsi convert code is written with reference of "http://answers.opencv.org/question/62446/conversion-from-rgb-to-hsi/"
void rgbToHsi(Mat &originalSample, Mat &intensitySample) {
  for(int i=0; i<originalSample.rows; i++){
    for(int j=0; j<originalSample.cols; j++){

      float b = originalSample.at<Vec3b>(i, j)[0];
      float g = originalSample.at<Vec3b>(i, j)[1];
      float r = originalSample.at<Vec3b>(i, j)[2];
      float h,s,in;

      in = (b + g + r) / 3;

      int minValue = 0;
      minValue = min(r, min(b,g));

      s = 1 - 3*(minValue/(b+g+r));

      if(s<0.00001) {
        s =0;
      } else if(s>0.99999) {
        s=1;
      }

      if(s != 0) {
        h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
        h = acos(h);

        if(b <= g) {
          h = h;
        } else{
          h = ((360 * 3.14159265) / 180.0) - h;
        }
      }

      intensitySample.at<Vec3b>(i, j)[0] = (h * 180) / 3.14159265;
      intensitySample.at<Vec3b>(i, j)[1] = s*100;
      intensitySample.at<Vec3b>(i, j)[2] = in;
    }
  }
}

void hough_stright(Mat &sample, Mat &binSample) {
  int max_r = sqrt((sample.rows/2)*(sample.rows/2) + (sample.cols/2)*(sample.cols/2));

  for(int i=0; i<1920; i++) {
    for(int j=0; j<1080; j++) {
      bin[i][j] = 0;
    }
  }

  int max_height=0;

  int x = sample.rows/2;
  int y = sample.cols/2;

  for(int i=0; i<sample.rows; i++){
    for(int j=0; j<sample.cols; j++){
      if((int)sample.at<uint8_t>(i, j) == 255) {
        for(int k=0; k<=180;k++) {
          int r = (i - x)*cos(k) + (j-y)*sin(k);
          bin[r+max_r][k]++;
          if((r+max_r)>max_height) {
            max_height = r+max_r;
          }
        }
      }
    }
  }



  for(int i=0; i<sample.rows; i++){
    for(int j=0; j<sample.cols; j++){
      if((int)sample.at<uint8_t>(i, j) == 255) {
        for(int k=0; k<=180;k++) {
          int r = (i - x)*cos(k) + (j-y)*sin(k);
          if ( bin[r+max_r][k] > 100){
            binSample.at<Vec3b>(i, j)[0] = 255;
          }
        }
      }
    }
  }

}

void hough_circle(Mat &sample, Mat &binSample) {
  int max_r = sqrt((sample.rows/2)*(sample.rows/2) + (sample.cols/2)*(sample.cols/2));

  /*for(int i=0; i<1920; i++) {
    for(int j=0; j<1080; j++) {
      for(int k=0; k<1000; k++) {
        cir[i][j][k] = 0;
      }
    }
  }



  for(int i=0; i<sample.rows; i++){
    for(int j=0; j<sample.cols; j++){
      if((int)sample.at<uint8_t>(i, j) == 255) {
        for(int k=0; k<=max_r;k++) {
          for(int l=0; l<360; l++) {
            int a = i - k*cos(l);
            int b = j + k*sin(l);
            //cir[a][b][k]++;
          }
        }
      }
    }
  }



  for(int i=0; i<sample.rows; i++){
    for(int j=0; j<sample.cols; j++){
      if((int)sample.at<uint8_t>(i, j) == 255) {
        for(int k=0; k<=360;k++) {
          int r = (i - x)*cos(k) + (j-y)*sin(k);
          if ( bin[r+max_r][k] > 100){
            binSample.at<Vec3b>(i, j)[0] = 255;
          }
        }
      }
    }
  }*/

}



int main (int argc, char **argv) {

  // Reading the image file from the main argument
  Mat originalSample = imread (argv[1], CV_LOAD_IMAGE_COLOR);
  Mat hsiSample = imread (argv[1], CV_LOAD_IMAGE_COLOR);
  Mat modifiedSample;
  Mat binSample = imread (argv[1], CV_LOAD_IMAGE_COLOR);

  Mat originalSample2 = imread (argv[2], CV_LOAD_IMAGE_COLOR);
  Mat hsiSample2 = imread (argv[2], CV_LOAD_IMAGE_COLOR);
  Mat modifiedSample2;
  Mat cirSample = imread (argv[2], CV_LOAD_IMAGE_COLOR);


  //Check that the images loaded
  if (!originalSample.data)
    {
      cout << "ERROR: Could not load image data." << endl;
      return -1;
    }

  rgbToHsi(originalSample,hsiSample);

  rgbToHsi(originalSample2,hsiSample2);

  Canny( hsiSample, modifiedSample, 100, 200, 3 );

  Canny( hsiSample2, modifiedSample2, 100, 200, 3 );

  hough_stright(modifiedSample, binSample);

  hough_circle(modifiedSample2, cirSample);


  imshow ("original Image", originalSample);
  imshow ("Hsi Image", hsiSample);
  imshow ("edge Image", modifiedSample);
  imshow ("final Image", binSample);

  imshow ("original Image2", originalSample2);
  imshow ("Hsi Image2", hsiSample2);
  imshow ("edge Image2", modifiedSample2);
  imshow ("final Image2", cirSample);

  waitKey (0);

  //destory the windows
  destroyWindow ("original Image");
  destroyWindow ("Hsi Image");
  destroyWindow ("edge Image");
  destroyWindow ("final Image");
  destroyWindow ("original Image2");
  destroyWindow ("Hsi Image2");
  destroyWindow ("edge Image2");
  destroyWindow ("final Image2");


}
