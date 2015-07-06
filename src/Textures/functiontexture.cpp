#include "functiontexture.h"
FunctionTexture::FunctionTexture(void (*fun)(unsigned char*,double*,double*, double*,double, double)):Texture(.3, 0.,0.){
func = fun;
}

void FunctionTexture::getColor(unsigned char* toFill, double* amb, double* op, double *ref,double x, double y){
func(toFill, amb, op, ref,x, y);
}

unsigned char FunctionTexture::constant(){ return 0; }