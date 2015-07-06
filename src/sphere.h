#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "shape.h"

class Sphere : public Shape{
public:
  double radius;
  Sphere(const Vector &c, Texture* t, double ya, double pi, double ro, double radius);
  double getIntersection(Ray ray,unsigned int* data1, double* data2, double* data3);
  void move();
  bool getLightIntersection(Ray ray, double* fill);
  void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth,unsigned int* data1, double* data2, double* data3);
  Vector getNormal(Vector point,unsigned int* data1, double* data2, double* data3);
  unsigned char reversible(unsigned int* data1, double* data2, double* data3);
  void setAngles(double a, double b, double c);
  void setYaw(double a);
  void setPitch(double b);
  void setRoll(double c);
};
#endif
