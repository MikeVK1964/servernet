

#if !defined(CONVCOOR_)
#define CONVCOOR_
#include <math.h>

#define mfabs(a) (((a)>0)?(a):(-a))
////#define M_PI 3.1415926535

// получить пеленг в град и даль хY
// x,y  ---км
// pg --- град.
// sq
template <class T>
inline void GetBG(T x,T y,T& pg,T& sq)
{
 sq=sqrt(x*x+y*y);
 if (sq==0)
 {
  pg=0;
  return;
 }
 if ((x>=0)&&(y>=0))
 {
  pg=asin(mfabs(x)/sq)*180/M_PI;
 }
 if ((x>=0)&&(y<0))
 {
  pg=90+asin(mfabs(y)/sq)*180/M_PI;
 }
 if ((x<0)&&(y<=0))
 {
  pg=270-asin(mfabs(y)/sq)*180/M_PI;
 }
 if ((x<0)&&(y>0))
 {
  pg=270+asin(mfabs(y)/sq)*180/M_PI;
 }

}
//дальность пеленг в XY
inline void DkmBToXY(const float dkm, const float b, float &x, float &y)
{
 
 y =dkm*cos(b*M_PI/180);
 x =dkm*sin(b*M_PI/180);
  
}

#endif
