#ifndef __MANDEL_TEXTURE_H__
#define __MANDEL_TEXTURE_H__
#include "texture.h"
class MandelTexture:public Texture{
public:
   unsigned int escape;
   double width, height, xs, ys;
   MandelTexture(double xx, double yy, double w, double h, unsigned int a);
   MandelTexture(unsigned int a);
   MandelTexture();
   void getColor(unsigned char* toFill, double* am, double *op, double *ref,double x, double y);
   unsigned char constant();
};

#endif
