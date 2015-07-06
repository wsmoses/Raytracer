#include "hyperboloid.h"

Hyperboloid::Hyperboloid(const Vector &ca, Texture* t, double ya, double pi, double ro, double aa, double bb, double cc):Shape(ca, t, ya, pi, ro){
   a = aa; b = bb; c = cc;
}
double  Hyperboloid::getIntersection(Ray ray, unsigned int* data1, double* data2, double* data3){
   double R0_0;
   double R0_1;
   double R0_2;
   double R0_3;
   double R0_4;
   double R0_5;
   double R0_6;
   double R0_7;
   double R0_8;
   double R0_9;
   double R0_10;
   double R0_11;
   double R0_12;
   double R0_13;
   double R0_14;
   double R0_15;
   double R0_16;
   double R0_17;
   double R0_18;
   double R0_19;
   double R0_20;
   double R0_21;
   double R0_22;
   double R0_23;
   double R0_24;
   double R0_25;
   double R0_26;
   double R0_27;
   double R0_28;
   double R0_29;
   double R0_30;
   double R0_31;
   double R0_32;
   double R0_33;
   double R0_34;
   double R0_35;
   R0_0 = a;
   R0_1 = b;
   R0_2 = c;
   R0_3 = center.x;
   R0_4 = center.y;
   R0_5 = center.z;
   R0_6 = ray.point.x;
   R0_7 = ray.point.y;
   R0_8 = ray.point.z;
   R0_9 = ray.vector.x;
   R0_10 = ray.vector.y;
   R0_11 = ray.vector.z;
   R0_12 = R0_2 * R0_2;
   R0_13 = R0_0 * R0_0;
   R0_14 = R0_1 * R0_1;
   R0_15 = -R0_7;
   R0_16 = R0_4 + R0_15;
   R0_17 = -R0_6;
   R0_18 = R0_16 * R0_9;
   R0_19 = R0_9 * R0_9;
   R0_20 = R0_0 + R0_3 + R0_17;
   R0_21 = -R0_8;
   R0_22 = R0_5 + R0_21;
   R0_23 = R0_22 * R0_9;
   R0_24 = -R0_3;
   R0_25 = R0_0 + R0_24 + R0_6;
   R0_26 = R0_2 * R0_9;
   R0_27 = R0_0 * R0_11;
   R0_28 = R0_13 * R0_12 * R0_16 * R0_9 * R0_10;
   R0_29 = R0_3 + R0_17;
   R0_30 = R0_12 * R0_29 * R0_9;
   R0_29 = -R0_5;
   R0_29 = R0_29 + R0_8;
   R0_31 = R0_13 * R0_29 * R0_11;
   R0_30 = R0_30 + R0_31;
   R0_31 = R0_14 * R0_9 * R0_30;
   R0_30 = -R0_12;
   R0_29 = R0_20 * R0_10;
   R0_32 = -R0_29;
   R0_29 = R0_18 + R0_32;
   R0_32 = R0_25 * R0_10;
   R0_33 = R0_18 + R0_32;
   R0_30 = R0_30 * R0_29 * R0_33;
   R0_29 = R0_5 * R0_10;
   R0_33 = R0_8 * R0_10;
   R0_32 = -R0_33;
   R0_33 = R0_4 * R0_11;
   R0_34 = -R0_33;
   R0_33 = R0_7 * R0_11;
   R0_29 = R0_29 + R0_32 + R0_34 + R0_33;
   R0_32 = R0_29 * R0_29;
   R0_29 = R0_13 * R0_32;
   R0_32 = R0_12 * R0_19;
   R0_34 = R0_20 * R0_11;
   R0_33 = -R0_34;
   R0_34 = R0_23 + R0_33;
   R0_33 = R0_25 * R0_11;
   R0_35 = R0_23 + R0_33;
   R0_34 = R0_34 * R0_35;
   R0_32 = R0_32 + R0_34;
   R0_34 = R0_14 * R0_32;
   R0_30 = R0_30 + R0_29 + R0_34;
   R0_29 = R0_13 * R0_14 * R0_12 * R0_19 * R0_30;
   if(R0_29<0) return inf;
   R0_30 = sqrt(R0_29);
   R0_28 = R0_28 + R0_31 + R0_30;
   R0_31 = R0_10 * R0_10;
   R0_30 = R0_13 * R0_12 * R0_9 * R0_31;
   R0_31 = -R0_27;
   R0_29 = R0_26 + R0_31;
   R0_31 = R0_26 + R0_27;
   R0_34 = R0_14 * R0_9 * R0_29 * R0_31;
   R0_30 = R0_30 + R0_34;
   R0_34 = 1 / R0_30;
   return R0_28 * R0_34;
}

void Hyperboloid::move(){
   //d = -vect.dot(center);
}
void Hyperboloid::getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth, unsigned int* data1, double* data2, double* data3){
   texture->getColor(toFill, am, op, ref, 0., 0.);
}
Vector Hyperboloid::getNormal(Vector point, unsigned int* data1, double* data2, double* data3){
return Vector(0, 1, 0);
   //return vect;
}
