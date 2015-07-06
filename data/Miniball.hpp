#include <cassert>
#include <algorithm>
#include <list>
#include <limits>
#include "../src/vector.h"

namespace Miniball {

// Global Functions
// ================
   double mb_sqr (double r) { 
      return r*r;}

// Functors
// ========

// functor to map a point iterator to the corresponding coordinate iterator;
// generic version for points whose coordinate containers have begin()
   template < typename Pit_, typename Cit_ >
   struct CoordAccessor {
      typedef Pit_                                  Pit;
      typedef Cit_                                  Cit;
      inline  Cit operator() (Pit it) const { 
         return (*it).begin(); }
   };

// partial specialization for points whose coordinate containers are arrays
   template < typename Pit_, typename Cit_ >
   struct CoordAccessor<Pit_, Cit_*> {
      typedef Pit_                                  Pit;      
      typedef Cit_*                                 Cit;
      inline  Cit operator() (Pit it) const { 
         return *it; }
   };

// Class Declaration
// =================

   template <typename CoordAccessor>
   class Miniball {
   private:
   // types
   // The iterator type to go through the input points
      typedef typename CoordAccessor::Pit                         Pit; 
   // The iterator type to go through the coordinates of a single point. 
      typedef typename CoordAccessor::Cit                         Cit; 
   // The coordinate type
      typedef typename std::iterator_traits<Cit>::value_type      NT;  
   // The iterator to go through the support points
      typedef typename std::list<Pit>::iterator                   Sit;
   
      Vector* points_begin;        
      unsigned int length;         
      CoordAccessor coord_accessor;  
      const NT nt0; // NT(0)                         
   
   //...for the algorithms
      std::list<Pit> L;                       
      Sit support_end;                        
      int fsize;   // number of forced points                                   
      int ssize;   // number of support points                               
   
   // ...for the ball updates
      NT* current_c;                         
      NT  current_sqr_r;                      
      NT** c;                                    
      NT* sqr_r;                    
   
   // helper arrays
      NT* q0;
      NT* z;
      NT* f;
      NT** v;
      NT** a;
   
   public:
   
   // PRE:  [begin, end) is a nonempty range
   // POST: computes the smallest enclosing ball of the points in the range
   //       [begin, end); the functor a maps a point iterator to an iterator 
   //       through the d coordinates of the point  
      Miniball (Vector* begin, unsigned int len);
   
   // POST: returns a pointer to the first element of an array that holds
   //       the d coordinates of the center of the computed ball  
      const NT* center () const;
   
   // POST: returns the squared radius of the computed ball  
      NT squared_radius () const;
   
   private:  
      void mtf_mb (Sit n);
      void mtf_move_to_front (Sit j);
      void pivot_mb (Pit n);
      void pivot_move_to_front (Pit j);
      NT excess (Pit pit) const;
      bool push (Pit pit);
      NT suboptimality () const;
      void create_arrays();
   };

// Class Definition
// ================
   template <typename CoordAccessor>
   Miniball<CoordAccessor>::Miniball (Vector* begin, unsigned int len) 
   : d (d_), 
   points_begin (begin), 
   length(len), 
   nt0 (NT(0)), 
   L(), 
   support_end (L.begin()), 
   fsize(0), 
   ssize(0), 
   current_c (NULL), 
   current_sqr_r (NT(-1)),
   c (NULL),
   sqr_r (NULL),
   q0 (NULL),
   z (NULL),
   f (NULL),
   v (NULL),
   a (NULL)
   {	
      create_arrays();
   
   // set initial center
      c[0] = nt0;
      current_c = c[0];
   
   // compute miniball
      pivot_mb (points_end);
   }

   template <typename CoordAccessor>
   void Miniball<CoordAccessor>::create_arrays() 
   {
      c = new Vector[d+1]; 
      v = new Vector[d+1]; 
      a = new Vector[d+1];
      for (int i=0; i<d+1; ++i) {
         c[i] = new Vector[d];
         v[i] = new Vector[d];
         a[i] = new Vect[d];
      }
      sqr_r = new NT[d+1];
      q0 = new NT[d];
      z = new NT[d+1];
      f = new NT[d+1];
   } 

   template <typename CoordAccessor>
   const typename Miniball<CoordAccessor>::NT* 
   Miniball<CoordAccessor>::center () const 
   {
      return current_c;
   }
 
   template <typename CoordAccessor>
   typename Miniball<CoordAccessor>::NT 
   Miniball<CoordAccessor>::squared_radius () const 
   {
      return current_sqr_r;
   }

   template <typename CoordAccessor>  
   void Miniball<CoordAccessor>::mtf_mb (Sit n)
   {
   // Algorithm 1: mtf_mb (L_{n-1}, B), where L_{n-1} = [L.begin, n)  
   // B: the set of forced points, defining the current ball
   // S: the superset of support points computed by the algorithm
   // --------------------------------------------------------------
   // from B. Gaertner, Fast and Robust Smallest Enclosing Balls, ESA 1999,
   // http://www.inf.ethz.ch/personal/gaertner/texts/own_work/esa99_final.pdf  
   
   //   PRE: B = S  
      assert (fsize == ssize);
   
      support_end = L.begin();
      if ((fsize) == d+1) 
         return;  
   
   // incremental construction
      for (Sit i = L.begin(); i != n;) 
      {
      // INV: (support_end - L.begin() == |S|-|B|)
         assert (std::distance (L.begin(), support_end) == ssize - fsize);
      
         Sit j = i++; 
         if (excess(*j) > nt0) 
            if (push(*j)) {          // B := B + p_i
               mtf_mb (j);            // mtf_mb (L_{i-1}, B + p_i)
               --fsize;                 // B := B - p_i
               mtf_move_to_front(j);  
            }
      }
   // POST: the range [L.begin(), support_end) stores the set S\B
   }

   template <typename CoordAccessor>  
   void Miniball<CoordAccessor>::mtf_move_to_front (Sit j) 
   {
      if (support_end == j)
         support_end++;
      L.splice (L.begin(), L, j);
   }

   template <typename CoordAccessor>  
   void Miniball<CoordAccessor>::pivot_mb (Pit n) 
   {
   // Algorithm 2: pivot_mb (L_{n-1}), where L_{n-1} = [L.begin, n)  
   // --------------------------------------------------------------
   // from B. Gaertner, Fast and Robust Smallest Enclosing Balls, ESA 1999,
   // http://www.inf.ethz.ch/personal/gaertner/texts/own_work/esa99_final.pdf  
      NT          old_sqr_r;
      const NT*   c;
      Pit         pivot, k;
      NT          e, max_e, sqr_r;
      Cit p;
      do {
         old_sqr_r = current_sqr_r;
         sqr_r = current_sqr_r;
      
         pivot = points_begin;
         max_e = nt0;
         for (k = points_begin; k != n; ++k) {
            p = coord_accessor(k);
            e = -sqr_r;
            c = current_c;
            for (int j=0; j<d; ++j)
               e += mb_sqr<NT>(*p++-*c++);
            if (e > max_e) {
               max_e = e;
               pivot = k;
            }
         }
      
         if (max_e > nt0) {
         // check if the pivot is already contained in the support set
            if (std::find(L.begin(), support_end, pivot) == support_end) {
               assert (fsize == 0);
               if (push (pivot)) {
                  mtf_mb(support_end);
                  --fsize;
                  pivot_move_to_front(pivot);
               }
            }
         }
      } while (old_sqr_r < current_sqr_r);
   }

   template <typename CoordAccessor>  
   void Miniball<CoordAccessor>::pivot_move_to_front (Pit j) 
   {
      L.push_front(j);
      if (std::distance(L.begin(), support_end) == d+2)
         support_end--;
   }

   template <typename CoordAccessor>  
   inline typename Miniball<CoordAccessor>::NT 
   Miniball<CoordAccessor>::excess (Pit pit) const 
   {
      Cit p = coord_accessor(pit);
      NT e = -current_sqr_r;
      NT* c = current_c;
      for (int k=0; k<d; ++k){
         e += mb_sqr<NT>(*p++-*c++);
      }
      return e;
   }

   template <typename CoordAccessor>  
   bool Miniball<CoordAccessor>::push (Pit pit) 
   {
      int i, j;
      NT eps = mb_sqr<NT>(std::numeric_limits<NT>::epsilon());
   
      Cit cit = coord_accessor(pit);
      Cit p = cit;
   
      if (fsize==0) {
         for (i=0; i<d; ++i)
            q0[i] = *p++;
         for (i=0; i<d; ++i)
            c[0][i] = q0[i];
         sqr_r[0] = nt0;
      } 
      else {
      // set v_fsize to Q_fsize
         for (i=0; i<d; ++i)
         //v[fsize][i] = p[i]-q0[i];
            v[fsize][i] = *p++-q0[i];
      
      // compute the a_{fsize,i}, i< fsize
         for (i=1; i<fsize; ++i) {
            a[fsize][i] = nt0;
            for (j=0; j<d; ++j)
               a[fsize][i] += v[i][j] * v[fsize][j];
            a[fsize][i]*=(2/z[i]);
         }
      
      // update v_fsize to Q_fsize-\bar{Q}_fsize
         for (i=1; i<fsize; ++i) {
            for (j=0; j<d; ++j)
               v[fsize][j] -= a[fsize][i]*v[i][j];
         }
      
      // compute z_fsize
         z[fsize]=nt0;
         for (j=0; j<d; ++j)
            z[fsize] += mb_sqr<NT>(v[fsize][j]);
         z[fsize]*=2;
      
      // reject push if z_fsize too small
         if (z[fsize]<eps*current_sqr_r) {
            return false;
         }
      
      // update c, sqr_r
         p=cit;
         NT e = -sqr_r[fsize-1];
         for (i=0; i<d; ++i)
            e += mb_sqr<NT>(*p++-c[fsize-1][i]);
         f[fsize]=e/z[fsize];
      
         for (i=0; i<d; ++i)
            c[fsize][i] = c[fsize-1][i]+f[fsize]*v[fsize][i];
         sqr_r[fsize] = sqr_r[fsize-1] + e*f[fsize]/2;
      }
      current_c = c[fsize];
      current_sqr_r = sqr_r[fsize];
      ssize = ++fsize;
      return true;
   }
}