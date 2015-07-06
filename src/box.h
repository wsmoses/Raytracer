#ifndef __BOX_H__
#define __BOX_H__
#include "plane.h"

class Box : public Plane{
public:
  Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx);
  double getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3);
  bool getLightIntersection(Ray ray, double* fill);
};

#endif
