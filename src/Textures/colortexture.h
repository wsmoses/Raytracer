#ifndef __COLOR_TEXTURE_H__
#define __COLOR_TEXTURE_H__
#include "texture.h"
#define WHITE new ColorTexture(255, 255, 255)
#define LIGHT_GRAY new ColorTexture(192, 192, 192)
#define GRAY new ColorTexture(128, 128, 128)
#define DARK_GRAY new ColorTexture(64, 64, 64)
#define BLACK new ColorTexture(0, 0, 0)
#define RED new ColorTexture(255, 0, 0)
#define PINK new ColorTexture(255, 175, 175)
#define ORANGE new ColorTexture(255, 200, 0)
#define YELLOW new ColorTexture(255, 255, 0)
#define GREEN new ColorTexture(0, 255, 0)
#define MAGENTA new ColorTexture(255, 0, 255)
#define CYAN new ColorTexture(0, 255, 255)
#define BLUE new ColorTexture(0, 0, 255)

// "transparent": [0,0,0,0]
#define ALICEBLUE new ColorTexture(240,248,255)
#define ANTIQUEWHITE new ColorTexture(250,235,215)
#define AQUA new ColorTexture(0,255,255)
#define AQUAMARINE new ColorTexture(127,255,212)
#define AZURE new ColorTexture(240,255,255)
#define BEIGE new ColorTexture(245, 245, 220)
#define BISQUE new ColorTexture(255,228,196)
#define BLANCHEDALMOND new ColorTexture(255,235,205)
#define BLUEVIOLET new ColorTexture(138,43,226)
class ColorTexture: public Texture{
public:
  unsigned char r, g, b;
  ColorTexture(unsigned char aa, unsigned char bb, unsigned char cc);
  ColorTexture(unsigned char aa, unsigned char bb, unsigned char cc, double alp);
  ColorTexture(unsigned char aa, unsigned char bb, unsigned char cc, double alp, double ref);
  ColorTexture(unsigned char aa, unsigned char bb, unsigned char cc, double alp, double ref, double amb);
  ColorTexture(char* def);
  void getColor(unsigned char* toFill, double* amb, double *op, double *ref, double x, double y);
};

#endif