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

ScintPlane::ScintPlane(char *name, int n, double plength, double pheight, CStransform *trans, int horizontal)
{
 
  // Converting plenth & pheight [m] into pixels

  cst = trans;
  N = n;
  paddle_length = plength;
  paddle_height = pheight;
  horiz = horizontal;
  std::string geometry = "HMS.txt";
  GetVariables *pmt = new GetVariables(geometry);
  

  //// FIXME:: convert to HMS units
  double fpaddleH = 0.25;
  double fpaddleL = 0.5625;
  int numPMT = pmt->GetInt("Number of paddle PMTs =");

  // cout << horiz << " horiiz" << endl;
  if (horiz == 1) {
    rot = 1;
  }
  else {
    rot =0;
  }

  /// Conversion between canvas units and actual units goes back and forth. Rotation hard-coded into ScintillatorPaddle class but does not match up with this class - scaling factors need to be fixed

  sx0 = cst->transXtoCX(0.0) - cst->transLtoCL(paddle_length/2.0); 
  sy0 = cst->transYtoCY(0.0) + cst->transLtoCL(paddle_height*(N)/2.0 - paddle_height);
  
  double CL = cst->transLtoCL(paddle_length);
  double CH = cst->transLtoCL(paddle_height);   

  sa = CL/fpaddleL;
  sb = CH/fpaddleH; 

  if (horiz == 1) {
    for(int i=0;i<n;i++) {
      paddle[i] = new ScintillatorPaddle(i, sx0, sy0-i*fpaddleH*sb, sa, sb, paddle_length, numPMT, rot);
    }
  }
  else {
    for(int i=0;i<n;i++) {
      paddle[i] = new ScintillatorPaddle(i, sx0, sy0+i*fpaddleH*sb, sa, sb, paddle_length, numPMT, rot);
    }
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

/* 
 * paddleHit() will turn on PMTs for paddle <num> if the TDC data in <left> and <right> 
 * correspond to a good hit.
 */
void ScintPlane::paddleHit(int num, double left, double right, double y)
{
  if (num<N)
    {
      paddle[num]->hit(left,right,y);
    }
  
}

void ScintPlane::paddleLeftHit(int padn)
{
  paddle[padn]->HitLeft();

}


void ScintPlane::paddleRightHit(int pad)
{
  paddle[pad]->HitRight();
  
}

void ScintPlane::paddleBothHit(int padnum)
{
  paddle[padnum]->HitBoth();
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
  double CX =  cst->transXtoCX(x);  /// -y ??
  double CY =  cst->transYtoCY(y);  /// -x ??	

  track_circ[i]->SetX1(CX);
  track_circ[i]->SetY1(CY);
  track_circ[i]->SetR1(0.008);
  track_circ[i]->SetR2(0.008);
  track_circ[i]->SetLineColor(1+i);
  track_circ[i]->SetLineWidth(2);
  track_circ[i]->Draw();

}
