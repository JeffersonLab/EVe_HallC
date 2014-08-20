///////////////////////////////////////
/*  ScintPlane3D.cxx  7/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddle3Ds to create a 3D plane

*/
///////////////////////////////////////

#include "ScintPlane3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

ScintPlane3D::ScintPlane3D(char *name, int n, double x, double y, double z, double length, double height, double thickness, TGeoVolume *paddles)
{
   N = n;

   GetVariables *ptr = new GetVariables("HMS.txt");
   int number = ptr->GetInt("Number of paddle PMTs =");

   // Determine center of scint plane
 
   double sy0 = y;
   double sx0 = x - (height*(n)/2.0 - height/2.0);
   double sz0 = z;

   for(int i = 0; i<n; i++)
   {
     paddle[i] = new ScintillatorPaddle3D(i, height*i+sx0, 0.0+sy0, 0.0+sz0, length, height, thickness, paddles, number);
   }
   
   cout<<"Scintillation Plane 3D is created!"<<endl;
}

ScintPlane3D::~ScintPlane3D()
{

}

void ScintPlane3D::paddleHit(int num, double left, double right)
{
  GetVariables *ptr = new GetVariables("HMS.txt");
  int numb = ptr->GetInt("Number of Paddle PMTs =");
  if (num<N)
    {
      
      paddle[num]->hit(left,right,numb);
    }
}

void ScintPlane3D::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }
}
