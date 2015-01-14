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

ScintPlane::ScintPlane(char *name, int n, double plength, double pheight, double PMTlength, CStransform *trans, double ang)
{
 
  // Converting plenth & pheight [m] into pixels

  cst = trans;
  N = n;
  paddle_length = plength;
  paddle_height = pheight;
  PMTl=cst->transLtoCL(PMTlength);
  //horiz = horizontal;
  angle=ang;
  std::string geometry = "HMS.txt";
  GetVariables *pmt = new GetVariables(geometry);
  

  //// FIXME:: convert to HMS units
  double fpaddleH = 0.25;  ///  0.25 ????
  //double fpaddleL = 0.5625;  /// 0.5625 before -- why these values??
  int numPMT = pmt->GetInt("Number of paddle PMTs =");

  // cout << horiz << " horiiz" << endl;
  /*if (horiz == 1) {
    rot = 1;
  }
  else {
    rot =0;
    }*/

  /// FIXME:: Conversion between canvas units and actual units goes back and forth. Rotation hard-coded into ScintillatorPaddle class but does not match up with this class - scaling factors need to be fixed

  sx0 = cst->transXtoCX(0.0);                                    ; 
  sy0 = cst->transYtoCY(0.0);                                                        ;
    double cx0= - cst->transLtoCL(paddle_length/2.0);
    double cy0=  cst->transLtoCL(paddle_height*(N)/2.0 - paddle_height);
  
  double CL = cst->transLtoCL(paddle_length);
  double CH = cst->transLtoCL(paddle_height);  
   

  //sa = CL/fpaddleL;
  
  //Consider sxpaddle length is different from sypaddle length
  sa=CL;
  sb = CH/fpaddleH; 
  /// sa and sb should be removed and left as CL and CH to be read into 
  /// ScintillatorPaddle -- scaling needs to be fixed in both classes

  for (int i=0; i<n ;i++){
    paddle[i]=new ScintillatorPaddle(i, sx0, sy0, sa ,sb ,cx0,cy0-i*CH, numPMT, PMTl,angle);} 
          title = new TLatex(sx0-0.2*sa, sy0-2.40*sb, name);
          title->SetTextSize(0.03);
          title->Draw(); 
  /*if (horiz == 1) {
    for(int i=0;i<n;i++) {
      paddle[i] = new ScintillatorPaddle(i, sx0, sy0-i*CH, sa, sb, paddle_length, numPMT, rot);} 
          title = new TLatex(sx0+sa*0.35-sa*0.3,sy0+sb*0.35, name);
          title->SetTextSize(0.03);
          title->Draw();
    
  }
  else {
    for(int i=0;i<n;i++) {
      paddle[i] = new ScintillatorPaddle(i, sx0, sy0+i*CH, sa, sb, paddle_length, numPMT, rot);
    }
          title = new TLatex(sy0, sx0-1.5*sb, name);
          title->SetTextSize(0.03);
          title->Draw();  
	  }*/
  
  
       
  
  for(int i = 0; i<MAX_TRACK_NUM; i++)track_circ[i] = new TEllipse(0,0,0);
   
  cout<<"Scintillation Plane is created!"<<endl;
}


ScintPlane::~ScintPlane()
{
  // Destructor
}

/* paddleHit() will turn on PMTs for paddle <num> if the TDC data in <left> and <right> correspond to a good hit.
 */ // -- OLD hit method
void ScintPlane::paddleHit(int num, double left, double right, double y)
{
  if (num<N)
    {
      paddle[num]->hit(left,right,y);
    }
  
}


/// NEW hit method used for tdchits. Needs to be implemented in ScintPlane3D
void ScintPlane::paddleLeftHit(int padn)
{ 
  if(padn<0) cerr << "Negative paddle index" << padn <<" \n";
  else
  paddle[padn]->HitLeft();

}


void ScintPlane::paddleRightHit(int pad)
{ if(pad<0) cerr << "Negative paddle index" << pad <<" \n";
  else
  paddle[pad]->HitRight();
  
}

void ScintPlane::paddleBothHit(int padnum)
{if(padnum<0) cerr << "Negative paddle index" << padnum <<" \n";
  else
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
  double CX =  cst->transXtoCX(x); 
  double CY =  cst->transYtoCY(y); 	

  track_circ[i]->SetX1(CX);
  track_circ[i]->SetY1(CY);
  track_circ[i]->SetR1(0.008);
  track_circ[i]->SetR2(0.008);
  track_circ[i]->SetLineColor(1+i);
  track_circ[i]->SetLineWidth(2);
  track_circ[i]->Draw();

}
