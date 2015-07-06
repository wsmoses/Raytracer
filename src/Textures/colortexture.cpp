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

/*
unsigned char clamp_css_byte(double tt) {
   int i = round(tt); 
   return i < 0 ? 0 : i > 255 ? 255 : (unsigned char)i;
}

double clamp_css_float(f) {
   return f < 0 ? 0 : f > 1 ? 1 : f;
}

unsigned char parse_css_int(unsigned char* str, int len) {  // int or percentage.
   if (str[len - 1] == '%')
      return clamp_css_byte(parseFloat(str, len-1) / 100 * 255);
   return clamp_css_byte(parseInt(str, len-1));
}

double parse_css_float(unsigned char* str, int len) {  // float or percentage.
   if (str[str.length - 1] == '%')
      return clamp_css_float(parseFloat(str, len-1) / 100);
   return clamp_css_float(parseFloat(str, len-1));
}

function css_hue_to_rgb(m1, m2, h) {
   h = fix(h);

   if (h * 6 < 1) 
      return m1 + (m2 - m1) * h * 6;
   if (h * 2 < 1) 
      return m2;
   if (h * 3 < 2) 
      return m1 + (m2 - m1) * (2/3 - h) * 6;
   return m1;
}

void trimLowerCase(unsigned char* str, int &len){
   char str2[len];
   int i = 0, j = 0;
   for(i = 0; i<len; i++){
      if(!(str[i]==' ' || str[i]=='\n' || str[i]=='\t' || str[i]=='\r')) {str2[j] = lowerCase(str[i]); j++;}
   }
   len = j;
}

ColorTexture* parseCSSColor(unsigned char* str, int len) {
   trimLowerCase(str, len);
  // Color keywords (and transparent) lookup.
   if (str in kCSSColorTable) 
      return kCSSColorTable[str].slice();  // dup.

  // #abc and #abc123 syntax.
   if (str[0] == '#') {
      if (str.length == 4) {
         int iv = parseInt(str.substr(1), 16);  // TODO(deanm): Stricter parsing.
         if (!(iv >= 0 && iv <= 0xfff)){ printf("INVALID COLOR %s", str); exit(0);}
         return new ColorTexture(((iv & 0xf00) >> 4) | ((iv & 0xf00) >> 8),
              (iv & 0xf) | ((iv & 0xf) << 4),
              (iv & 0xf0) | ((iv & 0xf0) >> 4));
      } 
      else if (len == 7) {
         var iv = parseInt(str.substr(1), 16);  // TODO(deanm): Stricter parsing.
         if (!(iv >= 0 && iv < 0xffffff)){ printf("INVALID COLOR %s", str); exit(0);}
         return new ColorTexture((iv & 0xff0000) >> 16,
              (iv & 0xff00) >> 8,
              iv & 0xff);
      }
      else{
         printf("INVALID COLOR %s", str); exit(0);
      }
   }

   int op = str.indexOf('('), ep = str.indexOf(')');
   if (op != -1 && ep + 1 == str.length) {
      char* fname = str.substr(0, op);
      char** params = str.substr(op+1, ep-(op+1)).split(',');
      double alpha = 1;  // To allow case fallthrough.
      switch (fname) {
         case "rgba":
            if (params.length !== 4) 
               return null;
            alpha = parse_css_float(params.pop());
         // Fall through.
         case "rgb":
            if (params.length != 3) 
               return null;
            return new ColorTexture(parse_css_int(params[0]),
                parse_css_int(params[1]),
                parse_css_int(params[2]),
                alpha);
         case "hsla":
            if (params.length != 4) 
               return null;
            alpha = parse_css_float(params.pop());
         // Fall through.
         case "hsl":
            if (params.length != 3) 
               return null;
            var h = (((parseFloat(params[0]) % 360) + 360) % 360) / 360;  // 0 .. 1
         // NOTE(deanm): According to the CSS spec s/l should only be
         // percentages, but we don't bother and let float or percentage.
            double s = parse_css_float(params[1]);
            double l = parse_css_float(params[2]);
            double m2 = l <= 0.5 ? l * (s + 1) : l + s - l * s;
            double m1 = l * 2 - m2;
            return return new ColorTexture(clamp_css_byte(css_hue_to_rgb(m1, m2, h+1/3) * 255),
                clamp_css_byte(css_hue_to_rgb(m1, m2, h) * 255),
                clamp_css_byte(css_hue_to_rgb(m1, m2, h-1/3) * 255),
                alpha);
         default:
            return null;
      }
   }

   return null;
}
*/
ColorTexture::ColorTexture(char* def):Texture(.3,1.,0.){

}
void ColorTexture::getColor(unsigned char* toFill, double* amb, double *op, double *ref, double x, double y){
   toFill[0] = r; toFill[1] = g; toFill[2] = b;
   *op = opacity;
   *ref = reflection;
   *amb = ambient;
}

unsigned char ColorTexture::constant(){ 
   return 1; }