///////////////////////////////////////
/*  ScintillatorPaddle.h  6/16/14
    
    Ben Davis-Purcell

    Class that creates a single (planar) scintillation paddle with attached PMTs

*/
///////////////////////////////////////

#ifndef ROOT_ScintillatorPaddle
#define ROOT_ScintillatorPaddle

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"

class ScintillatorPaddle {

 public:
  ScintillatorPaddle(int index, double x, double y, double a, double b, double cx0,double cy0, int PMTn, double PMTlength, double ang);
  virtual ~ScintillatorPaddle();
  void HitLeft();
  void HitRight();
  void HitBoth();
  void clear();

 protected:
  double sx0, sy0, sa, sb, pl;
  double paddle_length;
  int n;
  double angle;
  TPolyLine *plineL;
  TPolyLine *plineR;
  TPolyLine *scint; 
  TArrow *yindicator;
  TLatex *index_text;
};

#endif
