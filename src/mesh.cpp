#include "mesh.h"

   
Mesh::Mesh(Vector offset, Vector* points, unsigned int pointCount, unsigned int* polys, unsigned int polyCountt, Texture* t) :Shape(Vector(0,0,0), t, 0., 0., 0.), boundCenter(Vector(0,0,0)){
   textureX = 1.; textureY = 1.; polyCount = polyCountt;
   refreshBounds(points,offset, pointCount);
   center = boundCenter;//TODO IMPLEMENT MOVEMENT
   shapes = (Shape**)malloc(polyCountt*sizeof(Shape*));
   int i;
   for(i = 0; i<polyCountt; i++)
      shapes[i] = new Triangle(points[polys[3*i]] + offset, points[polys[3*i+1]] + offset, points[polys[3*i+2]] + offset, t);
}


Vector* getVectors(FILE* f, int len){
   Vector* vec = (Vector*)malloc(len*sizeof(Vector));
   float x, y, z;
   for(int i = 0; i<len; i++){
   //   printf("%d\n", i);
      fscanf(f, "%f %f %f\n", &x, &y, &z);
      vec[i].x = x;
      vec[i].y = y;
      vec[i].z = z;
    //  printf("\t%f %f %f (%f %f %f)\n", (vec[i].x), (vec[i].y), (vec[i].z), x, y, z);
   }
   return vec;
}
unsigned int* getTriangles(FILE* f, int len){
   unsigned int* vec = (unsigned int*)malloc(3*len*sizeof(unsigned int));
   int a, b, d;
   for(int i = 0; i<3*len; i+=3){
      fscanf(f, "%d %d %d\n", &vec[i], &vec[i+1], &vec[i+2]); 
   }
   return vec;
}

Mesh::Mesh(Vector offset, char* pointL, unsigned int pointCount, char* polyL, unsigned int polyCountt, Texture* t) :Shape(Vector(0,0,0), t, 0., 0., 0.), boundCenter(Vector(0,0,0)){
   
   
   textureX = 1.; textureY = 1.; polyCount = polyCountt;
   FILE* vectors = fopen(pointL,"r"), *triangles = fopen(polyL,"r");
   if (!vectors) {
      printf("Could not open pointq file %s\n", pointL);
      exit(1);
   }
   if (!triangles) {
      printf("Could not open triangles file %s\n", polyL);
      exit(1);
   }
   Vector* points = getVectors(vectors, pointCount);
   fclose(vectors);
   unsigned int* polys = getTriangles(triangles, polyCountt);
   fclose(triangles);
   
   refreshBounds(points,offset, pointCount);
   center = boundCenter;//TODO IMPLEMENT MOVEMENT
   shapes = (Shape**)malloc(polyCountt*sizeof(Shape*));
   
   int i;
   for(i = 0; i<polyCountt; i++){
      shapes[i] = new Triangle(points[polys[3*i]] + offset, points[polys[3*i+1]] + offset, points[polys[3*i+2]] + offset, t);
      }
   free(points);
}

void Mesh::refreshBounds(Vector* points, Vector offset, unsigned int length) {
   int i; 
   double dis,dis_sq,rad_sq,oldc_to_new_c;
   boundCenter.x = (points[0] + offset).x;
   boundCenter.y = (points[0] + offset).y;
   boundCenter.z = (points[0] + offset).z;
   radius = 0.0;
   
   for(i=0;i<length;i++) {
      rad_sq = radius * radius;
      dis_sq =  (points[i].x + offset.x - boundCenter.x)*(points[i].x + offset.x - boundCenter.x) +
            (points[i].y + offset.y - boundCenter.y)*(points[i].y + offset.y - boundCenter.y) +
            (points[i].z + offset.y - boundCenter.z)*(points[i].z + offset.z - boundCenter.z);
          
      if( dis_sq > rad_sq) {
         dis = sqrt( dis_sq);
         radius = (radius + dis)*.5;
         oldc_to_new_c = dis - radius;
         boundCenter.x = (radius*boundCenter.x + oldc_to_new_c*(points[i] + offset).x)/dis;
         boundCenter.y = (radius*boundCenter.y + oldc_to_new_c*(points[i] + offset).y)/dis;
         boundCenter.z = (radius*boundCenter.z + oldc_to_new_c*(points[i] + offset).z)/dis;
      }
   }
   printf("refreshBounds x=%lf y=%lf z=%lf rad=%lf\n", boundCenter.x, boundCenter.y, boundCenter.z, radius);
}

bool Mesh::getLightIntersection(Ray ray, double* fill){
   
   const double A = ray.vector.mag2();
   const double B = 2*ray.vector.dot(ray.point-center);
   const double C = (ray.point-center).mag2()-radius*radius;
   const double descriminant = B*B-4*A*C;
   if(descriminant<0. || descriminant<B*((B>=0)?B:-B)) return false;

   int curShape = polyCount;
   for(int i = 0; i<polyCount; i++){
      if(shapes[i]->getLightIntersection(ray, fill)){
        return true;
      }
   }
   
   return false;
}


double Mesh::getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3){
   
   const double A = ray.vector.mag2();
   const double B = 2*ray.vector.dot(ray.point-center);
   const double C = (ray.point-center).mag2()-radius*radius;
   const double descriminant = B*B-4*A*C;
   if(descriminant<0. || descriminant<B*((B>=0)?B:-B)) 
      return inf;

   int curShape = polyCount;
   double curTime;
   unsigned int tdata1;
   double tdata2, tdata3;
   for(int i = 0; i<polyCount; i++){
      double time = shapes[i]->getIntersection(ray, &tdata1, &tdata2, &tdata3);
      if(time !=inf && (curShape==polyCount || curTime > time)){ 
         curTime = time;
         curShape = i; 
         //*data1 = tdata1; 
         *data2 = tdata2; *data3 = tdata3;
      }
   }
   if(curShape==polyCount) 
      return inf;
   *data1 = curShape;
   return curTime;
}


void Mesh::move(){}
unsigned char Mesh::reversible(unsigned int* data1, double* data2, double* data3)
{ 
   return shapes[*data1]->reversible(data1, data2, data3);
}
void Mesh::getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth, unsigned int* data1, double* data2, double* data3){
   shapes[*data1]->getColor(toFill, am, op, ref, r, ray, depth, data1, data2, data3);
}
Vector Mesh::getNormal(Vector point, unsigned int* data1, double* data2, double* data3){
   return shapes[*data1]->getNormal(point,data1, data2, data3);
}
void Mesh::setAngles(double ya, double pitc, double rol){
   yaw = ya;
   pitc = pitc;
   roll = rol;
}
void Mesh::setYaw(double d){
   yaw = d;}
void Mesh::setPitch(double d){ pitch = d; }
void Mesh::setRoll(double d){ roll = d;}