//************************************************************************* 
//  ScintillationPlane.cxx  - Monday, June 9/14
// 
//  by Ben Davis-Purcell
// 
//  This class is used to create planar view of whole scintillation plane
//  that consists of n scintillation paddles
//
//
//*************************************************************************   

#include "ScintillationPlane.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

#define DEBUG_LEVEL 0

using namespace std;



ScintillationPlane::ScintillationPlane(char *ime, int n,  double plength, double pheight, CStransform *trans)
{
   // Variables plength and pheight are in [m]. Now we need to transform these to variables
   // from meters to pixels.
   
   cst = trans;
   N = n;
   paddle_length = plength;
   paddle_height = pheight;

   //Basically a paddle - draw dimensions
   double fpaddleH = 0.25;
   double fpaddleL = 0.5625;
   // Now it should be placed in a paddle criterion - all these transformations
   // Then the really big paddle as much as it should be
   
   // now we make proper transformations to get to the left upper corner.
   sx0 = cst->transXtoCX(0.0) - cst->transLtoCL(paddle_length/2.0); 
   sy0 = cst->transYtoCY(0.0) + cst->transLtoCL(paddle_height*(N)/2.0 - paddle_height);

   double CL = cst->transLtoCL(paddle_length);
   double CH = cst->transLtoCL(paddle_height);



   sa = CL/fpaddleL;
   sb = CH/fpaddleH; 

   for(int i = 0; i<n; i++)
   {

     paddle[i] = new Scintillator(i, sx0, sy0-i*fpaddleH*sb,sa,sb, paddle_length);		
   }
    
   title = new TLatex(sx0+sa*0.35-sa*0.3,sy0+sb*0.35, ime);
   title->SetTextSize(0.03);
   title->Draw();      

   for(int i = 0; i<MAX_TRACK_NUM; i++)track_circ[i] = new TEllipse(0,0,0);
   
   cout<<"Scintillation Plane is created!"<<endl;
}


ScintillationPlane::~ScintillationPlane()
{

}

void ScintillationPlane::paddleHit(int num, double left, double right, double y)
{
  if (num<N)
  {
    paddle[num]->hit(left,right,y);
  }
  else
  { 
#if DEBUG_LEVEL >= 3	
    cout<<"There is no paddle with this index!"<<endl;
#endif
  }
}

void ScintillationPlane::clear()
{
  for(int i = 0; i<N; i++)
  {
    paddle[i]->clear();
  }

	for (int i = 0; i<MAX_TRACK_NUM; i++)
	{
          track_circ[i]->SetX1(0.0);
	  track_circ[i]->SetY1(0.0);
	  track_circ[i]->SetR1(0.0);
	  track_circ[i]->SetR2(0.0);
	  track_circ[i]->SetFillColor(kWhite);
     	  track_circ[i]->Draw();

	  delete track_index[i];
	}
}

void ScintillationPlane::Track(double x, double y, int i)
{

#if DEBUG_LEVEL >= 3	
	cout<<"HURA: TRACK: (x,y): "<<x<<", "<<y<<endl;
#endif
	// First we need to transform meters to pixels
	double CX =  cst->transXtoCX(-y);
   	double CY =  cst->transYtoCY(-x);	

	track_circ[i]->SetX1(CX);
	track_circ[i]->SetY1(CY);
	track_circ[i]->SetR1(0.008);
	track_circ[i]->SetR2(0.008);
	track_circ[i]->SetLineColor(1+i);
	track_circ[i]->SetLineWidth(2);
	//track_circ[i]->SetFillColor(kOrange+0-i);
     	track_circ[i]->Draw();

}

