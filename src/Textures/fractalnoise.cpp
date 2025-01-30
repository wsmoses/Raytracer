#include "fractalnoise.h"

FractalNoise::FractalNoise(double xx, double yy, double xs, double ys, double ev):Texture(.3, 1.,0.){
  width = xx;
 height = yy;
 x= xs;
 y = ys;
 evo = ev;
}

/** output 0,1*/
double findnoise(int x,int y)
{
 int n=x+y*57;
 n=(n<<13)^n;
 int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
 return ((double)nn/1073741824.0)/2;
}

double antialias(double x,double y)
{
 double floorx=floor(x);
 double floory=floor(y);
 double s,t,u,v;//Integer declaration
 s=findnoise(floorx,floory);
 t=findnoise(floorx+1,floory);
 u=findnoise(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
 v=findnoise(floorx+1,floory+1);
 double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
 double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
 return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

unsigned char FractalNoise::constant(){return 0; }

void FractalNoise::getColor(unsigned char* toFill, double* am, double *op, double *ref,double mx, double my){
  double ret = antialias(mx/width*10+x,my/height*10+y);
  toFill[0] = toFill[1] = toFill[2] = (unsigned char)(255*ret);
      *op = opacity;
      *ref = reflection;
   *am = ambient;
}
