#include "imagetexture.h"

unsigned char ImageTexture::constant(){ 
   return 0; }

void ImageTexture::getColor(unsigned char* toFill, double* am, double *op, double *ref, double x, double y){
   int xi = (int)(x*w), yi = (int)(y*h);
 //  int xp, yp;   
 //  double dx = x*w-xi-.5;
 //  double dy = y*h-yi-.5;
   
  // if(dx<0) xp = (xi<=0)?0:(xi-1);
 //  else xp = (xi>=w-1)?(w-1):(xi+1);
   
  // if(dy<0) yp = (yi<=0)?0:(yi-1);
  // else yp = (yi>=h-1)?(h-1):(yi+1);
 //  dx = 2*fabs(dx);
  // dy = 2*fabs(dy);

   int p1 = 4*(xi+w*yi);
  // int px = 4*(xp+w*yi);
  // int py = 4*(xi+w*yp);
  // int pxy = 4*(xi+w*yp);
   
   /* toFill[0] = (unsigned char)((imageData[p1]*(1-dy)*(1-dx)+imageData[px]*dx*(1-dy)+imageData[py]*dy*(1-dx)+imageData[pxy]*dx*dy));
   toFill[1] = (unsigned char)((imageData[p1+1]*(1-dy)*(1-dx)+imageData[px+1]*dx*(1-dy)+imageData[py+1]*dy*(1-dx)+imageData[pxy+1]*dx*dy));
   toFill[2] = (unsigned char)((imageData[p1+2]*(1-dy)*(1-dx)+imageData[px+2]*dx*(1-dy)+imageData[py+2]*dy*(1-dx)+imageData[pxy+2]*dx*dy));
   *op = ((imageData[p1+3]*(1-dy)*(1-dx)+imageData[px+3]*dx*(1-dy)+imageData[py+3]*dy*(1-dx)+imageData[pxy+3]*dx*dy))*opacity/255.;
   */
   
   toFill[0] = imageData[p1];
   toFill[1] = imageData[p1+1];
   toFill[2] = imageData[p1+2];
   *op = imageData[p1+3]*opacity/255.;
   
   
   //if(imageData[p1+3]!=255){printf("G:%d %d %f\n",p1, imageData[p1+3], *op);}
  /*
   toFill[0] = (unsigned char)interpolate(interpolate(imageData[py],imageData[pxy], dx),interpolate(imageData[p1],imageData[px], dx), dy);
   toFill[1] = (unsigned char)interpolate(interpolate(imageData[py+1],imageData[pxy+1], dx),interpolate(imageData[p1+1],imageData[px+1], dx), dy);
   toFill[2] = (unsigned char)interpolate(interpolate(imageData[py+2],imageData[pxy+2], dx),interpolate(imageData[p1+2],imageData[px+2], dx), dy);
   *op = interpolate(interpolate(imageData[py+3],imageData[pxy+3], dx),interpolate(imageData[p1+3],imageData[px+3], dx), dy)*opacity/255.;
  */
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
         //   printf("setting: %d %d %d\n", x, y, imageData[total+3]);
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
         //   printf("setting: %d %d %d\n", x, y, imageData[total+3]);
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
   //      fseek(f, 1, SEEK_CUR); /* skip one byte, should be whitespace */
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
            fscanf(f, "%u", &imageData[total]);
            fscanf(f, "%u", &imageData[total+1]);
            fscanf(f, "%u", &imageData[total+2]);
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
      sprintf(command, "convert %s ppm:-", file);
      FILE* f = popen(command, "r");
      readPPM(f, file);
      fclose(f);
   }


}
