#include "imagetexture.h"

unsigned char ImageTexture::constant(){ 
   return 0; }

void ImageTexture::getColor(unsigned char* toFill, double* am, double *op, double *ref, double x, double y){
   int xi = (int)(x*w), yi = (int)(y*h);
   int p1 = 4*(xi+w*yi);
   toFill[0] = imageData[p1];
   toFill[1] = imageData[p1+1];
   toFill[2] = imageData[p1+2];
   *op = imageData[p1+3]*opacity/255.;
   *ref = reflection;
   *am = ambient;
}

void ImageTexture::maskImageAlpha(){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         {
            imageData[total+3]=imageData[total];
            imageData[total]=255;
            imageData[total+1]=255;
            imageData[total+2]=255;
         }
      }          
}


void ImageTexture::maskImage(unsigned char r, unsigned char g, unsigned char b, unsigned char rm, unsigned char gm, unsigned char bm, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==r && imageData[total+1]==g && imageData[total+2]==b)
         {
            imageData[total]=rm;
            imageData[total+1]=gm;
            imageData[total+2]=bm;
            imageData[total+3]=m;
         }
      }          
}

void ImageTexture::maskImageA(unsigned char r, unsigned char g, unsigned char b, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]>=r && imageData[total+1]>=g && imageData[total+2]>=b)
         {
            imageData[total+3]=m;
         }
      }          
}
void ImageTexture::maskImageU(unsigned char r, unsigned char g, unsigned char b, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]<=r && imageData[total+1]<=g && imageData[total+2]<=b)
         {
            imageData[total+3]=m;
         }
      }          
}
void ImageTexture::maskImage(unsigned char r, unsigned char g, unsigned char b, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==r && imageData[total+1]==g && imageData[total+2]==b){
            imageData[total+3]=m;
         }
      }          
}
void ImageTexture::maskImage(unsigned char r, unsigned char g, unsigned char b){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==r && imageData[total+1]==g && imageData[total+2]==b){
            imageData[total+3]=0;
         }
      }          
}

void ImageTexture::maskImage(ColorTexture b, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==b.r && imageData[total+1]==b.g && imageData[total+2]==b.b){
            imageData[total+3]=m;
         }
      }          
}
void ImageTexture::maskImage(ColorTexture b){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==b.r && imageData[total+1]==b.g && imageData[total+2]==b.b){
            imageData[total+3]=0;
         }
      }          
}


void ImageTexture::maskImage(ColorTexture* b, unsigned char m){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==b->r && imageData[total+1]==b->g && imageData[total+2]==b->b){
            imageData[total+3]=m;
         }
      }          
}
void ImageTexture::maskImage(ColorTexture* b){
int x,y;
   for(y = h-1; y>=0; y--)
      for(x = 0; x<w; x++){
         int total = 4*(x+y*w);
         if(imageData[total]==b->r && imageData[total+1]==b->g && imageData[total+2]==b->b){
            imageData[total+3]=0;
         }
      }          
}


void ImageTexture::getColor(unsigned char* toFill, double* am, double *op, double *ref,unsigned int x, unsigned int y){
   int start = 4*(x+w*y);
   toFill[0] = imageData[start];
   toFill[1] = imageData[start+1];
   toFill[2] = imageData[start+2];
   *op = imageData[start+3]*opacity/255.;
   *ref = reflection;
   *am = ambient;
}

unsigned char* ImageTexture::setColor(unsigned int x, unsigned int y, unsigned char* data){
   int start = 4*(x+w*y);
   imageData[start] = data[0];
   imageData[start+1] = data[1];
   imageData[start+2] = data[2];
   return &imageData[start];
}


unsigned char* ImageTexture::setColor(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b){
   int start = 4*(x+w*y);
   imageData[start] = r;
   imageData[start+1] = g;
   imageData[start+2] = b;
   return &imageData[start];
}

ImageTexture::ImageTexture(unsigned int ww, unsigned int hh):Texture(.3, 1., 0.){
   w =ww;
   h = hh;
   imageData = (unsigned char*)malloc(4*w*h*sizeof(unsigned char));
   int i;
   for(i = 0; i<w*h; i++){ imageData[i*4+3] = 255; }
} 
ImageTexture::ImageTexture(unsigned char* data, unsigned int ww, unsigned int hh):Texture(.3, 1., 0.){
   imageData = data;
   w =ww;
   h = hh;
}

void ImageTexture::readPPM(FILE* f, char* file){
   if (f == NULL){
      printf("File loading error!!! %s\n", file);
      exit(0);
   }
   int fchar = getc(f);
   if(fchar!='P'){
      printf("Header error --1st char not 'P' %s %c %d\n", file, fchar, fchar);
      exit(0);
   }
   int id = getc(f);
   while(fpeek(f)=='#'){
      int rr;
      do{
         rr = getc(f);
      } while(rr!='\n');
   }
   int x = 0, y = 0;
   if(id=='6'){
      int r = fscanf(f, "%u %u", &w, &h);
      if ( r < 2 ) {
         printf("Could not find width / height -6- %d %d %d\n", r, w, h);
         exit(0);
      }
      int ne = fpeek(f);
      while(ne == ' ' || ne=='\n' || ne=='\t'){ getc(f); ne = fpeek(f); }
      int d;
      r = fscanf(f, "%u", &d);
      if ( (r < 1) || ( d != 255 ) ){
         printf("Illegal max size %u %u", d, r);
         exit(0);
      }
      ne = fpeek(f);
      while(ne == ' ' || ne=='\n' || ne=='\t'){ getc(f); ne = fpeek(f); }
      imageData = (unsigned char*)malloc(4*w*h*(sizeof(unsigned char)));
      for(y = h-1; y>=0; y--)
         for(x = 0; x<w; x++){
            int total = 4*(x+y*w);
            imageData[total]=getc(f);
            imageData[total+1]=getc(f);
            imageData[total+2]=getc(f);
            imageData[total+3] = 255;
         }
   }
   else if(id=='3'){
      int ne = fpeek(f);
      while(ne == ' ' || ne=='\n' || ne=='\t'){ getc(f); ne = fpeek(f); }
      while(fpeek(f)=='#'){
         int rr;
         do{
            rr = getc(f);
         } while(rr!='\n');
      }
      int r = fscanf(f, "%u %u", &w, &h);
      if ( r < 2 ) {
         printf("Could not find width / height -3- %d %d %d\n", r, w, h);
         exit(0);
      }
      int d;
      r = fscanf(f, "%u", &d);
      if ( (r < 1) || ( d != 255 ) ){
         printf("Illegal max size %d %d %d %d", d, r, w, d);
         exit(0);
      }
      fseek(f, 1, SEEK_CUR); /* skip one byte, should be whitespace */
      id = getc(f);
      if(fpeek(f)=='#'){
         int rr;
         do{
            rr = getc(f);
         } while(rr!='\n');
      }
      imageData = (unsigned char*)malloc(4*w*h*(sizeof(unsigned char)));
         
      for(y = h-1; y>=0; y--)
         for(x = 0; x<w; x++){
            int total = 4*(x+y*w);
            unsigned int tmp;
            fscanf(f, "%u", &tmp);
            imageData[total] = (unsigned char)tmp;
            fscanf(f, "%u", &tmp);
            imageData[total+1] = (unsigned char)tmp;
            fscanf(f, "%u", &tmp);
            imageData[total+2] = (unsigned char)tmp;
            imageData[total+3] = 255;
         }
   }
   else{
      
      printf("Unknown PPM FILE!?\n");
      exit(0);
   }


}
ImageTexture::ImageTexture(char* file):Texture(.3, 1., 0.){
   char* ext = findExtension(file);
   if(extensionEquals(ext, "ppm")){
   
      FILE* f = fopen(file, "r");
      readPPM(f, file);
      fclose(f);
   }
   else{
      char command[2000];
      snprintf(command, sizeof(command), "convert %s ppm:-", file);
      FILE* f = popen(command, "r");
      readPPM(f, file);
      fclose(f);
   }


}
