#ifndef __PLANE_H__
#define __PLANE_H__

#include "shape.h"

class Plane : public Shape{
public:
  Vector vect, right, up;
  double d;
  Plane(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  double getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3);
  bool getLightIntersection(Ray ray, double* toFill);
  void move();
  void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth, unsigned int* data1, double* data2, double* data3);
  Vector getNormal(Vector point, unsigned int* data1, double* data2, double* data3);
  unsigned char reversible(unsigned int* data1, double* data2, double* data3);
  void setAngles(double yaw, double pitch, double roll);
  void setYaw(double d);
  void setPitch(double d);
  void setRoll(double d);
};

#endif
