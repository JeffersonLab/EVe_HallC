///////////////////////////////////////
/*  Eve.h  6/13/14
    
    Ben Davis-Purcell

    Event Display main class header file

*/
///////////////////////////////////////

#ifndef ROOT_EVe
#define ROOT_EVe


#include "TCanvas.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TGButtonGroup.h"
#include "TGNumberEntry.h"
#include <RQ_OBJECT.h>
#include "TGClient.h"
#include "TTree.h"
#include "TFile.h"

#include "THaRun.h"
#include <TString.h>
#include <TGFileDialog.h>
#include <TStyle.h>


class Eve{
  RQ_OBJECT("Eve");
 public:
  Eve(const TGWindow *p,UInt_t w, UInt_t h);
  ~Eve();
  
  void initRun(char *filename);
  void DoDraw(int event);
  void doNext();
  void doNextGood();
  void doPrevious();
  void doPreviousGood();
  void doThis();
  void SelectProj();
  void MyCloseWindow();
  void PrintToFile();



 private:
  TGMainFrame *fMainFrame;
  TRootEmbeddedCanvas *fRootEmbeddedCanvas;
  TCanvas *c1;
  TCanvas *c2;
  TCanvas *c3;
  TCanvas *c4;
  TCanvas *c5;

  TGRadioButton *fTextButtonProj;  


  TGNumberEntry *fNumberEntry1; 




  int EventNumber;
  int TotalNumberOfEvents;


  CStransform *cst;

  TTree *t1;








}
#endif
