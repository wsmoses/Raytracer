#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

struct LightNode{
   Light* data;
   LightNode* prev, *next;
};

class Shape;
struct ShapeNode{
   Shape* data;
   ShapeNode* prev, *next;
};

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;
   ShapeNode *listStart, *listEnd;
   LightNode *lightStart, *lightEnd;
   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(ShapeNode* s);
   void addLight(Light* s);
   void removeLight(LightNode* s);
};

class Shape{
  public:
   Shape(const Vector &c, Texture* t, double ya, double pi, double ro);
   double yaw, pitch, roll, xsin, xcos, ysin, ycos, zsin, zcos;
   Vector center;
   Texture* texture;
   double textureX, textureY, mapX, mapY, mapOffX, mapOffY;
   Texture* normalMap;
   virtual double getIntersection(Ray ray) = 0;
   virtual bool getLightIntersection(Ray ray, double* fill) = 0;
   virtual void move() = 0;
   virtual unsigned char reversible() = 0;
   virtual void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth) = 0;
   virtual Vector getNormal(Vector point) = 0;
   virtual void setAngles(double yaw, double pitch, double roll) = 0;
   virtual void setYaw(double d) = 0;
   virtual void setPitch(double d) = 0;
   virtual void setRoll(double d) = 0;
};

void getLight(double* toFill, Autonoma* aut, Vector point, Vector norm, unsigned char r);

#endif
