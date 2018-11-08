#include <math.h>
#include "generator.h"

static double e1(double t){
  return 1.0;
}

static double edigital(double t){
  while(t-0.005 > 0)
    t -= 0.005;
  return t<0.0025;
}

static double esin50(double t){
  return sin(2*M_PI*50*t);
}

static double esin796(double t){
  return sin(2*M_PI*796*t);
}

static double esin900(double t){
  return sin(2*M_PI*900*t);
}

Generator * generators(){
  static Generator generators[5] = {
    (Generator){.e = e1, .name="1v"},
    (Generator){.e = esin50, .name="sin_f=50"},
    (Generator){.e = esin796, .name="sin_f=796"},
    (Generator){.e = esin900, .name="sin_f=900"},
    (Generator){.e = edigital, .name="digital"}
  };
  return generators;
}
