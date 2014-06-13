
//************************************************************************* 
//  Scintillator.h - Monday, June 9/14
// 
//  by Ben Davis-Purcell
// 
// This class is used to create the planar view of a single scintillator paddle.
//
//
//*************************************************************************  

#ifndef ROOT_scintillator
#define ROOT_scintillator

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"

class Scintillator {
  
public:
  Scintillator(int index, double xpos, double ypos, double pmtx, double pmty, double length, int pmtnum);
  virtual ~Scintillator();
  void hit(double left, double right, double ypos);
  void clear();

protected:
  double sx0, sy0,spmtx,spmty;  // starting positions for coordinates
  double paddle_length;
  int pmtnum;                //# of PMTS on scintillator (1 or 2)
  TPolyLine *plineL;
  TPolyLine *plineR;
  TPolyLine *scintillator; 
  TArrow *yindicator;
  TLatex *index_text;
};

#endif
