#include "colortexture.h"
ColorTexture::ColorTexture(unsigned char ra, unsigned char ga, unsigned char ba):Texture(.3, 1., 0.){
   r = ra; g = ga; b = ba;
}
ColorTexture::ColorTexture(unsigned char ra, unsigned char ga, unsigned char ba, double al):Texture(.3, al, 0.){
   r = ra; g = ga; b = ba;
}
ColorTexture::ColorTexture(unsigned char ra, unsigned char ga, unsigned char ba, double al, double ref):Texture(.3, al, ref){
   r = ra; g = ga; b = ba;
}
ColorTexture::ColorTexture(unsigned char ra, unsigned char ga, unsigned char ba, double al, double ref, double am):Texture(am, al, ref){
   r = ra; g = ga; b = ba;
}

ColorTexture::ColorTexture(char* def):Texture(.3,1.,0.){

}
void ColorTexture::getColor(unsigned char* toFill, double* amb, double *op, double *ref, double x, double y){
   toFill[0] = r; toFill[1] = g; toFill[2] = b;
   *op = opacity;
   *ref = reflection;
   *amb = ambient;
}