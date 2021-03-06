#include "shape.h"

Shape::Shape(const Vector &c, Texture* t, double ya, double pi, double ro): center(c), texture(t), yaw(ya), pitch(pi), roll(ro){
};

void Shape::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
}

void Shape::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
}

void Shape::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
}

void Shape::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
}

void calcColor(unsigned char* toFill,Autonoma* c, Ray ray, unsigned int depth){
   double curTime;
   Shape* curShape = NULL;
   ShapeNode* t = c->listStart;
   unsigned int data1;
   double data2, data3;
   unsigned int tdata1;
   double tdata2, tdata3;
   while(t!=NULL){
      double time = t->data->getIntersection(ray, &tdata1, &tdata2, &tdata3);
      if(time !=inf && (curShape==NULL || curTime > time)){
         curTime = time;
         curShape = t->data;
         data1 = tdata1; data2 = tdata2; data3 = tdata3;
         //printf("II\n");
         //printf("I: \t%u\n", data1);
      }
      t = t->next;
   }
   if(curShape==NULL){
      double opacity, reflection, ambient;
      if(c->skybox->constant())
         c->skybox->getColor(toFill, &ambient, &opacity, &reflection, 0., 0.);
      else{
         Vector temp = ray.vector.normalize();
         const double x = temp.x;
         const double z = temp.z;
         const double me = (temp.y<0)?-temp.y:temp.y;
         const double angle = atan2(z, x);
         c->skybox->getColor(toFill, &ambient, &opacity, &reflection, fix(angle/M_TWO_PI),fix(me));
      }
   }
   else{
      Vector intersect = curTime*ray.vector+ray.point;
      double opacity, reflection, ambient;
      if(curShape->texture->constant())
         curShape->texture->getColor(toFill, &ambient, &opacity, &reflection, 0., 0.);
      else
         curShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth, &data1, &data2, &data3);
      
      double lightData[3];
      getLight(lightData, c, intersect, curShape->getNormal(intersect, &data1, &data2, &data3), curShape->reversible(&data1, &data2, &data3));
      toFill[0] = (unsigned char)(toFill[0]*(ambient+lightData[0]*(1-ambient)));
      toFill[1] = (unsigned char)(toFill[1]*(ambient+lightData[1]*(1-ambient)));
      toFill[2] = (unsigned char)(toFill[2]*(ambient+lightData[2]*(1-ambient)));
      if(depth<c->depth && (opacity<1-1e-6 || reflection>1e-6)){
         unsigned char col[4];
         if(opacity<1-1e-6){
            Ray nextRay = Ray(intersect+ray.vector*1E-4, ray.vector);
            calcColor(col, c, nextRay, depth+1);
            toFill[0]= (unsigned char)(toFill[0]*opacity+col[0]*(1-opacity));
            toFill[1]= (unsigned char)(toFill[1]*opacity+col[1]*(1-opacity));
            toFill[2]= (unsigned char)(toFill[2]*opacity+col[2]*(1-opacity));
        
         // if(toFill[0]==255 && toFill[1]==255 && toFill[2]==255) printf("WTF\n");
          //  printf("%d %d %d ||%d %d %d %f %f %f\n", toFill[0], toFill[1], toFill[2], col[0], col[1], col[2], nextRay.point.x, nextRay.point.y, nextRay.point.z);
         }
         if(reflection>1e-6){
            Vector norm = curShape->getNormal(intersect, &data1, &data2, &data3).normalize();
            Vector vec = ray.vector-2*norm*(norm.dot(ray.vector));
            Ray nextRay = Ray(intersect+vec*1E-4, vec);
            calcColor(col, c, nextRay, depth+1);
         
            toFill[0]= (unsigned char)(toFill[0]*(1-reflection)+col[0]*(reflection));
            toFill[1]= (unsigned char)(toFill[1]*(1-reflection)+col[1]*(reflection));
            toFill[2]= (unsigned char)(toFill[2]*(1-reflection)+col[2]*(reflection));
         }
      }
      
   }
}
