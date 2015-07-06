
#include "light.h"
#include "camera.h"
      
Light::Light(const Vector & cente, unsigned char* colo) : center(cente){
   color = colo;
}

unsigned char* Light::getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}

Autonoma::Autonoma(const Camera& c): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = BLACK;
}

Autonoma::Autonoma(const Camera& c, Texture* tex): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = tex;
}

void Autonoma::addShape(Shape* r){
   ShapeNode* hi = (ShapeNode*)malloc(sizeof(ShapeNode));
   hi->data = r;
   hi->next = hi->prev = NULL;
   if(listStart==NULL){
      listStart = listEnd = hi;
   }
   else{
      listEnd->next = hi;
      hi->prev = listEnd;
      listEnd = hi;
   }
}

void Autonoma::removeShape(ShapeNode* s){
   if(s==listStart){
      if(s==listEnd){
         listStart = listStart = NULL;
      }
      else{
         listStart = s->next;
         listStart->prev = NULL;
      }
   }
   else if(s==listEnd){
      listEnd = s->prev;
      listEnd->next = NULL;
   }
   else{
      ShapeNode *b4 = s->prev, *aft = s->next;
      b4->next = aft;
      aft->prev = b4;
   }
   free(s);
}

void Autonoma::addLight(Light* r){
   LightNode* hi = (LightNode*)malloc(sizeof(LightNode));
   hi->data = r;
   hi->next = hi->prev = NULL;
   if(lightStart==NULL){
      lightStart = lightEnd = hi;
   }
   else{
      lightEnd->next = hi;
      hi->prev = lightEnd;
      lightEnd = hi;
   }
}

void Autonoma::removeLight(LightNode* s){
   if(s==lightStart){
      if(s==lightEnd){
         lightStart = lightStart = NULL;
      }
      else{
         lightStart = s->next;
         lightStart->prev = NULL;
      }
   }
   else if(s==lightEnd){
      lightEnd = s->prev;
      lightEnd->next = NULL;
   }
   else{
      LightNode *b4 = s->prev, *aft = s->next;
      b4->next = aft;
      aft->prev = b4;
   }
   free(s);
}

void getLight(double* tColor, Autonoma* aut, Vector point, Vector norm, unsigned char flip){
   tColor[0] = tColor[1] = tColor[2] = 0.;
   LightNode *t = aut->lightStart;
   unsigned int data1;
   double data2, data3;
   while(t!=NULL){
      double lightColor[3];     
      lightColor[0] = t->data->color[0]/255.;
      lightColor[1] = t->data->color[1]/255.;
      lightColor[2] = t->data->color[2]/255.;
      Vector ra = t->data->center-point;
      ShapeNode* shapeIter = aut->listStart;
      bool hit = false;
      while(!hit && shapeIter!=NULL){
        hit = shapeIter->data->getLightIntersection(Ray(point+ra*.01, ra), lightColor);
         /*
         double time = shapeIter->data->getIntersection(Ray(point+ra*.01, ra), &data1, &data2, &data3);
         if(time>1E-9 && time<1){
            unsigned char toFill[3];
            double opacity, reflection, ambient;
            if(shapeIter->data->texture->constant())
               shapeIter->data->texture->getColor(toFill, &ambient, &opacity, &reflection, 0., 0.);
            else
               shapeIter->data->getColor(toFill, &ambient, &opacity, &reflection, aut, Ray(point+ra*(.01+time), ra), aut->depth, &data1, &data2, &data3);
            if(opacity>1-1E-9) { hit = true;
               break;
            }
            toFill[0] = (unsigned char)((1-opacity)*toFill[0]);
            toFill[1] = (unsigned char)((1-opacity)*toFill[1]);
            toFill[2] = (unsigned char)((1-opacity)*toFill[2]);
            if(toFill[0]<lightCol[0]) lightCol[0] = toFill[0];
            if(toFill[1]<lightCol[1]) lightCol[1] = toFill[1];
            if(toFill[2]<lightCol[2]) lightCol[2] = toFill[2];
         }*/
         shapeIter = shapeIter->next;
      }
      double perc = (norm.dot(ra)/(ra.mag()*norm.mag()));
      if(!hit){
      if(flip && perc<0) perc=-perc;
        if(perc>0){
      
         tColor[0]+= perc*(lightColor[0]);
         tColor[1]+= perc*(lightColor[0]);
         tColor[2]+= perc*(lightColor[0]);
         if(tColor[0]>1.) tColor[0] = 1.;
         if(tColor[1]>1.) tColor[1] = 1.;
         if(tColor[2]>1.) tColor[2] = 1.;
/*         tColor[1] = ground(perc*(lightCol[1])/255.+tColor[1]);
         tColor[2] = ground(perc*(lightCol[2])/255.+tColor[2]);*/
        }
      }
      t =t->next;
   }
}
