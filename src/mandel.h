#ifndef __MANDEL_H__
#define __MANDEL_H__
#include "box.h"

class Mandelbrot : public Box{
public:
   unsigned int escape;
   Mandelbrot(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty, unsigned int es);
   double getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3);
   bool getLightIntersection(Ray ray, double* fill);
   };

#endif
