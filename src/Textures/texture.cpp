#include "texture.h"

double interpolate(double a,double b,double x)
{
   double f=(1.0-cos(x * M_PI))* 0.5;
   return a*(1.0-f)+b*f;
}

Texture::Texture(double am, double op, double ref):ambient(am),opacity(op), reflection(ref){}

double fix(double a){
   a = fmod(a, 1.);
   if(a<0) a+=1.;
   return a;
}

double ground(double e){
   return (e>1.)?1.:e;
}
const char* findExtension(const char* ola){
   const char* end = ola;
   while(*end!='\0') end++;
   const char* start = end;
   while(*start!='.' && start>ola) start--;
   if(*start=='.') start++;
   return start;
}

char lowerCase(char c){
   if ((c >= 'A') && (c <= 'Z')) 
      return c-'a'+'A'; 
   else 
      return c;
}
 
int fpeek(FILE *stream)
{
   int c;
   c = fgetc(stream);
   ungetc(c, stream);
   return c;
}
  
bool extensionEquals(const char* a, const char* knownExt){
   while(*a!='\0'){
      if(a[0] != knownExt[0]) 
         return false;
      a++;
      knownExt++;
   }
   return knownExt[0]=='\0';
}
