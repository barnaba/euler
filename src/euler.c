#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "generator.h"

const int defaultStepPow = 20;

const double constR = 30; // 30 Ohm
const double constL = 0.02; // 20mH
const double constC = 0.000002; // 2uF
const double startIl = 0.0;
const double startUc = 0.0;
const double tMax = 0.01;


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
    int i;
    int steps = pow(2, steppow) + 1;

    double im1,im2,um1,um2;
    double t;
    double e = 0.0;

    double prevIl = startIl;
    double prevUc = startUc;

    double step = tMax / steps;

    printf("step: %g\n", step);
    //skip i=0 cause we already have prevIl and prevUc from back then
    //we can do this cause power in t0 is also 0
    //but lets calculate it for clarity

    double power = prevIl * g.e(0);

    for(i=1; i < steps; ++i){
      // m1 = f(xi,yi)
      // m2 =...
      // just see https://i.imgur.com/qhWvFZ1.png
      t = i*step;

      um1 = ucSlope(prevIl);
      im1 = ilSlope(g.e(t),prevIl,prevUc);

      um2 = ucSlope(prevIl + step * im1);
      im2 = ilSlope(g.e(t+step), prevIl + step * im1, prevUc + step * um1);

      prevIl = prevIl + step * ((im1 + im2)/2);
      prevUc = prevUc + step * ((um1 + um2)/2);
      fprintf(plot, "%f %f %f %f\n", t, prevIl, prevUc, g.e(t));

    // i: 0      -> *1 - skipped
    // i: odd    -> *2
    // i: even   -> *4
      power += (2+((i+1)%2*2)) * prevIl * g.e(t+step) ;
    }

    //have to care about last term though

    //last step should be even for that to not introduce an error
    //so last step i was even, we did i++, and is uneven now

    //lets just check
    assert((i%2)==1);

    // so we kinda already did the last step, but it was *4, so removing 3 of those times
    // this is not the cleanest way to do things perhaps
    power -= 3 * prevIl * g.e(t+step);
    power *= step/3;

    printf("power: %g after %d steps\n", power, i-1);
    fflush(stdout);


    return 0;
}

int main(int argc, char ** argv){
  Generator * gens = generators();
  FILE *plot;
  char filepath[32];
  int steppow = (argc < 2 ? defaultStepPow : atoi(argv[1]));

  for (int i=0; i<5; ++i){
    Generator g = gens[i];

    snprintf(filepath, sizeof(filepath[0]) * sizeof(filepath), "./%s%s", g.name, ".dat");

    plot = fopen(filepath, "w");
    assert(plot != NULL);
    printf("file : %s\n", filepath);

    simulate(steppow, plot, g);

    fclose(plot);
  }


  return 0;
}
