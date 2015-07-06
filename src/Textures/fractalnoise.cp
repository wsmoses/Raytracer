#include "fractalnoise.h"

FractalNoise::FractalNoise(double xx, double yy, double w, double h, unsigned int a):Texture(.3, 1.,0.){
 height = h;
 width = w;
 ys = yy;
 xs = xx;
 escape = a;
}
MandelTexture::MandelTexture(unsigned int a):Texture(.3, 1.,0.){
 height = 2.;
 width = 3.5;
 ys = 0.;
 xs = -.75;
 escape = a;
}
MandelTexture::MandelTexture():Texture(.3, 1.,0.){
 height = 2.;
 width = 3.5;
 ys = 0.;
 xs = -.75;
 escape = 300;
}

/** output -1,1*/
double findnoise2(double x,double y)
{
 int n=(int)x+(int)y*57;
 n=(n<<13)^n;
 int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
 return 1.0-((double)nn/1073741824.0);
}

double noise(double x,double y)
{
 double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
 double floory=(double)((int)y);
 double s,t,u,v;//Integer declaration
 s=findnoise2(floorx,floory);
 t=findnoise2(floorx+1,floory);
 u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
 v=findnoise2(floorx+1,floory+1);
 double int1=interpolate1(s,t,x-floorx);//Interpolate between the values.
 double int2=interpolate1(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
 return interpolate1(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}
