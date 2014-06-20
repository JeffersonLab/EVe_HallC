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
  ScintillatorPaddle(int index, double x, double y, double a, double b, double length, int PMTn);
  virtual ~ScintillatorPaddle();
  void hit(double left, double right, double y);
  void clear();

 protected:
  double x0, y0, pa, pb;
  double paddle_length;
  int n;
  TPolyLine *plineL;
  TPolyLine *plineR;
  TPolyLine *scint; 
  TArrow *yindicator;
  TLatex *index_text;
};

#endif
