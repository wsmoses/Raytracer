#include "sphere.h"

Sphere::Sphere(const Vector &c, Texture* t, double ya, double pi, double ro, double rad): Shape(c, t, ya, pi, ro){
  textureX = textureY = 1.;
  normalMap = NULL;
   radius = rad;
}
bool Sphere::getLightIntersection(Ray ray, double* fill){
   const double A = ray.vector.mag2();
   const double B = 2*ray.vector.dot(ray.point-center);
   const double C = (ray.point-center).mag2()-radius*radius;
   const double descriminant = B*B-4*A*C;
   if(descriminant<0. || descriminant<B*((B>=0)?B:-B)) return false;
   
      const double desc = sqrt(descriminant);
      const double root1 = (-B-desc)/(2*A);
      const double root2 = (-B+desc)/(2*A);
   const double time = (root1>0)?root1:root2;
   if(time>=1.) return false;
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
      Vector point = ray.point+ray.vector*time;
      double data2 = (center.y-point.y+radius)/(2*radius);
      double data3 = atan2( point.z-center.z, point.x-center.x);
      unsigned char temp[4];
      double amb, op, ref;
      texture->getColor(temp, &amb, &op, &ref,fix((yaw+data2)/M_TWO_PI/textureX),fix((pitch/M_TWO_PI-(data3)))/textureY);
      if(op>1-1E-6) return true;
      fill[0]*=temp[0]/255.;
      fill[1]*=temp[1]/255.;
      fill[2]*=temp[2]/255.;
      return false;
    }
}
double Sphere::getIntersection(Ray ray,unsigned int* data1, double* data2, double* data3){
   const double A = ray.vector.mag2();
   const double B = 2*ray.vector.dot(ray.point-center);
   const double C = (ray.point-center).mag2()-radius*radius;
   const double descriminant = B*B-4*A*C;
   *data2 = inf;
   if(descriminant<0) return inf;
   else{
      const double desc = sqrt(descriminant);
      const double root1 = (-B-desc)/(2*A);
      const double root2 = (-B+desc)/(2*A);
      return (root1>0)?(root1):((root2>0)?root2:inf);
   }
}
void Sphere::move(){
   return;
}
unsigned char Sphere::reversible(unsigned int* data1, double* data2, double* data3){return 0;}

void Sphere::getColor(unsigned char* toFill, double* amb, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth,unsigned int* data1, double* data2, double* data3){
     if(*data2==inf){
     *data3 = (center.y-ray.point.y+radius)/(2*radius);
     *data2 = atan2( ray.point.z-center.z, ray.point.x-center.x);
     }
     
   texture->getColor(toFill, amb, op, ref,fix((yaw+*data2)/M_TWO_PI/textureX),fix((pitch/M_TWO_PI-(*data3))/textureY));
}
Vector Sphere::getNormal(Vector point,unsigned int* data1, double* data2, double* data3){
   Vector vect = point-center;
/*   A x B = <x, y, z>
<ay bz- az by,  bz ax - az bx, ax by - bx ay>
az = 0

<ay bz,  bz ax, ax by - bx ay >

bx = 0

<ay bz,  bz ax, ax by >

ax = 1

<ay bz,  bz , by >

B: <0, z, -y>
A <-1,x/y,0>
*/
if(normalMap=='\0')
      return vect;
     if(*data2==inf){
     *data3 = (center.y-point.y+radius)/(2*radius);
     *data2 = atan2( point.z-center.z, point.x-center.x);
     }
     //TODO FIX THIS
     vect = vect.normalize();
     Vector right = Vector(vect.x, vect.z, -vect.y);
     Vector up = Vector(vect.z, vect.y, -vect.x);
      double am, ref, op;
      unsigned char norm[3];
      normalMap->getColor(norm, &am, &op, &ref, fix(((mapOffX+mapOffX)+*data2)/M_TWO_PI/mapX),fix(((mapOffY+mapOffY)/M_TWO_PI-(*data3))/mapY));
      return ((norm[0]-128)*right+(norm[1]-128)*up+norm[2]*vect).normalize();
}

void Sphere::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
}

void Sphere::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
}

void Sphere::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
}

void Sphere::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
}