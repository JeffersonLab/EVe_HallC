///////////////////////////////////////
/*  ScintPlane.h  6/17/14
    
    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddles to create a plane

*/
///////////////////////////////////////

#ifndef ROOT_ScintPlane
#define ROOT_ScintPlane

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "ScintillatorPaddle.h"
#include "TLatex.h"
#include "CStransform.h"
#include "TEllipse.h"
#include "GetVariables.h"

#define MAX_PADDLE_NUM 100
#define MAX_TRACK_NUM 77

class ScintPlane {
 public: 
  ScintPlane(char* splaneName, CStransform *trans);
  virtual ~ScintPlane();
  void paddleLeftHit(int padn);
  void paddleRightHit(int pad);
  void paddleBothHit(int padnum);
  void Track(double x, double y, int i);
  void SPHit2D(int NumL, int NumR, double poshit[], double neghit[]);
  void clear();
  

protected:
  int N; // number of paddles
  //int horiz; // 1 if scint plane has horizontally stacked paddles, 0 if vertical
  //int rot; // defines horiz for individual scint paddles
  double angle;
  double sx0, sy0,sa,sb;
  double paddle_length;
  double paddle_height;
  double PMTl;
  char* splaneName;
  ScintillatorPaddle *paddle[MAX_PADDLE_NUM];
  //GetVariables myvars;
  TLatex *title;
  CStransform *cst;
  TEllipse *track_circ[MAX_TRACK_NUM];
  TLatex *track_index[MAX_TRACK_NUM];

};

#endif
