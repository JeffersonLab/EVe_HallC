///////////////////////////////////////
/*  Eve.h  6/13/14
    
    Ben Davis-Purcell

    Event Display main class header file

*/
///////////////////////////////////////

#ifndef ROOT_Eve
#define ROOT_Eve


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

#include "CStransform.h"
#include "ScintPlane.h"
#include "THaRun.h"
#include <TString.h>
#include <TGFileDialog.h>
#include <TStyle.h>


class Eve{
  RQ_OBJECT("Eve")
 public:
  Eve(const TGWindow *p,UInt_t w, UInt_t h);
  ~Eve();
  

  void CreateWires();
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

  ScintPlane *sdE;
  ScintPlane *sE;


  int EventNumber;
  int TotalNumberOfEvents;


  CStransform *cst;

  TTree *t1;


  // variables for scintillation plane
  Double_t B_tp_e_nhit;
  Double_t B_tp_e_LT[25];
  Double_t B_tp_e_RT[25];
  Double_t B_tp_e_hit_ypos[25];
  Double_t B_tp_e_hit_bar[25];
  
  
  Double_t B_tp_de_nhit;
  Double_t B_tp_de_LT[25];
  Double_t B_tp_de_RT[25];
  Double_t B_tp_de_hit_ypos[25];
  Double_t B_tp_de_hit_bar[25];


  int run_number;
  const char *crun_number;
  
  const char *graph_title;
  
  TLatex *title;





};
#endif
