#include "plane.h"

Plane::Plane(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty) : Shape(c, t, ya, pi, ro), vect(c), right(c), up(c){
   textureX = tx; textureY = ty;
   setAngles(yaw, pitch, roll);
   normalMap = '\0';
   mapX = textureX; mapY = textureY;
}

void Plane::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
   d = -vect.dot(center);
}

void Plane::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
   
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;   
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
   d = -vect.dot(center);
}

void Plane::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   d = -vect.dot(center);
}

void Plane::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
//   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
  // up.z = -xcos*ysin;
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   //right.z = -xsin;
   d = -vect.dot(center);
}

double Plane::getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   *data2 = inf;
   return (r>0)?r:inf;
}

bool Plane::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   if(texture->constant()){
      if(texture->opacity>1-1E-6) return true;
      unsigned char temp[4];
      double amb, op, ref;
      texture->getColor(temp, &amb, &op, &ref, 0.,0.);
      if(op>1-1E-6) return true;
      fill[0]*=temp[0]/255.;
      fill[1]*=temp[1]/255.;
      fill[2]*=temp[2]/255.;
      return false;
   }
    else{   
      //if(texture->opacity>1-1E-6) return true;   
      Vector dist = solveScalers(right, up, vect, ray.point-center);
      unsigned char temp[4];
      double amb, op, ref;
      texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
      if(op>1-1E-6) return true;
      fill[0]*=temp[0]/255.;
      fill[1]*=temp[1]/255.;
      fill[2]*=temp[2]/255.;
      return false;
    }
}
/*
bool Plane::getLightIntersection(Ray ray, double* toFill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || t>=1.) return false;
   if(texture->constant()){
    unsigned char a[3];
    double o, r, g
   }
   *data2 = inf;
   return (r>0)?r:inf;
}
*/
void Plane::move(){
   d = -vect.dot(center);
}
void Plane::getColor(unsigned char* toFill,double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth, unsigned int* data1, double* data2, double* data3){
   if(*data2==inf)
   {
      Vector dist = solveScalers(right, up, vect, ray.point-center);
      *data2=dist.x;
      *data3=dist.y;
   }
   texture->getColor(toFill, am, op, ref, fix(*data2/textureX-.5), fix(*data3/textureY-.5));
}
unsigned char Plane::reversible(unsigned int* data1, double* data2, double* data3){ 
   return 1; }

Vector Plane::getNormal(Vector point, unsigned int* data1, double* data2, double* data3){
   if(normalMap=='\0')
      return vect;
   else{
      if(*data2==inf)
      {
         Vector dist = solveScalers(right, up, vect, point-center);
         *data2=dist.x;
         *data3=dist.y;
   //      printf("%f %f\n", *data2, *data3);
      }
      double am, ref, op;
      unsigned char norm[3];
      normalMap->getColor(norm, &am, &op, &ref, fix(*data2/mapX-.5+mapOffX), fix(*data3/mapY-.5+mapOffY));
      Vector ret = ((norm[0]-128)*right+(norm[1]-128)*up+norm[2]*vect).normalize();
   //   printf("%f %f %f\n", ret.x, ret.y, ret.z);
      return ret;
   }
}