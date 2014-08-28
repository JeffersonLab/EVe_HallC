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
  ScintillatorPaddle(int index, double x, double y, double a, double b, double length, int PMTn, int rotation);
  virtual ~ScintillatorPaddle();
  void hit(double left, double right, double y);
  void HitLeft();
  void HitRight();
  void HitBoth();
  void clear();

 protected:
  double sx0, sy0, sa, sb;
  double paddle_length;
  int n, rot;
  TPolyLine *plineL;
  TPolyLine *plineR;
  TPolyLine *scint; 
  TArrow *yindicator;
  TLatex *index_text;
};

#endif
