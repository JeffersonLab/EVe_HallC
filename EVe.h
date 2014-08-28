//************************************************************************* 
//  EVe.h  - 4/14/2008
// 
//  by miham
// 
//  This is the main class of the event display. 
// 
//
//************************************************************************* 

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

#include "WirePlane.h"
#include "MWDChamber.h"
#include "CStransform.h"
#include "MultiRoads.h"
#include "Track.h"
#include "ScintPlane.h"
#include "Detector3D.h"
#include "THaRun.h"
#include <TString.h>
#include <TGFileDialog.h>
#include <TStyle.h>



class EVe{
  RQ_OBJECT("EVe")
public:
  EVe(const TGWindow *p, UInt_t w, UInt_t h);
  ~EVe();
  void CreateXprojection();
  void CreateUVprojection();
  void CreateYprojection();
  void CreateWires();
  void Create3DView();
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
  TGRadioButton *fTextButtonXPlane;
  TGRadioButton *fTextButtonUVPlane;
  TGRadioButton *fTextButtonYPlane;
  TGRadioButton *fTextButton3dView;
  TGRadioButton *fTextButtonProj;	

  TGRadioButton *fTextButtonWires;
  TGCheckButton *fTextButtonTrackProj;
  TGCheckButton *fTextButtonTrack;
  TGCheckButton *fTextButtonRoads;
  TGNumberEntry *fNumberEntry1;

  MWDChamber *mwdc1;
  MWDChamber *mwdc2;

  ScintPlane *s1X;
  ScintPlane *s1Y;
  ScintPlane *s2X;
  ScintPlane *s2Y;

  int EventNumber;
  int TotalNumberOfEvents;

  WirePlane *x1;
  WirePlane *x1p;
  WirePlane *x2;
  WirePlane *x2p;

  WirePlane *u1;
  WirePlane *v1;
  WirePlane *u2;
  WirePlane *v2;

  WirePlane *y1;
  WirePlane *y1p;
  WirePlane *y2;
  WirePlane *y2p;
 
  CStransform *cst;
  CStransform *s2x_cst;
  CStransform *s2y_cst;
  MultiRoads *umrd;
  Track *utr[MAX_ROADS_NUM];
  MultiRoads *vmrd;
  Track *vtr[MAX_ROADS_NUM];
  MultiRoads *xmrd;
  Track *xtr[MAX_ROADS_NUM];

  TTree *t1;

  /// MWDC1 nhits
  Int_t Ndata_H_dc_1u1_tdchits;
  Int_t Ndata_H_dc_1v1_tdchits;
  Int_t Ndata_H_dc_1x1_tdchits;
  Int_t Ndata_H_dc_1x2_tdchits;
  Int_t Ndata_H_dc_1y1_tdchits;
  Int_t Ndata_H_dc_1y2_tdchits;

  /// MWDC1 tdc hits
  Double_t H_dc_1u1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_1v1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_1x1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_1x2_tdchits[MAX_HITS_NUM];
  Double_t H_dc_1y1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_1y2_tdchits[MAX_HITS_NUM];

  /// MWDC1 hit times
  Double_t H_dc_1u1_time[MAX_HITS_NUM];
  Double_t H_dc_1v1_time[MAX_HITS_NUM];
  Double_t H_dc_1x1_time[MAX_HITS_NUM];
  Double_t H_dc_1x2_time[MAX_HITS_NUM];
  Double_t H_dc_1y1_time[MAX_HITS_NUM];
  Double_t H_dc_1y2_time[MAX_HITS_NUM];

  /// MWDC2 nhits
  Int_t Ndata_H_dc_2u1_tdchits;
  Int_t Ndata_H_dc_2v1_tdchits;
  Int_t Ndata_H_dc_2x1_tdchits;
  Int_t Ndata_H_dc_2x2_tdchits;
  Int_t Ndata_H_dc_2y1_tdchits;
  Int_t Ndata_H_dc_2y2_tdchits;

  /// MWDC2 hit tdc hits
  Double_t H_dc_2u1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_2v1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_2x1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_2x2_tdchits[MAX_HITS_NUM];
  Double_t H_dc_2y1_tdchits[MAX_HITS_NUM];
  Double_t H_dc_2y2_tdchits[MAX_HITS_NUM];

  /// MWDC2 hit times
  Double_t H_dc_2u1_time[MAX_HITS_NUM];
  Double_t H_dc_2v1_time[MAX_HITS_NUM];
  Double_t H_dc_2x1_time[MAX_HITS_NUM];
  Double_t H_dc_2x2_time[MAX_HITS_NUM];
  Double_t H_dc_2y1_time[MAX_HITS_NUM];
  Double_t H_dc_2y2_time[MAX_HITS_NUM];


  /// TRACKING
  Int_t Ndata_H_tr_x;
  Double_t H_tr_x[MAX_TRACK_NUM];
  Double_t H_tr_y[MAX_TRACK_NUM];
  Double_t H_tr_ph[MAX_TRACK_NUM];
  Double_t H_tr_th[MAX_TRACK_NUM];



  // variables for scintillation planes

  Int_t Ndata_H_hod_1x_negtdchits;
  Int_t Ndata_H_hod_1x_postdchits;
  Double_t H_hod_1x_negtdchits[16];
  Double_t H_hod_1x_postdchits[16];
  Int_t Ndata_H_hod_1y_negtdchits;
  Int_t Ndata_H_hod_1y_postdchits;
  Double_t H_hod_1y_negtdchits[16];
  Double_t H_hod_1y_postdchits[16];
  Int_t Ndata_H_hod_2x_negtdchits;
  Int_t Ndata_H_hod_2x_postdchits;
  Double_t H_hod_2x_negtdchits[16];
  Double_t H_hod_2x_postdchits[16];
  Int_t Ndata_H_hod_2y_negtdchits;
  Int_t Ndata_H_hod_2y_postdchits;
  Double_t H_hod_2y_negtdchits[16];
  Double_t H_hod_2y_postdchits[16];

  Detector3D *detector;

  int run_number;
  const char *crun_number;

  const char *graph_title;

  TLatex *title;



};
#endif
