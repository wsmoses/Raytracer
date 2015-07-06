#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "vector.h"

class Camera{
public:
   Vector focus, right, up, forward;
   double xcos, xsin, ycos, ysin, zcos, zsin;
   Camera(const Vector & cente);
  
   Camera(const Vector &f, double a, double b, double c);

   void setAngles(double a, double b, double c);
   void setYaw(double a);
   void setPitch(double a);
   void setRoll(double b);
private:
   double yaw, pitch, roll;
  
} ;

#endif
