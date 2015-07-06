#ifndef __DISK_H__
#define __DISK_H__
#include "plane.h"

class Disk : public Plane{
public:
  Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx);
  double getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3);
  bool getLightIntersection(Ray ray, double* fill);
};

#endif
