#ifndef __HYPERBOLOID_H__
#define __HYPREBOLOID_H__
#include "shape.h"

class Hyperboloid : public Shape{
public:
   double a, b, c;
   Hyperboloid(const Vector &c, Texture* t,double ya, double pi, double ro, double aa, double bb, double cc);
   double getIntersection(Ray ray,unsigned int* data1, double* data2, double* data3);
   void move();
   void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth,unsigned int* data1, double* data2, double* data3);
   Vector getNormal(Vector point,unsigned int* data1, double* data2, double* data3);
};


#endif
