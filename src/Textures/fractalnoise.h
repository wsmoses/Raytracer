#ifndef __FRACTAL_NOISE_H__
#define __FRACTAL_NOISE_H__
#include "texture.h"

class FractalNoise: public Texture{
public:
  double width, height, x, y, evo;
  FractalNoise(double a, double b, double c, double d, double e);
  void getColor(unsigned char* toFill, double* amb, double *op, double *ref, double x, double y);
  unsigned char constant();
};

#endif
