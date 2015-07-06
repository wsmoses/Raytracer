#include <cstdlib>
#include <iostream>
#include "Miniball.hpp"
#include "../src/vector.h"
int main (int argc, char* argv[])
{

   int             n = 1000000;      // number of points

   double seed;                      // initialize random number generator
   if (argc != 2) {
      seed = 0;
   } 
   else
      seed = std::atoi(argv[1]);
   std::srand (seed);

   // generate random points and store them in a 2-d array
   // ----------------------------------------------------
   Vector ap[n];
   for (int i=0; i<n; ++i) {
      mytype* p = new mytype[d];
      ap[i] = Vector(rand(), rand(), rand());
   }

   // define the types of iterators through the points and their coordinates
   // ----------------------------------------------------------------------
   typedef Vector const* PointIterator; 
   typedef const Vector CoordIterator;

   // create an instance of Miniball
   // ------------------------------
   typedef Miniball::
      Miniball <Miniball::CoordAccessor<PointIterator, CoordIterator> > 
      Miniball;
   Miniball mb (ap, n);
   
   // output results
   // --------------
   // center
   std::cout << "Center:\n  ";
   const Vector center = mb.center(); 
   for(int i=0; i<d; ++i, ++center)
      std::cout << *center << " ";
   std::cout << std::endl;

   // squared radius
   std::cout << "Squared radius:\n  ";
   std::cout << mb.squared_radius() <<  std::endl;

   return 0;
}
