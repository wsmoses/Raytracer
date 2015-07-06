#include "camera.h"

Camera::Camera(const Vector & cente) : focus(cente), right(cente), up(cente), forward(cente){
   setAngles(0, 0, 0);
}

Camera::Camera(const Vector &f, double a, double b, double c) : focus(f), right(f), up(f), forward(f){
   setAngles(a, b, c);
}

void Camera::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
   
   forward.x = xsin*ycos*zcos+ysin*zsin;
   forward.y = ysin*zcos-xsin*ycos*zsin;
   forward.z = xcos*ycos;
   
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
}

void Camera::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
  
   forward.x = xsin*ycos*zcos+ysin*zsin;
   forward.y = ysin*zcos-xsin*ycos*zsin;
   forward.z = xcos*ycos;
   
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
}

void Camera::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
 
   forward.x = xsin*ycos*zcos+ysin*zsin;
   forward.y = ysin*zcos-xsin*ycos*zsin;
   forward.z = xcos*ycos;
   
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
}

void Camera::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
   
   forward.x = xsin*ycos*zcos+ysin*zsin;
   forward.y = ysin*zcos-xsin*ycos*zsin;
   
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
}

