#include "mandeltexture.h"
MandelTexture::MandelTexture(double xx, double yy, double w, double h, unsigned int a):Texture(.3, 1.,0.){
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

void colorBlue(unsigned int iteration, double x, double y, double x2, double y2, unsigned char* colors, unsigned int escape){
   if(iteration>=escape){
      colors[0] = '\0';
      colors[1] = '\0';
      colors[2] = '\0';
   
   }
   else{
      double value = 1-(iteration +1 -log(log(x2+y2))/0.6931471805599453)/escape;
      value*= value;
      value = 1-value;
      colors[0] = (unsigned char)((short)(value*255));
      colors[1] = colors[0];
      colors[2] = (unsigned char)((short)(255*(.4+3*value/5)));
   }
}


void MandelTexture::getColor(unsigned char* toFill, double* am, double *op, double *ref,double mx, double my){
mx=(mx-.5)*width+xs;
my=(my-.5)*height+ys;
   double x = mx;
   double y=my;
   double y2 = y*y;
   double x2 = x*x;
   unsigned int iteration = 0;
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
   }while ( (x2 = x*x) + (y2 = y*y) < 4  &&  iteration < escape );
      colorBlue(iteration, x, y, x2, y2, toFill, escape);
      *op = opacity;
      *ref = reflection;
   *am = ambient;
}
unsigned char MandelTexture::constant(){ 
   return 0; }

