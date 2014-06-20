///////////////////////////////////////
/*  ScintPlane.cxx  6/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddles to create a plane

*/
///////////////////////////////////////

#include "ScintPlane.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

ScintPlane::ScintPlane(char *name, int n, double plength, double pheight, CStransform *trans)
{
 
  // Converting plenth & pheight [m] into pixels

  cst = trans;
  N = n;
  paddle_length = plength;
  paddle_height = pheight;

  double fpaddleH = 0.25;
  double fpaddleL = 0.5625;


  sx0 = cst->transXtoCX(0.0) - cst->transLtoCL(paddle_length/2.0); 
  sy0 = cst->transYtoCY(0.0) + cst->transLtoCL(paddle_height*(N)/2.0 - paddle_height);
  
  double CL = cst->transLtoCL(paddle_length);
  double CH = cst->transLtoCL(paddle_height);   

  sa = CL/fpaddleL;
  sb = CH/fpaddleH; 

  for(int i=0;i<n;i++) {
    paddle[i] = new ScintillatorPaddle(i, sx0, sy0-i*fpaddleH*sb, sa, sb, paddle_length, 2);
  }


  title = new TLatex(sx0+sa*0.35-sa*0.3,sy0+sb*0.35, name);
  title->SetTextSize(0.03);
  title->Draw();      

  for(int i = 0; i<MAX_TRACK_NUM; i++)track_circ[i] = new TEllipse(0,0,0);
   
  cout<<"Scintillation Plane is created!"<<endl;
}


ScintPlane::~ScintPlane()
{
  // Destructor
}

void ScintPlane::paddleHit(int num, double left, double right, double y)
{
  if (num<N)
    {
      paddle[num]->hit(left,right,y);
    }
  
}

void ScintPlane::clear()
{
  for(int i = 0; i<N; i++) {
    paddle[i]->clear();
  }

  for(int i=0;i<MAX_TRACK_NUM;i++) {
    track_circ[i]->SetX1(0.0);
    track_circ[i]->SetY1(0.0);
    track_circ[i]->SetR1(0.0);
    track_circ[i]->SetR2(0.0);
    track_circ[i]->SetFillColor(kWhite);
    track_circ[i]->Draw();
    
    delete track_index[i];
  }
}

void ScintPlane::Track(double x, double y, int i)
{
  double CX =  cst->transXtoCX(-y);
  double CY =  cst->transYtoCY(-x);	

  track_circ[i]->SetX1(CX);
  track_circ[i]->SetY1(CY);
  track_circ[i]->SetR1(0.008);
  track_circ[i]->SetR2(0.008);
  track_circ[i]->SetLineColor(1+i);
  track_circ[i]->SetLineWidth(2);
  track_circ[i]->Draw();

}
