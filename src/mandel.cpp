#include "mandel.h"
Mandelbrot::Mandelbrot(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty, unsigned int es) : Box(c, t, ya, pi, ro, tx, ty){
   escape = es;
}


double Mandelbrot::getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3){
   double time = Box::getIntersection(ray, data1, data2, data3);
   if(time==inf) 
      return time;
   else{
   
      int iteration = 0;
      double mx = *data2/textureX*3.5-.75;
      double my = *data3/textureY*2;
      double x = mx;
      double y=my;
      double y2 = y*y;
      double x2 = x*x;
      do
      {
         y = 2*x*y + my;
         x = x2-y2+mx;
         y2 = y*y;
         y = 2*x*y + my;
         x = x*x-y2+mx;
         y2 = y*y;
         y = 2*x*y + my;
         x = x*x-y2+mx;
         y2 = y*y;
         y = 2*x*y + my;
         x = x*x-y2+mx;
         iteration+=4;
      }while ( (x2 = x*x) + (y2 = y*y) < 4  &&  iteration < 16 );
      return(iteration<=12)?inf:time;
   }
}

bool Mandelbrot::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*r-center);
   
   int iteration = 0;
  double mx = dist.x/textureX*3.5-.75;
  double my = dist.y/textureY*2;
  double x = mx;
  double y=my;
  double y2 = y*y;
  double x2 = x*x;
  do
  {
      y = 2*x*y + my;
      x = x2-y2+mx;
      y2 = y*y;
      y = 2*x*y + my;
      x = x*x-y2+mx;
      y2 = y*y;
      y = 2*x*y + my;
      x = x*x-y2+mx;
      y2 = y*y;
      y = 2*x*y + my;
      x = x*x-y2+mx;
      iteration+=4;
  }while ( (x2 = x*x) + (y2 = y*y) < 4  &&  iteration < 16 );
  if(iteration<=12) return false;
   
   
   
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
      if(texture->opacity>1-1E-6) return true;   
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