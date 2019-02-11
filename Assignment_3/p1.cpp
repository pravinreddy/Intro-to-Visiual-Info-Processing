
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;


#define PI 3.14159265

int f[] ={10,11,12,11,12,13,12,11};
int g[] = {10,-10,8,-7,8,-8,7,-7};
int h[] = {10,11,12,11,12,13,12,11,10,-10,8,-7,8,-8,7,-7};
//int f[] ={100,100,100,100,100,100,100};
//int f[] ={2,4,5,3};

double calculateDct1( int u, int N) {
  double au;
  if(u==0){
    au = sqrt(1.00/N);
  } else {
    au = sqrt(2.00/N);
  }

  double sum =0;

  for(int x=0; x<N; x++) {
      sum =sum + f[x]*cos(((2*x+1)*u*PI)/(2*N));
  }
  return au*sum;
}

double calculateDct2( int u, int N) {
  double au;
  if(u==0){
    au = sqrt(1.00/N);
  } else {
    au = sqrt(2.00/N);
  }

  double sum =0;

  for(int x=0; x<N; x++) {
      sum =sum + g[x]*cos(((2*x+1)*u*PI)/(2*N));
  }
  return au*sum;
}

double calculateDct3( int u, int N) {
  double au;
  if(u==0){
    au = sqrt(1.00/N);
  } else {
    au = sqrt(2.00/N);
  }

  double sum =0;

  for(int x=0; x<N; x++) {
      sum =sum + h[x]*cos(((2*x+1)*u*PI)/(2*N));
  }
  return au*sum;
}

/** @function main */
int main( int argc, char** argv ) {
  int numberOfPoints = 8;
  double c1[numberOfPoints+1];
  double c2[numberOfPoints+1];
  double c3[17];


  for(int i=0; i<numberOfPoints; i++) {
    c1[i] = 0;
    c2[i] = 0;
  }


  for(int i=0; i<numberOfPoints; i++) {
      c1[i]  = calculateDct1(i,numberOfPoints);
  }

  for(int i=0; i<numberOfPoints; i++) {
      c2[i]  = calculateDct2(i,numberOfPoints);
  }

  for(int i=0; i<16; i++) {
      c3[i]  = calculateDct3(i,16);
  }


  for(int i=0; i<numberOfPoints; i++) {
      cout<<c1[i]<<"\t\t";
  }
  cout<<endl;

  for(int i=0; i<numberOfPoints; i++) {
    cout<<c2[i]<<"\t\t";
  }
  cout<<endl;

  for(int i=0; i<16; i++) {
    cout<<c3[i]<<"\t\t";
  }
  cout<<endl;

}
