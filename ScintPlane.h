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
#include "EVe_DB.h"

class ScintPlane {
 public: 
  ScintPlane(char *name, int n, double plength, double pheight,double PMTlength,  CStransform *trans, double ang);
  virtual ~ScintPlane();
  void paddleLeftHit(int padn);
  void paddleRightHit(int pad);
  void paddleBothHit(int padnum);
  void Track(double x, double y, int i);
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
  ScintillatorPaddle *paddle[MAX_PADDLE_NUM];
  //GetVariables myvars;
  TLatex *title;
  CStransform *cst;
  TEllipse *track_circ[MAX_TRACK_NUM];
  TLatex *track_index[MAX_TRACK_NUM];

};

#endif
