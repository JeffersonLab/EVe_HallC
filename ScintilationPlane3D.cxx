//************************************************************************* 
//  ScintilationPlane3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class is creates 3D view of one scintillation plane, consisting 
//  n scintillation paddles.
//
//
//*************************************************************************  

#include "ScintilationPlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

#define DEBUG_LEVEL 0


using namespace std;


ScintilationPlane3D::ScintilationPlane3D(char *ime, int n, double x, double y, double z, double length, double height, double thickness, TGeoVolume *paddles)
{
   N = n;

   // koordinate x, y, z - determine the center of scintillation plane
   // Compute the center plane
 
   double sy0 = y;
   double sx0 = x - (height*(n)/2.0 - height/2.0);
   double sz0 = z;
	


   for(int i = 0; i<n; i++)
   {
     paddle[i] = new Scintilator3D(i, height*i+sx0, 0.0+sy0, 0.0+sz0, length, height, thickness, paddles);
   }
   
   cout<<"Scintilation Plane 3D is created!"<<endl;
}

ScintilationPlane3D::~ScintilationPlane3D()
{

}

void ScintilationPlane3D::paddleHit(int num, double left, double right)
{
  if (num<N)
  {
    //cout<<"-----------> Paddle number je: "<<num<<endl;
    paddle[num]->hit(left,right);
  }
  else
  { 
#if DEBUG_LEVEL >= 3
    cout<<"There is no paddle with this index!"<<endl;
#endif
  }
}

void ScintilationPlane3D::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }
}
