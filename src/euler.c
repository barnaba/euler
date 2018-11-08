#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULT_STEP 20

const double constR = 30; // 30 Ohm
const double constL = 0.02; // 20mH
const double constC = 0.000002; // 2uF
const double startIl = 0.0;
const double startUc = 0.0;
const double tMax = 0.01;

double e1(double t){
  return 1.0;
}

double edigital(double t){
  while(t-0.005 > 0)
    t -= 0.005;
  return t<0.0025;
}

double esin50(double t){
  return sin(2*M_PI*50*t);
}

double esin796(double t){
  return sin(2*M_PI*796*t);
}

double esin900(double t){
  return sin(2*M_PI*900*t);
}

typedef struct {
  double (*e)(double t);
  char name[10];
} Generator;

const Generator generators[5] = {
  (Generator){.e = e1, .name="1v"},
  (Generator){.e = esin50, .name="sin_f=50"},
  (Generator){.e = esin796, .name="sin_f=796"},
  (Generator){.e = esin900, .name="sin_f=900"},
  (Generator){.e = edigital, .name="digital"}
};

double ucSlope(double il){
  // 1/C * il
  return (il / constC);
}

double ilSlope(double e, double il, double uc){
  // 1/L(E - R * il - uC)

  //those are generally low
  // 0 < e(t) < 1
  // abs(il) doesn't seem to go over 0.1, so * constR it's under 2
  // abs(uc) can go up to maybe 4
  return (e - il * constR - uc)/constL;
}

int simulate(int steppow, FILE *plot, Generator g){
    double im1,im2,um1,um2;
    double t;

    double prevIl = startIl;
    double prevUc = startUc;

    double step = powf(0.5, steppow);
    printf("step: %g\n", step);
    fflush(stdout);
    // m1 = f(xi,yi)
    // m2 =...
    // just see https://i.imgur.com/qhWvFZ1.png

    //skip i=0 cause we already have prevIl and prevUc from back then

    for(int i=1; (t=i*step) < tMax; ++i){
      um1 = ucSlope(prevIl);
      im1 = ilSlope(g.e(t),prevIl,prevUc);

      um2 = ucSlope(prevIl + step * im1);
      im2 = ilSlope(g.e(t+step), prevIl + step * im1, prevUc + step * um1);

      prevIl = prevIl + step * ((im1 + im2)/2);
      prevUc = prevUc + step * ((um1 + um2)/2);
      fprintf(plot, "%f %f %f %f\n", t, prevIl, prevUc, g.e(t));
    }

    return 0;
}

int main(int argc, char ** argv){
  FILE *plot;
  char filepath[32];
  int steppow = (argc < 2 ? DEFAULT_STEP : atoi(argv[1]));

  for (int i=0; i<5; ++i){
    Generator g = generators[i];

    snprintf(filepath, sizeof(filepath[0]) * sizeof(filepath), "./%s%s", g.name, ".dat");

    plot = fopen(filepath, "w");
    assert(plot != NULL);
    printf("file : %s\n", filepath);

    simulate(steppow, plot, g);

    fclose(plot);
  }


  return 0;
}
