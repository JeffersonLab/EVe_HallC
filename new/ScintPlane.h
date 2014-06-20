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
#include "Eve_DB.h"

class ScintPlane {
 public: 
  ScintPlane(char *name, int n, double plength, double pheight, CStransform *trans);
  virtual ~ScintPlane();
  void paddleHit(int num, double left, double right, double y);
  void Track(double x, double y, int i);
  void clear();
  

protected:
  int N; // number of paddles
  double sx0, sy0,sa,sb;
  double paddle_length;
  double paddle_height;
  ScintillatorPaddle *paddle[MAX_PADDLE_NUM];
  TLatex *title;
  CStransform *cst;
  TEllipse *track_circ[MAX_TRACK_NUM];
  TLatex *track_index[MAX_TRACK_NUM];

};

#endif
