#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#define M_TWO_PI 6.28318530718
#if defined(_MSC_VER)
#define popen _popen
#endif
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>
class Texture{
public:
/** from 0 to 1 **/
   double opacity, reflection, ambient;
   Texture(double am, double op, double ref);
   virtual void getColor(unsigned char* toFill, double* am, double *opacity, double *reflection,double x, double y) = 0;
   Texture* clone();
};

 double interpolate(double a,double b,double x);
 
const char* findExtension(const char* s);

double ground(double e);

double fix(double a);

char lowerCase(char c);

int fpeek(FILE* f);
bool extensionEquals(const char* a, const char* knownExt);
#endif
