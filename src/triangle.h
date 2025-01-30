#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "plane.h"

class Triangle : public Plane{
public:
   double thirdX;
   Triangle(Vector c, Vector b, Vector a, Texture* t);
   double getIntersection(Ray ray,unsigned int* data1, double* data2, double* data3);
   bool getLightIntersection(Ray ray, double* fill);
};

#endif
