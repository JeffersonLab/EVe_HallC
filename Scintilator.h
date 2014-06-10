
//************************************************************************* 
//  Scintilator.h - 4/14/2008
// 
//  by miham
// 
//  This class is used to create planar view one scintillation paddle.
//
//
//*************************************************************************  

#ifndef ROOT_scintilator
#define ROOT_scintilator

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"

class Scintilator {
  
public:
  Scintilator(int index, double x, double y, double a, double b, double length);
  virtual ~Scintilator();
  void hit(double left, double right, double y);
  void clear();

protected:
  double sx0, sy0,sa,sb;
  double paddle_length;
  TPolyLine *plineL;
  TPolyLine *plineR;
  TPolyLine *scintilator; 
  TArrow *yindicator;
  TLatex *index_text;
};

#endif
