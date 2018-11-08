#ifndef GENERATOR_H
#define GENERATOR_H

typedef struct {
  double (*e)(double t);
  char name[10];
} Generator;

Generator * generators();

#endif
