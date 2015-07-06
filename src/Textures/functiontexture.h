#ifndef __FUNC_TEXTURE_H__
#define __FUNC_TEXTURE_H__
#include "texture.h"
class FunctionTexture:public Texture{
public:
   void (*func)(unsigned char*,double*,double*, double*, double, double);
  FunctionTexture(void (*fun)(unsigned char*, double*, double*, double*,double, double));
   void getColor(unsigned char* toFill, double* am, double *op, double *ref,double x, double y);
   unsigned char constant();
};

#endif