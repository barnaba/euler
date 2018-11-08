#include <math.h>
#include <stdio.h>
#include <assert.h>

const double constR = 30; // 30 Ohm
const double constL = 0.02; // 20mH
const double constC = 0.000002; // 2uF
const double startIl = 0.0;
const double startUc = 0.0;
const double step = 0.00000001;
const double tMax = 0.01;

double e(double t){
  //return 1.0;
  //return sin(2*M_PI*900*t);
  while(t-0.005 > 0)
    t -= 0.005;
  return t<0.0025;
}

double ucSlope(double il){
  // 1/C * il
  return (il / constC);
}

double ilSlope(double t, double il, double uc){
  // 1/L(E - R * il - uC)

  //those are generally low
  // 0 < e(t) < 1
  // abs(il) doesn't seem to go over 0.1, so * constR it's under 2
  // abs(uc) can go up to maybe 4
  return (e(t) - il * constR - uc)/constL;
}

int main(int argc, char ** argv){

  double im1,im2,um1,um2;
  double t;

  double prevIl = startIl;
  double prevUc = startUc;

  // m1 = f(xi,yi)
  // m2 =...
  // just see https://i.imgur.com/qhWvFZ1.png

  //skip i=0 cause we already have prevIl and prevUc from back then
  for(int i=1; (t=i*step) < tMax; ++i){
    um1 = ucSlope(prevIl);
    im1 = ilSlope(t,prevIl,prevUc);

    um2 = ucSlope(prevIl + step * im1);
    im2 = ilSlope(t+step, prevIl + step * im1, prevUc + step * um1);

    prevIl = prevIl + step * ((im1 + im2)/2);
    prevUc = prevUc + step * ((um1 + um2)/2);
    printf("%f %f %f %f\n", t, prevIl, prevUc, e(t));
  }


  return 0;
}
