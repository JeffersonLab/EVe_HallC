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
#include "ScintilationPlane.h"
#include "BigBiteDetector3D.h"
#include "THaRun.h"
#include <TString.h>
#include <TGFileDialog.h>
#include <TStyle.h>



class EVe{
  RQ_OBJECT("EVe")
public:
  EVe(const TGWindow *p, UInt_t w, UInt_t h);
  ~EVe();
  void CreateUprojection();
  void CreateVprojection();
  void CreateXprojection();
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
  TGRadioButton *fTextButtonUPlane;
  TGRadioButton *fTextButtonVPlane;
  TGRadioButton *fTextButtonXPlane;
  TGRadioButton *fTextButton3dView;
  TGRadioButton *fTextButtonProj;	

  TGRadioButton *fTextButtonWires;
  TGCheckButton *fTextButtonTrackProj;
  TGCheckButton *fTextButtonTrack;
  TGCheckButton *fTextButtonRoads;
  TGNumberEntry *fNumberEntry1;

  MWDChamber *mwdc1;
  MWDChamber *mwdc2;

  ScintilationPlane *sdE;
  ScintilationPlane *sE;

  int EventNumber;
  int TotalNumberOfEvents;


  WirePlane *u1;
  WirePlane *u1p;
  WirePlane *u2;
  WirePlane *u2p;

  WirePlane *v1;
  WirePlane *v1p;
  WirePlane *v2;
  WirePlane *v2p;

  WirePlane *x1;
  WirePlane *x1p;
  WirePlane *x2;
  WirePlane *x2p;
 
  CStransform *cst;
  MultiRoads *umrd;
  Track *utr[MAX_ROADS_NUM];
  MultiRoads *vmrd;
  Track *vtr[MAX_ROADS_NUM];
  MultiRoads *xmrd;
  Track *xtr[MAX_ROADS_NUM];

  TTree *t1;

    // Prva MWDC komora
  Double_t B_mwdc_u1_nhits;
  Double_t B_mwdc_u1p_nhits;
  Double_t B_mwdc_v1_nhits;
  Double_t B_mwdc_v1p_nhits;
  Double_t B_mwdc_x1_nhits;
  Double_t B_mwdc_x1p_nhits;

  Double_t B_mwdc_u1_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_u1p_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_v1_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_v1p_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_x1_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_x1p_hit_wire[MAX_HITS_NUM];

  Double_t B_mwdc_u1_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_u1p_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_v1_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_v1p_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_x1_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_x1p_hit_time[MAX_HITS_NUM];
  
    
  // Druga MWDC komora
  
  Double_t B_mwdc_u2_nhits;
  Double_t B_mwdc_u2p_nhits;
  Double_t B_mwdc_v2_nhits;
  Double_t B_mwdc_v2p_nhits;
  Double_t B_mwdc_x2_nhits;
  Double_t B_mwdc_x2p_nhits;

  Double_t B_mwdc_u2_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_u2p_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_v2_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_v2p_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_x2_hit_wire[MAX_HITS_NUM];
  Double_t B_mwdc_x2p_hit_wire[MAX_HITS_NUM];
  
  Double_t B_mwdc_u2_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_u2p_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_v2_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_v2p_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_x2_hit_time[MAX_HITS_NUM];
  Double_t B_mwdc_x2p_hit_time[MAX_HITS_NUM];


  // U projekcija
  
  Double_t B_mwdc_u_nroads;

  Double_t B_mwdc_u_rd_zL[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_zU[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_xUL[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_xUR[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_xLL[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_xLR[MAX_ROADS_NUM];

  Double_t B_mwdc_u_rd_good[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_pos[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_slope[MAX_ROADS_NUM];
  Double_t B_mwdc_u_rd_chi2[MAX_ROADS_NUM];

  // V projekcija
  
  Double_t B_mwdc_v_nroads;

  Double_t B_mwdc_v_rd_zL[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_zU[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_xUL[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_xUR[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_xLL[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_xLR[MAX_ROADS_NUM];

  Double_t B_mwdc_v_rd_good[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_pos[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_slope[MAX_ROADS_NUM];
  Double_t B_mwdc_v_rd_chi2[MAX_ROADS_NUM];

  // X projekcija
  
  Double_t B_mwdc_x_nroads;

  Double_t B_mwdc_x_rd_zL[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_zU[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_xUL[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_xUR[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_xLL[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_xLR[MAX_ROADS_NUM];

  Double_t B_mwdc_x_rd_good[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_pos[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_slope[MAX_ROADS_NUM];
  Double_t B_mwdc_x_rd_chi2[MAX_ROADS_NUM];

  Double_t B_mwdc_u_ngood;
  Double_t B_mwdc_v_ngood; 
  Double_t B_mwdc_x_ngood;
 
  Double_t B_tr_n;
  Double_t B_tr_x[MAX_TRACK_NUM];
  Double_t B_tr_y[MAX_TRACK_NUM];
  Double_t B_tr_ph[MAX_TRACK_NUM];
  Double_t B_tr_th[MAX_TRACK_NUM];

  Double_t B_tr_px[MAX_TRACK_NUM];
  Double_t B_tr_py[MAX_TRACK_NUM];
  Double_t B_tr_pz[MAX_TRACK_NUM];
  Double_t B_tr_p[MAX_TRACK_NUM];




  // variables for scintilation plane
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


  BigBiteDetector3D *bigbite;

  int run_number;
  const char *crun_number;

  const char *graph_title;

  TLatex *title;






};
#endif
