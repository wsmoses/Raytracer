#ifndef __MESH_H__
#define __MESH_H__

#include "shape.h"
#include "triangle.h"
class Mesh : public Shape{
public:
   double textureX, textureY;
   Shape** shapes;
   unsigned int polyCount;
   Vector boundCenter;
   double radius;
   Mesh(Vector offset, Vector* points, unsigned int pointCount, unsigned int* polys, unsigned int polyCountt, Texture* t);
   Mesh(Vector offset, char* pointL, unsigned int pointCount, char* polyL, unsigned int polyCountt, Texture* t);
   double getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3);
   void move();
   void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth, unsigned int *data1, double *data2, double *data3);
   Vector getNormal(Vector point, unsigned int* data1, double* data2, double* data3);
   unsigned char reversible(unsigned int* data1, double* data2, double* data3);
   void setAngles(double yaw, double pitch, double roll);
   void setYaw(double d);
   void setPitch(double d);
   void setRoll(double d);
   void refreshBounds(Vector* points, Vector offset, unsigned int length) ;
   bool getLightIntersection(Ray ray, double* fill);
};

#endif
