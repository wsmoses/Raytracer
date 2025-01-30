//#include<printf.h>
#include "src/vector.h"
#include "src/shape.h"
#include "src/sphere.h"
#include "src/plane.h"
#include "src/light.h"
#include "src/box.h"
#include "src/disk.h"
#include "src/hyperboloid.h"
#include "src/mandel.h"
#include "src/mesh.h"
#include "src/Textures/imagetexture.h"
#include "src/Textures/functiontexture.h"
#include "src/Textures/fractalnoise.h"
#include "src/Textures/mandeltexture.h"
#include "src/Textures/colortexture.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
using namespace std;

unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}
     
int W = 1000, H = 1000;

unsigned char* DATA = (unsigned char*)malloc(W*H*3*sizeof(unsigned char));
unsigned char get(int i, int j, int k){
   return DATA[3*(i+j*W)+k]; 
}
unsigned char* getPos(int i, int j){
   return &DATA[3*(i+j*W)]; 
}
void set(int i, int j, unsigned char r, unsigned char g, unsigned char b){
   DATA[3*(i+j*W)] = r; 
   DATA[3*(i+j*W)+1] = g; 
   DATA[3*(i+j*W)+2] = b; 
}

void refresh(Autonoma* c){
   for(int n = 0; n<H*W; ++n) 
   { 
      Vector ra = c->camera.forward+((double)(n%W)/W-.5)*((c->camera.right))+(.5-(double)(n/W)/H)*((c->camera.up));
      calcColor(&DATA[3*n], c, Ray(c->camera.focus, ra), 0);
   }
}

void outputPPM(FILE* f){
   fprintf(f, "P6 %d %d 255 ", W, H);
   fwrite(DATA, 1, W*H * 3, f);
}

void outputPPM(char* file){
   FILE* f = fopen(file, "w");
   outputPPM(f);
   fclose(f);
}
void output(char* file){
   char command[2000];
   FILE* f;
   snprintf(command, sizeof(command), "convert ppm:- %s", file);
   printf("%s\n",command);
   f = popen(command, "w");
   outputPPM(f);
   fclose(f);
}

Texture* parseTexture(FILE* f, bool allowNull) {
   char texture_type[80];
   if (fscanf(f, "%s", texture_type) == EOF) {
      printf("Could not find texture type\n");
      exit(1);
   }
   if (strcmp(texture_type, "null")) {
      if (allowNull)
         return NULL;
      printf("Null texture not permitted\n");
      exit(1);
   }
   if (strcmp(texture_type, "color")) {
      int r, g, b;
      double opacity, reflection, ambient;
      if (fscanf(f, "%d %d %d %lf %lf %lf\n", &r, &g, &b, &opacity, &reflection, &ambient) == EOF) {
         printf("Could not read <r> <g> <b> <opacity> <reflection> <ambient>\n");
         exit(1);
      }
      return new ColorTexture((unsigned char)r, (unsigned char)g, (unsigned char)b, opacity, reflection, ambient);
   }
   if (strcmp(texture_type, "fractal")) {
      double x, y;
      double xoff, yoff;
      double evolution;
      if (fscanf(f, "%lf %lf %lf %lf %lf\n", &x, &y, &xoff, &yoff, &evolution) == EOF) {
         printf("Could not read <r> <g> <b> <opacity> <reflection> <ambient>\n");
         exit(1);
      }
      return new FractalNoise(x, y, xoff, yoff, evolution);
   }
   if (strcmp(texture_type, "image")) {
      char image_file[100];
      if (fscanf(f, "%s\n", image_file) == EOF) {
         printf("Could not read <image path>\n");
         exit(1);
      }
      return new ImageTexture(image_file);
   }
   if (strcmp(texture_type, "mandel")) {
      double x, y;
      double h, w;
      int escape;
      if (fscanf(f, "%lf %lf %lf %lf %d\n", &x, &y, &h, &w, &escape) == EOF) {
         printf("Could not read <x> <y> <height> <width> <escape limit>\n");
         exit(1);
      }
      return new MandelTexture(x, y, h, w, escape);
   }

   printf("Unknown texture type %s\n", texture_type);
   exit(1);
}

Autonoma* createInputs(const char* inputFile) {
   ImageTexture* ola = new ImageTexture(2, 2);
   ola->setColor(0, 0, ola->setColor(1, 1, 0, 0, 0));
   ola->setColor(1, 0, ola->setColor(0, 1, 255, 255, 255));
   ola->reflection = .2;
   
   double camera_x = 0;
   double camera_y = 2;
   double camera_z = 0;
   double yaw = 0;
   double pitch = 0;
   double roll = 0;
   char background[80] = "images/skybox.jpg";

   FILE *f = NULL;
   if (inputFile) {
      FILE *f = fopen(inputFile, "r");
      if (!f) {
         printf("Could not open input file %s\n", inputFile);
         exit(1);
      }
      if (fscanf(f, "%lf %lf %lf %lf %lf %lf\n", &camera_x, &camera_y, &camera_z, &yaw, &pitch, &roll) == EOF) {
         printf("Could not read <camera_x> <camera_y> <camera_z> <yaw> <pitch> <roll>\n");
         exit(1);
      }
      if (fscanf(f, "%s\n", background) == EOF) {
         printf("Could not read <background image file path>\n");
         exit(1);
      }
   }
   Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(camera_x, camera_y, camera_z), yaw, pitch, roll),new ImageTexture(background));

   if (f) {
      char object_type[80];
      while (fscanf(f, "%s", object_type) != EOF) {
         if (strcmp(object_type, "light")) {
            double light_x, light_y, light_z;
            int color_r, color_g, color_b;
            if (fscanf(f, "%lf %lf %lf %d %d %d\n", &light_x, &light_y, &light_z, &color_r, &color_g, &color_b) == EOF) {
               printf("Could not read <light_x> <light_y> <light_z> <color_r> <color_g> <color_b>\n");
               exit(1);
            }
            Light *light = new Light(Vector(light_x, light_y, light_z), getColor(color_r, color_g, color_b));
            MAIN_DATA->addLight(light);
         } else if (strcmp(object_type, "plane")) {
            double plane_x, plane_y, plane_z;
            double yaw, pitch, roll;
            double tx, ty;
            if (fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf\n", &plane_x, &plane_y, &plane_z, &yaw, &pitch, &roll, &tx, &ty) == EOF) {
               printf("Could not read <plane_x> <plane_y> <plane_z> <yaw> <pitch> <roll> <tx> <ty>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape *shape = new Plane(Vector(plane_x, plane_y, plane_z), texture, yaw, pitch, roll, tx, ty);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else if (strcmp(object_type, "disk")) {
            double disk_x, disk_y, disk_z;
            double yaw, pitch, roll;
            double tx, ty;
            if (fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf\n", &disk_x, &disk_y, &disk_z, &yaw, &pitch, &roll, &tx, &ty) == EOF) {
               printf("Could not read <disk_x> <disk_y> <disk_z> <yaw> <pitch> <roll> <tx> <ty>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape* shape = new Disk(Vector(disk_x, disk_y, disk_z), texture, yaw, pitch, roll, tx, ty);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else if (strcmp(object_type, "box")) {
            double box_x, box_y, box_z;
            double yaw, pitch, roll;
            double tx, ty;
            if (fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf\n", &box_x, &box_y, &box_z, &yaw, &pitch, &roll, &tx, &ty) == EOF) {
               printf("Could not read <box_x> <box_y> <box_z> <yaw> <pitch> <roll> <tx> <ty>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape* shape = new Box(Vector(box_x, box_y, box_z), texture, yaw, pitch, roll, tx, ty);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else if (strcmp(object_type, "triangle")) {
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            if (fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3) == EOF) {
               printf("Could not read <x1> <y1> <z1> <x2> <y2> <z2> <x3> <y3> <z3>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape* shape = new Triangle(Vector(x1, y1, z1), Vector(x2, y2, z2), Vector(x3, y3, z3), texture);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else if (strcmp(object_type, "sphere")) {
            double sphere_x, sphere_y, sphere_z;
            double yaw, pitch, roll;
            double radius;
            if (fscanf(f, "%lf %lf %lf %lf %lf %lf %lf\n", &sphere_x, &sphere_y, &sphere_z, &yaw, &pitch, &roll, &radius) == EOF) {
               printf("Could not read <sphere_x> <sphere_y> <sphere_z> <yaw> <pitch> <roll> <radius>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape* shape = new Sphere(Vector(sphere_x, sphere_y, sphere_z), texture, yaw, pitch, roll, radius);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else if (strcmp(object_type, "mesh")) {
             char point_filepath[100];
             char poly_filepath[100];
             int num_points;
             int num_polygons;
            if (fscanf(f, "%s %d %s %d\n", point_filepath, &num_points, poly_filepath, &num_polygons) == EOF) {
               printf("Could not read <point filepath> <num_points> <polygons filepath> <num_polygons>\n");
               exit(1);
            }
            Texture *texture = parseTexture(f, false);
            Shape* shape = new Mesh(point_filepath, num_points, poly_filepath, num_polygons, texture);
            MAIN_DATA->addShape(shape);
            shape->normalMap = parseTexture(f, true);
         } else {
           printf("Unknown object type %s\n", object_type);
           exit(1);
         }
      }
   }


//  ola->normalMap = new ImageTexture("images/sea.jpg");
//  ola->mapX = ola->mapY = 10;

/*ImageTexture* ola = new ImageTexture(20, 20);
for(int i = 0; i<20*20; i++){
if(i%2==0)ola->setColor(i%20, i/20, 255, 0, 255);
else ola->setColor(i%20, i/20, 255,255,0);
}*/
  // Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.502048-1, 0.582642, -2.499250), M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
//    Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.502048, 0.582642, -2.499250-1), M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
 

 MAIN_DATA->addLight(new Light(Vector(0, 5, 0), getColor(255, 255, 255)));
  Texture* ft = new ColorTexture(150,150,150);
  ft->reflection = 0;
    MAIN_DATA->addShape(new Plane(Vector(0, 0, 20), ft, 0,M_PI_4, -M_PI_2, .6, .6));
   MAIN_DATA->addShape(new Plane(Vector(0, 0, 20), ft, 0,M_PI_4, M_PI_2, .6, .6));
   
   MAIN_DATA->addShape(new Plane(Vector(0, 0, 0), ola, M_PI_4, M_PI_2,  0, 1, 1));
   MAIN_DATA->addShape(new Disk(Vector(0,.01,8),RED,M_PI_4, M_PI_2,  0, 2, 2));
   MAIN_DATA->addShape(new Box(Vector(0-3/sqrt(2),1,8+3/sqrt(2)),BLUE,-M_PI_4, 0,  0, 7, 2));
   MAIN_DATA->addShape(new Box(Vector(0-3.8/sqrt(2),2.54,8+3.8/sqrt(2)),ORANGE,-M_PI_4, -M_PI_4*1.25,  0, 7, 2));
//  MAIN_DATA->addShape(new Plane(Vector(0,0,0), ola, 0, 0, -M_PI_2, 1, 1));
//  MAIN_DATA->addShape(new Plane(Vector(0,0,0), ola, 0, 0, -M_PI_2, 1, 1));
  // ImageTexture* normal = new ImageTexture("images/sea.jpg");
//Texture* alpha;= new ImageTexture("images/montreal.png");
//Texture *alpha = new FractalNoise(1., 1., 0., 0., 0.);
//alpha->maskImageA(150,150,150,0);
//Texture* alpha = GREEN;
/*Plane* b;
//alpha->ambient = 1.;
//alpha->opacity = .7;
MAIN_DATA->addShape(b = new Box(Vector(0, 0, 1),alpha, 0,0,0,1,1));

//b->normalMap = normal;
b->mapX = 1.;
b->mapY = 1.;*/
   //Autonoma* MAIN_DATA = new Autonoma(Camera(Vector(1.6, .5, -2.6), -M_PI_2, 0, 0), new ImageTexture("images/skybox.jpg"));
   
   /*
   Vector a = Vector(2, 3, 1), b = Vector(-2, -3, -1), c = Vector(-5, 3, 4), d = Vector(4, -1, 2);
   MAIN_DATA->addShape(new Triangle(a, b, c, BLUE));
   MAIN_DATA->addShape(new Sphere(a, RED,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(b, BLUE,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(c, BLACK,0,0,0,.1));
   MAIN_DATA->addShape(new Sphere(c, GREEN,0,0,0,.1));
   */
   //Mesh* mesh = new Mesh("data/x.txt", 1586, "data/f.txt", 3168,new ColorTexture(200, 200, 200));
   //Texture* tex = new ColorTexture(200, 200, 200);
   //tex->reflection = .6;
   //Mesh* mesh = new Mesh("data/elepx.txt", 62779, "data/elepf.txt", 111748,tex);
   //Mesh* mesh = new Mesh("data/billyp.txt", 4, "data/billyv.txt", 2,new ColorTexture(200, 200, 200));
   //for(int i = 0; i<mesh->polyCount; i++)
   //MAIN_DATA->addShape(mesh);
//   Shape* mesh = new Sphere(Vector(1.502048, 0.582642, -2.499250), new ColorTexture(200, 200, 200),0,0,0, 0.249319);
  // MAIN_DATA->addShape(mesh);
   
   //importMesh(MAIN_DATA, "data/x.txt", "data/f.txt");
   //Triangle* tr = (Triangle*)MAIN_DATA->listStart->data;
   //printf("%f %f %f y:%f p:%f r:%f||\n",tr->center.x, tr->center.y, tr->center.z, tr->yaw, tr->pitch, tr->roll);
//   MAIN_DATA->addShape(new Box(Vector(1.500000, 0.666667, -2.500000), RED, M_PI_2, 0, 0, 1, 1));
  
//   printf("seg?\n");
   //Texture* te;
   //MAIN_DATA->addShape(new Sphere(Vector(.5-.5, .5-.5, 1./6.),te = BLUE, 0, 0, 0, 1./6.));
   //te->reflection = .7;
   //MAIN_DATA->addShape(new Sphere(Vector(5./6.-.5, .5-.5, .5), GREEN, 0, 0, 0, 1./6.));
   //Texture* tx = new ImageTexture("images/world.jpg");
   //tx->ambient=0.1;
   //ImageTexture* clouds = new ImageTexture("images/cloud.jpg");
   //clouds->maskImageAlpha();
   //Shape* mars = new Sphere(Vector(1./3.-.5, 2./3.-.5, 2./3.), tx, 0, 0, 0, 1./3.);
   //MAIN_DATA->addShape(mars);
   //Sphere* cl;
   //MAIN_DATA->addShape(cl = new Sphere(Vector(1./3.-.5, 2./3.-.5, 2./3.), clouds, 0, 0, 0, .34));
   //mars->normalMap = new ImageTexture("images/EarthNormalMap.png");
   //mars->mapX = mars->mapY = 1;
  // Shape* mand;
   //Texture* mp;
   
  // MAIN_DATA->addShape(mand = new Mandelbrot(Vector(0, .5, .5),mp = new MandelTexture(), 0, 0, 0, 3.5/2, 2./2, 13));
   //mp->ambient = .7;
   //MAIN_DATA->addShape(new Triangle(Vector(0,0,0),Vector(1,1,1),Vector(1,0,0), RED));
      //for(int i = 0; i<4; i++) printf("%d %d %d %d\n", ola->imageData[4*i], ola->imageData[4*i+1], ola->imageData[4*i+2], ola->imageData[4*i+3]);
  // Plane* pl = new Plane(Vector(0-.5, -.5, 0), AQUA, 0, -M_PI/2,  0, .6, .6);
  // pl->texture->reflection = .1;
  // pl->texture->opacity = .7;
  // MAIN_DATA->addShape(pl);
  // pl->normalMap = normal;
  // pl->mapX = pl->mapY = 1;
   return MAIN_DATA;
}

double identity(double x, double from, double to) {
   return (1 - x) * from + x * to;
}
double expfn(double x, double from, double to) {
   return (to - from) * exp(10 * x) / exp(10) + from;
}
double sinfn(double x, double from, double to) {
   return (to - from) * sin(x * 6.28) + from;
}

void setFrame(const char* animateFile, Autonoma* MAIN_DATA, int frame, int frameLen) {
   if (animateFile) {
      char object_type[80];
      char transition_type[80];
      int obj_num;
      char field_type[80];
      double from;
      double to;
      FILE* f = fopen(animateFile, "r");
      while (fscanf(f, "%s %s %d %s %lf %lf", transition_type, object_type, &obj_num, field_type, &from, &to) != EOF) {
         double (*func)(double, double, double);
         if (strcmp(transition_type, "linear")) {
            func = identity;
         } else if (strcmp(transition_type, "exp")) {
            func = expfn;
         } else if (strcmp(transition_type, "sin")) {
            func = sinfn;
         } else {
            printf("Unknown transition type %s, expected one of linear, exp, or sin\n", transition_type);
            exit(1);
         }
         double result = func((double)frame / frameLen, from, to);

         if (strcmp(object_type, "camera")) {
            if (strcmp(field_type, "yaw")) {
               MAIN_DATA->camera.setYaw(result);
            } else if (strcmp(field_type, "pitch")) {
               MAIN_DATA->camera.setPitch(result);
            } else if (strcmp(field_type, "roll")) {
               MAIN_DATA->camera.setRoll(result);
            } else if (strcmp(field_type, "x")) {
               MAIN_DATA->camera.focus.x = result;
            } else if (strcmp(field_type, "y")) {
               MAIN_DATA->camera.focus.y = result;
            } else if (strcmp(field_type, "z")) {
               MAIN_DATA->camera.focus.y = result;
            } else {
               printf("Unknown camera field_type %s, expected one of yaw, pitch, roll, x, y, z\n", field_type);
               exit(1);
            }
         } else if (strcmp(object_type, "object")) {
            ShapeNode* node = MAIN_DATA->listStart;
            for (int i=0; i<obj_num; i++) {
               if (node == MAIN_DATA->listEnd) {
                  printf("Could not find object number %d\n", obj_num);
                  exit(1);
               }
               node = node->next;
            }
            Shape* shape = node->data;

            if (strcmp(field_type, "yaw")) {
               shape->setYaw(result);
            } else if (strcmp(field_type, "pitch")) {
               shape->setPitch(result);
            } else if (strcmp(field_type, "roll")) {
               shape->setRoll(result);
            } else if (strcmp(field_type, "textureX")) {
               shape->textureX = result;
            } else if (strcmp(field_type, "textureY")) {
               shape->textureY = result;
            } else if (strcmp(field_type, "mapX")) {
               shape->mapX = result;
            } else if (strcmp(field_type, "mapX")) {
               shape->mapX = result;
            } else if (strcmp(field_type, "mapOffX")) {
               shape->mapOffX = result;
            } else if (strcmp(field_type, "mapOffY")) {
               shape->mapOffY = result;
            } else {
               printf("Unknown shape field_type %s, expected one of yaw, pitch, roll, textureX, textureY, mapOffX, mapOffY\n", field_type);
               exit(1);
            }
         } else {
            printf("Unknown object_type %s, expected one of camera, object\n", field_type);
            exit(1);
         }
      }
   }
    //  mand->setYaw(exp((double)frame/48)); 
     // mars->setYaw(-(double)frame/24);

   MAIN_DATA->camera.setPitch(M_PI*frame/frameLen);
   MAIN_DATA->camera.setYaw(2*M_PI*(.25-(double)frame/frameLen));
   MAIN_DATA->camera.focus.z = -2.499250-sin(2*M_PI*frame/frameLen);
   MAIN_DATA->camera.focus.x = 1.502048-cos(2*M_PI*frame/frameLen);

    //   pl->mapOffY = (double)frame/48;
    //   mars->setYaw((double)-frame/frameLen*M_TWO_PI);
    //   cl->setYaw((double)frame/48*M_TWO_PI);

   refresh(MAIN_DATA);
}

int main(int argc, const char** argv){

   int frameLen = 1;
   const char* inFile = NULL;
   const char* animateFile = NULL;
   const char* outFile = NULL;
   bool toMovie = true;
   for (int i=1; i<argc; i++) {
      if (strcmp(argv[i], "-H")) {
         if (i + 1 >= argc) {
            printf("Error -H option must be followed by an integer height");
         }
         H = atoi(argv[i+1]);
         i++;
         continue;
      }
      if (strcmp(argv[i], "-W")) {
         if (i + 1 >= argc) {
            printf("Error -W option must be followed by an integer width");
         }
         H = atoi(argv[i+1]);
         i++;
         continue;
      }
      if (strcmp(argv[i], "-F")) {
         if (i + 1 >= argc) {
            printf("Error -F option must be followed by an integer number of frames");
         }
         frameLen = atoi(argv[i+1]);
         i++;
         continue;
      }
      if (strcmp(argv[i], "-o")) {
         if (i + 1 >= argc) {
            printf("Error -o option must be followed by an output file path");
         }
         outFile = argv[i+1];
         i++;
         continue;
      }
      if (strcmp(argv[i], "-i")) {
         if (i + 1 >= argc) {
            printf("Error -i option must be followed by an input file path");
         }
         inFile = argv[i+1];
         i++;
         continue;
      }
      if (strcmp(argv[i], "-a")) {
         if (i + 1 >= argc) {
            printf("Error -a option must be followed by an animation input file path");
         }
         animateFile = argv[i+1];
         i++;
         continue;
      }
      if (strcmp(argv[i], "--movie")) {
         toMovie = true;
         continue;
      }
      if (strcmp(argv[i], "--no-movie")) {
         toMovie = false;
         continue;
      }
      if (strcmp(argv[i], "--help")) {
         printf("Usage %s [-H <height>] [-W <width>] [-F <framecount>] [--movie] [--no-movie] [--help] [-o <outfile>] [-i <infile>]", argv[0]);
         return 0;
      }
      printf("Unknown option %s, look at %s --help\n", argv[i], argv[0]);
      return 1;
   }

   if (outFile == NULL) {
      if (frameLen == 1) {
         outFile = "output.png";
      } else {
         outFile = "output.mp4";
      }
   }

   Autonoma* MAIN_DATA = createInputs(inFile);
   
   int frame;
   char command[200];
   
   for(frame = 0; frame<frameLen; frame++) {
      setFrame(animateFile, MAIN_DATA, frame, frameLen);      
      if (frameLen == 1) {
         snprintf(command, sizeof(command), "%s", outFile);    
      } else {
         snprintf(command, sizeof(command), "%s.tmp.%d.png", outFile, frame);
      }
      output(command);      
      printf("Done Frame %7d|\n", frame);
   }
   if (frameLen > 1 && toMovie) {
      snprintf(command, sizeof(command), "ffmpeg -r 24 -i %s.tmp.%%07d.png -vcodec ffv1 -sameq %s.tmp.avi && ffmpeg -i %s.tmp.avi -c:v libx264 -preset veryslow -qp 0 -r 24 %s", outFile, outFile, outFile, outFile);
   }   
   return 0;
   
}
