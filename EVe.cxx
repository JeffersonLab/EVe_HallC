
//************************************************************************* 
//  EVe.cxx  - 4/14/2008
// 
//  by miham
// 
//  This is the main class of the event display. 
// 
//
//*************************************************************************


// TODO: FULL tracks are not working yet poperly, because, I dont know
// precise positions of the target, chambers etc. 

#include "EVe.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <TApplication.h>
#include <TROOT.h>

#include "EVe_DB.h"

#define DEBUG_LEVEL  0

// If you want full tracks set FULL_TRACK = 1, but for this you will also need some //additional variables.
#define FULL_TRACK 0



using namespace std;


EVe::EVe(const TGWindow *p, UInt_t w, UInt_t h)
{
   // main frame
   //fMainFrame = new TGMainFrame(0,10,10,kMainFrame | kVerticalFrame);
   fMainFrame = new TGMainFrame(p, w, h, kMainFrame | kVerticalFrame);
   fMainFrame->SetLayoutBroken(kTRUE);
   fMainFrame->SetWindowName("Event ViewEr - EVE");
   fMainFrame->Connect("CloseWindow()", "EVe", this, "MyCloseWindow()");

   // embedded canvas
   fRootEmbeddedCanvas = new TRootEmbeddedCanvas(0,fMainFrame,h-16, h-16);
   Int_t wfRootEmbeddedCanvas = fRootEmbeddedCanvas->GetCanvasWindowId();
   c1 = new TCanvas("c1", 10, 10, wfRootEmbeddedCanvas);
   c2 = new TCanvas("c2", 10, 10, wfRootEmbeddedCanvas);
   c3 = new TCanvas("c3", 10, 10, wfRootEmbeddedCanvas);
   c4 = new TCanvas("c4", 10, 10, wfRootEmbeddedCanvas);
   c5 = new TCanvas("c5", 10, 10, wfRootEmbeddedCanvas);
   fMainFrame->AddFrame(fRootEmbeddedCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRootEmbeddedCanvas->MoveResize(8,8,h-16,h-16);


   //______________ First group fo the buttons____________________
   TGButtonGroup *fGroupFrame3 = new TGButtonGroup(fMainFrame,"View type:", kVerticalFrame);
   fGroupFrame3->SetLayoutBroken(kTRUE);

   fGroupFrame3->SetLayoutManager(new TGVerticalLayout(fGroupFrame3));
   fGroupFrame3->Resize(128,100);
  
   fTextButtonWires = new TGRadioButton(fGroupFrame3,"Planar view");
   fGroupFrame3->AddFrame(fTextButtonWires, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonWires->MoveResize(8,24,106,20);

   fTextButton3dView = new TGRadioButton(fGroupFrame3,"3D View");
   fGroupFrame3->AddFrame(fTextButton3dView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton3dView->MoveResize(8,40,106,20);

   fTextButtonProj = new TGRadioButton(fGroupFrame3,"Projection view");
   fGroupFrame3->AddFrame(fTextButtonProj, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonProj->MoveResize(8,56,106,20);

   fTextButtonWires->SetState(kButtonDown);
   //fTextButtonProj->SetState(kButtonDown);
   //fTextButton3dView->SetState(kButtonDown);

   fMainFrame->AddFrame(fGroupFrame3, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame3->MoveResize(w-16-128-128, 100,128,100);



   //______________ Second group fo the buttons____________________
   // Here are ratio buttons for selecting MWDC 
   TGButtonGroup *fGroupFrame1 = new TGButtonGroup(fMainFrame,"Projections:", kVerticalFrame);
   fGroupFrame1->SetLayoutBroken(kTRUE);
   fTextButtonXPlane = new TGRadioButton(fGroupFrame1,"X-Projection");
   fGroupFrame1->AddFrame(fTextButtonXPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonXPlane->MoveResize(8,24,106,20);
   
   fTextButtonUVPlane = new TGRadioButton(fGroupFrame1,"UV-Projection");
   fGroupFrame1->AddFrame(fTextButtonUVPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonUVPlane->MoveResize(8,40,106,20);

   fTextButtonYPlane = new TGRadioButton(fGroupFrame1,"Y-Projection");
   fGroupFrame1->AddFrame(fTextButtonYPlane, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonYPlane->MoveResize(8,56,106,20);

   // In the beginning the first button is down
   fTextButtonXPlane->SetState(kButtonDown);

   fGroupFrame1->SetLayoutManager(new TGVerticalLayout(fGroupFrame1));
   fGroupFrame1->Resize(128,120);
   fMainFrame->AddFrame(fGroupFrame1, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame1->MoveResize(w-8-128, 100,128, 100);

   //______________ Third group fo the buttons____________________
   // Here are check buttons for options 
   TGButtonGroup *fGroupFrame2 = new TGButtonGroup(fMainFrame,"Options", kVerticalFrame);
   fGroupFrame2->SetLayoutBroken(kTRUE);
   fTextButtonRoads = new TGCheckButton(fGroupFrame2,"Show Roads");
   fGroupFrame2->AddFrame(fTextButtonRoads, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonRoads->MoveResize(8,24,106,20);

   fTextButtonRoads->SetState(kButtonDown);

   fTextButtonTrackProj = new TGCheckButton(fGroupFrame2,"Tr. Projections");
   fGroupFrame2->AddFrame(fTextButtonTrackProj, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonTrackProj->MoveResize(8,40,106,20);
 
   fTextButtonTrackProj->SetState(kButtonDown);

   fTextButtonTrack = new TGCheckButton(fGroupFrame2,"Show Tracks");
   fGroupFrame2->AddFrame(fTextButtonTrack, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonTrack->MoveResize(8,56,106,20);
 
   fTextButtonTrack->SetState(kButtonDown);

   fGroupFrame2->SetLayoutManager(new TGVerticalLayout(fGroupFrame2));
   fGroupFrame2->Resize(128,96);
   fMainFrame->AddFrame(fGroupFrame2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame2->MoveResize(w-8-192,200,128,96);



   //________________________ Buttons___________________________________

   fNumberEntry1 = new TGNumberEntry(fMainFrame, (Double_t) 0,8,-1,(TGNumberFormat::EStyle) 5);
   fMainFrame->AddFrame(fNumberEntry1, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fNumberEntry1->MoveResize(150,380+246,72,22);
   fNumberEntry1->MoveResize(w - 260, 350,100,30);

   TGTextButton *fTextButtonSE = new TGTextButton(fMainFrame,"Show this Event");
   fMainFrame->AddFrame(fTextButtonSE, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonSE->MoveResize(230,380+246,100,30);
   fTextButtonSE->MoveResize(w-150, 350,100,30);

   TGTextButton *fTextButtonNext = new TGTextButton(fMainFrame,"Next");
   fMainFrame->AddFrame(fTextButtonNext, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonNext->MoveResize(630,380+246,104,30);
   fTextButtonNext->MoveResize(w-150, 390,100,30);

   TGTextButton *fTextButtonPrevious = new TGTextButton(fMainFrame,"Previous");
   fMainFrame->AddFrame(fTextButtonPrevious, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonPrevious->MoveResize(520,380+246,100,30);
   fTextButtonPrevious->MoveResize(w -260, 390 ,100,30);

   TGTextButton *fTextButtonNextGood = new TGTextButton(fMainFrame,"Next Good");
   fMainFrame->AddFrame(fTextButtonNextGood, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonNextGood->MoveResize(745,380+246,100,30);
   fTextButtonNextGood->MoveResize(w - 150, 430, 100,30);

   TGTextButton *fTextButtonPreviousGood = new TGTextButton(fMainFrame,"Previous Good");
   fMainFrame->AddFrame(fTextButtonPreviousGood, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonPreviousGood->MoveResize(380,380+246,130,30);
   fTextButtonPreviousGood->MoveResize(w - 260, 430, 100,30);


   TGTextButton *fTextButtonExit = new TGTextButton(fMainFrame,"E&xit","gApplication->Terminate(0)");
   fMainFrame->AddFrame(fTextButtonExit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonExit->MoveResize(w-150, h-50,100,30);

   TGTextButton *fTextButtonPrint = new TGTextButton(fMainFrame,"&Print");
   fMainFrame->AddFrame(fTextButtonPrint, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonPrint->MoveResize(w-260, h-50,100,30);

   fMainFrame->MapSubwindows();
   fMainFrame->Resize(fMainFrame->GetDefaultSize());
   fMainFrame->MapWindow();
   //fMainFrame->Resize(1000,678);
   fMainFrame->Resize(w,h);


   EventNumber = 0;
   cst = new CStransform(2.2, 0.5, 0.3);
   umrd = new MultiRoads(cst);
   for (int q = 0; q<MAX_ROADS_NUM; q++) utr[q] = new Track(cst);
   
   vmrd = new MultiRoads(cst);
   for (int q = 0; q<MAX_ROADS_NUM; q++) vtr[q] = new Track(cst);

   xmrd = new MultiRoads(cst);
   for (int q = 0; q<MAX_ROADS_NUM; q++) xtr[q] = new Track(cst);

   fTextButtonNext->Connect("Clicked()", "EVe", this, "doNext()");
   fTextButtonPrevious->Connect("Clicked()", "EVe", this, "doPrevious()");
   fTextButtonSE->Connect("Clicked()", "EVe", this, "doThis()");
   fTextButtonNextGood->Connect("Clicked()", "EVe", this, "doNextGood()");
   fTextButtonPreviousGood->Connect("Clicked()", "EVe", this, "doPreviousGood()");
   fTextButtonPrint->Connect("Clicked()", "EVe", this, "PrintToFile()");
  
   if (fTextButtonProj->IsOn())
   {
     if (fTextButtonXPlane->IsOn()) CreateXprojection(); 
     if (fTextButtonUVPlane->IsOn()) CreateUVprojection();
     if (fTextButtonYPlane->IsOn()) CreateYprojection();
     if (fTextButton3dView->IsOn()) Create3DView();
   }
   
   if (fTextButtonWires->IsOn()) CreateWires();
  
   if (fTextButton3dView->IsOn()) Create3DView();

   fTextButtonXPlane->Connect("Clicked()", "EVe", this, "CreateXprojection()");
   fTextButtonUVPlane->Connect("Clicked()", "EVe", this, "CreateUVprojection()");
   fTextButtonYPlane->Connect("Clicked()", "EVe", this, "CreateYprojection()");
   fTextButtonWires->Connect("Clicked()", "EVe", this, "CreateWires()");
   fTextButton3dView->Connect("Clicked()", "EVe", this, "Create3DView()");
   fTextButtonProj->Connect("Clicked()", "EVe", this, "SelectProj()");

   cout<<"EVe created!!!"<<endl;
}

EVe::~EVe()
{
  fMainFrame->Cleanup();
  delete fMainFrame;
}


void EVe::MyCloseWindow()
{
  fMainFrame->SendCloseMessage();
  gApplication->Terminate();
}


void EVe::Create3DView()
{
   fRootEmbeddedCanvas->AdoptCanvas(c5);
   c5->cd();
   c5->Clear();

   detector = new Detector3D();
   c5->Update();
}


void EVe::SelectProj()
{
     if (fTextButtonXPlane->IsOn()) CreateXprojection(); 
     if (fTextButtonUVPlane->IsOn()) CreateUVprojection();
     if (fTextButtonYPlane->IsOn()) CreateYprojection();
     if (fTextButton3dView->IsOn()) Create3DView();
}

void EVe::CreateXprojection()
{

 if (fTextButtonProj->IsOn())
 {
     fRootEmbeddedCanvas->AdoptCanvas(c1);
     c1->cd();
     c1->Clear();

     // u1 = new WirePlane("U1",142,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(uplane_z1),cst->transLtoCL(L1),1.0,1.0,-1);
   
     // u1p = new WirePlane("U1p",142,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(uplane_z1+0.002),cst->transLtoCL(L1),1.0,1.0,1);

     // u2 = new WirePlane("U2",201,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(uplane_z2),cst->transLtoCL(L2),1.0,1.0,-1);
  
     // u2p = new WirePlane("U2p",201,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(uplane_z2+0.002),cst->transLtoCL(L2),1.0,1.0,1);


     x1 = new WirePlane((char*)"X1",MWDC1_X,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(x1_dist),cst->transLtoCL(L1),1.0,1.0,-1);
   
     x1p = new WirePlane((char*)"X1p",MWDC1_Xp,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(x1p_dist),cst->transLtoCL(L1),1.0,1.0,+1);

     x2 = new WirePlane((char*)"X2",MWDC1_X,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(x2_dist),cst->transLtoCL(L2),1.0,1.0,-1);
  
     x2p = new WirePlane((char*)"X2p",MWDC1_Xp,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(x2p_dist),cst->transLtoCL(L2),1.0,1.0,+1);

     c1->Update();
  }
}

void EVe::CreateUVprojection()
{
  if (fTextButtonProj->IsOn())
  {

     fRootEmbeddedCanvas->AdoptCanvas(c2);
     c2->cd();
     c2->Clear();

     u1 = new WirePlane((char*)"U1",MWDC1_U,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(u1_dist),cst->transLtoCL(L1),1.0,1.0,-1);
     v1 = new WirePlane((char*)"V1",MWDC1_V,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(v1_dist+0.002),cst->transLtoCL(L1),1.0,1.0,+1);

     u2 = new WirePlane((char*)"U2",MWDC1_U,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(u2_dist),cst->transLtoCL(L2),1.0,1.0,-1);
     v2 = new WirePlane((char*)"V2",MWDC1_V,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(v2_dist+0.002),cst->transLtoCL(L2),1.0,1.0,+1);

     c2->Update();
  }
}

void EVe::CreateYprojection()
{
  if (fTextButtonProj->IsOn())
  {
     fRootEmbeddedCanvas->AdoptCanvas(c4);
     c4->cd();
     c4->Clear();

     y1 = new WirePlane((char*)"Y1",MWDC1_Y,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(y1_dist),cst->transLtoCL(L1),1.0,1.0,-1);
     y1p = new WirePlane((char*)"Y1p",MWDC1_Yp,-0.5*cst->transLtoCL(L1)+cst->transXtoCX(0.0), cst->transYtoCY(y1p_dist),cst->transLtoCL(L1),1.0,1.0,1);

     y2 = new WirePlane((char*)"Y2",MWDC1_Y,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(y2_dist),cst->transLtoCL(L2),1.0,1.0,-1);
     y2p = new WirePlane((char*)"Y2p",MWDC1_Yp,-0.5*cst->transLtoCL(L2)+cst->transXtoCX(0.0), cst->transYtoCY(y2p_dist),cst->transLtoCL(L2),1.0,1.0,+1);

     c4->Update();
  }
}


void EVe::CreateWires()
{
   fRootEmbeddedCanvas->AdoptCanvas(c3);
   c3->cd();
   c3->Clear();
   CStransform *mwdc1_cst = new CStransform(canvas_length, canvas_MWDC1_posx, canvas_MWDC1_posy);
   CStransform *mwdc2_cst = new CStransform(canvas_length, canvas_MWDC2_posx, canvas_MWDC2_posy);

   GetVariables *vars = new GetVariables("HMS.txt");
   int NScintPlanes = vars->GetInt("Number of Scint Planes =");
   

   CStransform *dE_cst = new CStransform(canvas_length, canvas_dE_posx, canvas_dE_posy);
   CStransform *E_cst = new CStransform(canvas_length, canvas_E_posx, canvas_E_posy);

   if (NScintPlanes == 4) {
     s2x_cst = new CStransform(canvas_length, 0.57, 0.5);
     s2y_cst = new CStransform(canvas_length, 0.84, 0.2);
   }

   // TODO: Wire number is different in different wire planes. For now we asume
   // in planar view, that the number of wires is in all three w.p. the same. 
   mwdc1 = new MWDChamber((char*)"MWDC-1", MWDC1_X, L1, MWDC_width, mwdc1_cst,0);
   mwdc2 = new MWDChamber((char*)"MWDC-2", MWDC1_X, L2, MWDC_width, mwdc2_cst,0);
  
   /// Variables to generate scintillator planes

   GetVariables *orientation1 = new GetVariables("HMS.txt");

   int orient1 = orientation1->GetInt("1st Scint Array Rotation =");
  
   GetVariables *orientation2 = new GetVariables("HMS.txt");

   int orient2 = orientation2->GetInt("2nd Scint Array Rotation =");
   
   GetVariables *pad1 = new GetVariables("HMS.txt");

   int nPaddles1 = pad1->GetInt("1st Scint Array NPaddles =");
   GetVariables *pad2 = new GetVariables("HMS.txt");
   
   int nPaddles2 = pad2->GetInt("2nd Scint Array NPaddles =");
   

   s1X = new ScintPlane((char*)"s1x-plane", nPaddles1, dE_length, dE_height, dE_cst, orient1);
   
   s1Y = new ScintPlane((char*)"s1y-plane", nPaddles2, E_length, E_height, E_cst, orient2);
   
   if (NScintPlanes == 4) {
     s2X = new ScintPlane((char*)"s2x-plane", nPaddles1, dE_length, dE_height, s2x_cst, orient1);
     
     s2Y = new ScintPlane((char*)"s2y-plane", nPaddles2, E_length, E_height, s2y_cst, orient2);
   }


   // In the end we plot a coordinate system

   TArrow *xaxis = new TArrow(0.82, 0.02, 0.95, 0.02, 0.02, "|>");
   xaxis->Draw();
   TArrow *yaxis = new TArrow(0.82, 0.02, 0.82, 0.15, 0.02, "|>");
   yaxis->Draw();

   TLatex *xtext = new TLatex(0.94,0.03, "x");
   xtext->SetTextSize(0.03);
   xtext->Draw();  

   TLatex *ytext = new TLatex(0.83, 0.122, "y");
   ytext->SetTextSize(0.03);
   ytext->Draw(); 

   c3->Update();
}

void EVe::initRun(char *filename)
{
  cout << "Reading file " << endl;
  TFile *f1 = new TFile(filename, "r");
  t1 = (TTree*) f1->Get("T");
  TotalNumberOfEvents =  t1->GetEntries();
  
  THaRun* runinfo = (THaRun*)gROOT->FindObject("Run_Data");
  if(runinfo==NULL) run_number = 0;
  else run_number = runinfo->GetNumber(); 
  cout<<"RUN #: "<<run_number<<endl;
  
#if DEBUG_LEVEL >= 3
   cout<<"Number of Entries is: "<<TotalNumberOfEvents<<endl;
#endif

  // Now we read all data
  // t1->SetBranchAddress( "BB.mwdc.u1.nhits", &B_mwdc_u1_nhits);
  // t1->SetBranchAddress( "BB.mwdc.u1p.nhits", &B_mwdc_u1p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.u2.nhits", &B_mwdc_v2p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.u2p.nhits", &B_mwdc_v2_nhits);


  // t1->SetBranchAddress( "BB.mwdc.v1.nhits", &B_mwdc_v1_nhits);
  // t1->SetBranchAddress( "BB.mwdc.v1p.nhits", &B_mwdc_v1p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.v2.nhits", &B_mwdc_u2p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.v2p.nhits", &B_mwdc_u2_nhits);

  // t1->SetBranchAddress( "BB.mwdc.x1.nhits", &B_mwdc_x1_nhits);
  // t1->SetBranchAddress( "BB.mwdc.x1p.nhits", &B_mwdc_x1p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.x2.nhits", &B_mwdc_x2p_nhits);
  // t1->SetBranchAddress( "BB.mwdc.x2p.nhits", &B_mwdc_x2_nhits);

   t1->SetBranchAddress( "Ndata.H.dc.1u1.tdchits", &Ndata_H_dc_1u1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.1v1.tdchits", &Ndata_H_dc_1v1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2u1.tdchits", &Ndata_H_dc_2u1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2v1.tdchits", &Ndata_H_dc_2v1_tdchits);

   t1->SetBranchAddress( "Ndata.H.dc.1x1.tdchits", &Ndata_H_dc_1x1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.1x2.tdchits", &Ndata_H_dc_1x2_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2x1.tdchits", &Ndata_H_dc_2x1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2x2.tdchits", &Ndata_H_dc_2x2_tdchits);

   t1->SetBranchAddress( "Ndata.H.dc.1y1.tdchits", &Ndata_H_dc_1y1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.1y2.tdchits", &Ndata_H_dc_1y2_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2y1.tdchits", &Ndata_H_dc_2y1_tdchits);
   t1->SetBranchAddress( "Ndata.H.dc.2y2.tdchits", &Ndata_H_dc_2y2_tdchits);

   /// UV Wire plane
   t1->SetBranchAddress( "H.dc.1u1.tdchits", &H_dc_1u1_tdchits);
   t1->SetBranchAddress( "H.dc.1v1.tdchits", &H_dc_1v1_tdchits);
   t1->SetBranchAddress( "H.dc.2u1.tdchits", &H_dc_2u1_tdchits);
   t1->SetBranchAddress( "H.dc.2v1.tdchits", &H_dc_2v1_tdchits);

   t1->SetBranchAddress( "H.dc.1u1.time", &H_dc_1u1_time);
   t1->SetBranchAddress( "H.dc.1v1.time", &H_dc_1v1_time);
   t1->SetBranchAddress( "H.dc.2u1.time", &H_dc_2u1_time);
   t1->SetBranchAddress( "H.dc.2v1.time", &H_dc_2v1_time);

   /// X Wire plane
   t1->SetBranchAddress( "H.dc.1x1.tdchits", &H_dc_1x1_tdchits);
   t1->SetBranchAddress( "H.dc.1x2.tdchits", &H_dc_1x2_tdchits);
   t1->SetBranchAddress( "H.dc.2x1.tdchits", &H_dc_2x1_tdchits);
   t1->SetBranchAddress( "H.dc.2x2.tdchits", &H_dc_2x2_tdchits);

   t1->SetBranchAddress( "H.dc.1x1.time", &H_dc_1x1_time);
   t1->SetBranchAddress( "H.dc.1x2.time", &H_dc_1x2_time);
   t1->SetBranchAddress( "H.dc.2x1.time", &H_dc_2x1_time);
   t1->SetBranchAddress( "H.dc.2x2.time", &H_dc_2x2_time);

   /// Y Wire plane
   t1->SetBranchAddress( "H.dc.1y1.tdchits", &H_dc_1y1_tdchits);
   t1->SetBranchAddress( "H.dc.1y2.tdchits", &H_dc_1y2_tdchits);
   t1->SetBranchAddress( "H.dc.2y1.tdchits", &H_dc_2y1_tdchits);
   t1->SetBranchAddress( "H.dc.2y2.tdchits", &H_dc_2y2_tdchits);

   t1->SetBranchAddress( "H.dc.1y1.time", &H_dc_1y1_time);
   t1->SetBranchAddress( "H.dc.1y2.time", &H_dc_1y2_time);
   t1->SetBranchAddress( "H.dc.2y1.time", &H_dc_2y1_time);
   t1->SetBranchAddress( "H.dc.2y2.time", &H_dc_2y2_time);


  // // U-wire plane
  // t1->SetBranchAddress( "BB.mwdc.u1.hit.wire", &B_mwdc_u1_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.u1p.hit.wire", &B_mwdc_u1p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.u2.hit.wire", &B_mwdc_v2p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.u2p.hit.wire", &B_mwdc_v2_hit_wire);

  // t1->SetBranchAddress( "BB.mwdc.u1.hit.time", &B_mwdc_u1_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.u1p.hit.time", &B_mwdc_u1p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.u2.hit.time", &B_mwdc_v2p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.u2p.hit.time", &B_mwdc_v2_hit_time);

  // t1->SetBranchAddress( "BB.mwdc.u.nroads", &B_mwdc_u_nroads); 
  // t1->SetBranchAddress( "BB.mwdc.u.rd.zL", &B_mwdc_u_rd_zL);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.zU", &B_mwdc_u_rd_zU);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.xLL", &B_mwdc_u_rd_xLL);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.xLR", &B_mwdc_u_rd_xLR);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.xUL", &B_mwdc_u_rd_xUL);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.xUR", &B_mwdc_u_rd_xUR);
  
  // t1->SetBranchAddress( "BB.mwdc.u.rd.good", &B_mwdc_u_rd_good);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.pos", &B_mwdc_u_rd_pos);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.slope", &B_mwdc_u_rd_slope);
  // t1->SetBranchAddress( "BB.mwdc.u.rd.chi2", &B_mwdc_u_rd_chi2);
 
  // // V-wire plane
  // t1->SetBranchAddress( "BB.mwdc.v1.hit.wire", &B_mwdc_v1_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.v1p.hit.wire", &B_mwdc_v1p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.v2.hit.wire", &B_mwdc_u2p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.v2p.hit.wire", &B_mwdc_u2_hit_wire);

  // t1->SetBranchAddress( "BB.mwdc.v1.hit.time", &B_mwdc_v1_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.v1p.hit.time", &B_mwdc_v1p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.v2.hit.time", &B_mwdc_u2p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.v2p.hit.time", &B_mwdc_u2_hit_time);

  // t1->SetBranchAddress( "BB.mwdc.v.nroads", &B_mwdc_v_nroads); 
  // t1->SetBranchAddress( "BB.mwdc.v.rd.zL", &B_mwdc_v_rd_zL);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.zU", &B_mwdc_v_rd_zU);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.xLL", &B_mwdc_v_rd_xLL);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.xLR", &B_mwdc_v_rd_xLR);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.xUL", &B_mwdc_v_rd_xUL);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.xUR", &B_mwdc_v_rd_xUR);
  
  // t1->SetBranchAddress( "BB.mwdc.v.rd.good", &B_mwdc_v_rd_good);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.pos", &B_mwdc_v_rd_pos);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.slope", &B_mwdc_v_rd_slope);
  // t1->SetBranchAddress( "BB.mwdc.v.rd.chi2", &B_mwdc_v_rd_chi2);

  // // X-wire plane
  // t1->SetBranchAddress( "BB.mwdc.x1.hit.wire", &B_mwdc_x1_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.x1p.hit.wire", &B_mwdc_x1p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.x2.hit.wire", &B_mwdc_x2p_hit_wire);
  // t1->SetBranchAddress( "BB.mwdc.x2p.hit.wire", &B_mwdc_x2_hit_wire);

  // t1->SetBranchAddress( "BB.mwdc.x1.hit.time", &B_mwdc_x1_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.x1p.hit.time", &B_mwdc_x1p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.x2.hit.time", &B_mwdc_x2p_hit_time);
  // t1->SetBranchAddress( "BB.mwdc.x2p.hit.time", &B_mwdc_x2_hit_time);

  // t1->SetBranchAddress( "BB.mwdc.x.nroads", &B_mwdc_x_nroads); 
  // t1->SetBranchAddress( "BB.mwdc.x.rd.zL", &B_mwdc_x_rd_zL);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.zU", &B_mwdc_x_rd_zU);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.xLL", &B_mwdc_x_rd_xLL);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.xLR", &B_mwdc_x_rd_xLR);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.xUL", &B_mwdc_x_rd_xUL);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.xUR", &B_mwdc_x_rd_xUR);
  
  // t1->SetBranchAddress( "BB.mwdc.x.rd.good", &B_mwdc_x_rd_good);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.pos", &B_mwdc_x_rd_pos);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.slope", &B_mwdc_x_rd_slope);
  // t1->SetBranchAddress( "BB.mwdc.x.rd.chi2", &B_mwdc_x_rd_chi2);

  // t1->SetBranchAddress( "BB.mwdc.u.ngood", &B_mwdc_u_ngood);
  // t1->SetBranchAddress( "BB.mwdc.v.ngood", &B_mwdc_v_ngood);
  // t1->SetBranchAddress( "BB.mwdc.x.ngood", &B_mwdc_x_ngood);

   /// TRACKS
   t1->SetBranchAddress( "Ndata.H.tr.x", &Ndata_H_tr_x);
   t1->SetBranchAddress( "H.tr.x", &H_tr_x);
   t1->SetBranchAddress( "H.tr.y", &H_tr_y);
   t1->SetBranchAddress( "H.tr.ph", &H_tr_ph);
   t1->SetBranchAddress( "H.tr.th", &H_tr_th); 
  


  // t1->SetBranchAddress( "BB.tp.e.nhit", &B_tp_e_nhit);
  // t1->SetBranchAddress( "BB.tp.e.LT", &B_tp_e_LT);
  // t1->SetBranchAddress( "BB.tp.e.RT", &B_tp_e_RT);
  // t1->SetBranchAddress( "BB.tp.e.hit_ypos", &B_tp_e_hit_ypos);
  // t1->SetBranchAddress( "BB.tp.e.hit_bar", &B_tp_e_hit_bar);
  
  // t1->SetBranchAddress( "BB.tp.de.nhit", &B_tp_de_nhit);
  // t1->SetBranchAddress( "BB.tp.de.LT", &B_tp_de_LT);
  // t1->SetBranchAddress( "BB.tp.de.RT", &B_tp_de_RT);
  // t1->SetBranchAddress( "BB.tp.de.hit_ypos", &B_tp_de_hit_ypos);
  // t1->SetBranchAddress( "BB.tp.de.hit_bar", &B_tp_de_hit_bar);

   /// S1X
   t1->SetBranchAddress( "Ndata.H.hod.1x.negtdchits", &Ndata_H_hod_1x_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.1x.postdchits", &Ndata_H_hod_1x_postdchits);
   t1->SetBranchAddress( "H.hod.1x.negtdchits", &H_hod_1x_negtdchits);
   t1->SetBranchAddress( "H.hod.1x.postdchits", &H_hod_1x_postdchits);
  // t1->SetBranchAddress( "BB.tp.e.hit_ypos", &B_tp_e_hit_ypos);
  //t1->SetBranchAddress( "BB.tp.e.hit_bar", &B_tp_e_hit_bar);
  
   ///S1Y
   t1->SetBranchAddress( "Ndata.H.hod.1y.negtdchits", &Ndata_H_hod_1y_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.1y.postdchits", &Ndata_H_hod_1y_postdchits);
   t1->SetBranchAddress( "H.hod.1y.negtdchits", &H_hod_1y_negtdchits);
   t1->SetBranchAddress( "H.hod.1y.postdchits", &H_hod_1y_postdchits);
  //t1->SetBranchAddress( "BB.tp.de.hit_ypos", &B_tp_de_hit_ypos);
  //t1->SetBranchAddress( "BB.tp.de.hit_bar", &B_tp_de_hit_bar);

   ///S2X
   t1->SetBranchAddress( "Ndata.H.hod.2x.negtdchits", &Ndata_H_hod_2x_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.2x.postdchits", &Ndata_H_hod_2x_postdchits);
   t1->SetBranchAddress( "H.hod.2x.negtdchits", &H_hod_2x_negtdchits);
   t1->SetBranchAddress( "H.hod.2x.postdchits", &H_hod_2x_postdchits);

   ///S2Y
   t1->SetBranchAddress( "Ndata.H.hod.2y.negtdchits", &Ndata_H_hod_2y_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.2y.postdchits", &Ndata_H_hod_2y_postdchits);
   t1->SetBranchAddress( "H.hod.2y.negtdchits", &H_hod_2y_negtdchits);
   t1->SetBranchAddress( "H.hod.2y.postdchits", &H_hod_2y_postdchits);

  /* TRACKS
#if FULL_TRACK > 0
  t1->SetBranchAddress( "BB.tr.px", &B_tr_px);
  t1->SetBranchAddress( "BB.tr.py", &B_tr_py);
  t1->SetBranchAddress( "BB.tr.pz", &B_tr_pz);
  t1->SetBranchAddress( "BB.tr.p", &B_tr_p);
#endif
  */

} 

void EVe::DoDraw(int event)
{
  t1->GetEntry(event);


  TString Buff;
  if(event==0) {
    Buff = "";
   } else {
   Buff = "Run #";
   Buff += run_number;
   Buff += ", Event #";
   Buff += event;
   }
   graph_title = Buff.Data();
   //cout<<"Char #: "<<graph_title<<endl;
  

//_______________________ Lets handle X-projection view _______________________________________

  if (fTextButtonProj->IsOn() && fTextButtonXPlane->IsOn())
  {

// #if DEBUG_LEVEL >= 3
//     cout<<"Plane U1 has been hit ... times: "<<B_mwdc_u1_nhits<<endl;
//     cout<<"Plane U1p has been hit ... times: "<<B_mwdc_u1p_nhits<<endl;
//     cout<<"Plane U2 has been hit ... times: "<<B_mwdc_u2_nhits<<endl;
//     cout<<"Plane U2p has been hit ... times: "<<B_mwdc_u2p_nhits<<endl;
//     cout<<endl;
//     cout<<endl;
// #endif

    c1->cd();
    x1->clear();  
    x1p->clear(); 
    x2->clear();  
    x2p->clear();

    c1->Update();


    if (title != NULL) delete title;
    title = new TLatex(0.02,0.96, graph_title);
    title->SetTextSize(0.03);
    title->Draw();   



   for (int i = 0; i<Ndata_H_dc_1x1_tdchits; i++)
      {

        x1->SetWire(H_dc_1x1_tdchits[i],H_dc_1x1_time[i]); 
      }
    
    for (int i = 0; i<Ndata_H_dc_1x2_tdchits; i++)
      {

        x1p->SetWire(H_dc_1x2_tdchits[i],H_dc_1x2_time[i]); 
      }

    for (int i = 0; i<Ndata_H_dc_2x1_tdchits; i++)
      {

        x2->SetWire(H_dc_2x1_tdchits[i],H_dc_2x1_time[i]); 
      }
    
    for (int i = 0; i<Ndata_H_dc_2x2_tdchits; i++)
      {

        x2p->SetWire(H_dc_2x2_tdchits[i],H_dc_2x2_time[i]); 
      }


//     umrd->Clear();
//     for (int q=0; q< MAX_ROADS_NUM; q++) utr[q]->Clear();

// #if DEBUG_LEVEL >= 3
//     cout<<"Number of roads in U projection is: "<<B_mwdc_u_nroads<<endl;
//     cout<<"Number of good roads in U projection is: "<<B_mwdc_u_ngood<<endl;
// #endif
    
//     for (int j = 0; j<B_mwdc_u_nroads; j++)
//     { 
// #if DEBUG_LEVEL >= 3
//         cout<<"******** Data for road No.: "<<j<<" of: "<<B_mwdc_u_nroads<<" ************"<<endl;
//         cout<<"-----> zL: "<<B_mwdc_u_rd_zL[j]<<endl;
// 	cout<<"-----> zU: "<<B_mwdc_u_rd_zU[j]<<endl;
// 	cout<<"-----> xLL: "<<B_mwdc_u_rd_xLL[j]<<endl;
// 	cout<<"-----> xLR: "<<B_mwdc_u_rd_xLR[j]<<endl;
// 	cout<<"-----> xUL: "<<B_mwdc_u_rd_xUL[j]<<endl;
// 	cout<<"-----> xUR: "<<B_mwdc_u_rd_xUR[j]<<endl;
//         cout<<endl;
// 	cout<<"-----> good: "<<B_mwdc_u_rd_good[j]<<endl;
// 	cout<<"-----> chi2: "<<B_mwdc_u_rd_chi2[j]<<endl;
// 	cout<<"-----> pos: "<<B_mwdc_u_rd_pos[j]<<endl;
// 	cout<<"-----> slope: "<<B_mwdc_u_rd_slope[j]<<endl;
//         cout<<"************************************************"<<endl;
// #endif
	

// cout<<"Sem tlele 1"<<endl;
// 	 Double_t y[4] = {B_mwdc_u_rd_zL[j],
// 			  B_mwdc_u_rd_zU[j],
// 			  B_mwdc_u_rd_zU[j], 
// 			  B_mwdc_u_rd_zL[j]};

//         Double_t x[4] = {B_mwdc_u_rd_xLL[j],
// 			 B_mwdc_u_rd_xUL[j], 
// 			 B_mwdc_u_rd_xUR[j], 
// 			 B_mwdc_u_rd_xLR[j]};

// cout<<"Sem tlele 2"<<endl;	

// 	if (fTextButtonRoads->IsOn()) umrd->Show(j,x,y);

// cout<<"Sem tlele 3"<<endl;
//         if (B_mwdc_u_rd_good[j]>0 && (fTextButtonTrackProj->IsOn()) )
// 	{
// cout<<"Sem tlele 3a"<<endl;
// 		// If road is good, then we draw the calculated track.
// 		double tz1 = -0.4; // in meters
// 		double tz2 = 1.2; // in meters
// cout<<"Sem tlele 3b"<<endl;
// 		double tx1 = B_mwdc_u_rd_slope[j]*tz1 + B_mwdc_u_rd_pos[j];
// cout<<"Sem tlele 3c"<<endl;
// 		double tx2 = B_mwdc_u_rd_slope[j]*tz2 + B_mwdc_u_rd_pos[j];
		
// cout<<"Sem tlele 3d"<<endl;
// cout<<j<<endl;
// 		utr[j]->DrawMe(tx1,tz1,tx2,tz2);
// cout<<"Sem tlele 3e"<<endl;
// 	}
//     }
// cout<<"Sem tlele 4"<<endl;
//     c1->Draw();
//     c1->Update(); 

// cout<<"Sem tlele 5"<<endl;

    c1->Draw();
    c1->Update();
  }

// //_________________________ Lets handle UV-projection view ______________________________

  if (fTextButtonProj->IsOn() && fTextButtonUVPlane->IsOn())
    {

// #if DEBUG_LEVEL >= 3
//     cout<<"Plane V1 has been hit ... times: "<<B_mwdc_v1_nhits<<endl;
//     cout<<"Plane V1p has been hit ... times: "<<B_mwdc_v1p_nhits<<endl;
//     cout<<"Plane V2 has been hit ... times: "<<B_mwdc_v2_nhits<<endl;
//     cout<<"Plane V2p has been hit ... times: "<<B_mwdc_v2p_nhits<<endl;
//     cout<<endl;
//     cout<<endl;
// #endif

      c2->cd();
      u1->clear();  
      v1->clear();
      u2->clear();  
      v2->clear();
      
      c2->Update();
      
      
      if (title != NULL) delete title;
      title = new TLatex(0.02,0.96, graph_title);
      title->SetTextSize(0.03);
      title->Draw();   

  for (int i = 0; i<Ndata_H_dc_1u1_tdchits; i++)
      {

        u1->SetWire(H_dc_1u1_tdchits[i],H_dc_1u1_time[i]); 
      }
    
    for (int i = 0; i<Ndata_H_dc_1v1_tdchits; i++)
      {

        v1->SetWire(H_dc_1v1_tdchits[i],H_dc_1v1_time[i]); 
      }

    for (int i = 0; i<Ndata_H_dc_2u1_tdchits; i++)
      {

        u2->SetWire(H_dc_2u1_tdchits[i],H_dc_2u1_time[i]); 
      }
    
    for (int i = 0; i<Ndata_H_dc_2v1_tdchits; i++)
      {

        v2->SetWire(H_dc_2v1_tdchits[i],H_dc_2v1_time[i]); 
      }



//     vmrd->Clear();
//     for (int q=0; q< MAX_ROADS_NUM; q++) vtr[q]->Clear();

// #if DEBUG_LEVEL >= 3
//     cout<<"Number of roads in V projection is: "<<B_mwdc_v_nroads<<endl;
//     cout<<"Number of good roads in V projection is: "<<B_mwdc_v_ngood<<endl;
// #endif
   
//     for (int j = 0; j<B_mwdc_v_nroads; j++)
//     { 
// #if DEBUG_LEVEL >= 3
//         cout<<"******** Data for V road No.: "<<j<<" ************"<<endl;
//         cout<<"-----> zL: "<<B_mwdc_v_rd_zL[j]<<endl;
// 	cout<<"-----> zU: "<<B_mwdc_v_rd_zU[j]<<endl;
// 	cout<<"-----> xLL: "<<B_mwdc_v_rd_xLL[j]<<endl;
// 	cout<<"-----> xLR: "<<B_mwdc_v_rd_xLR[j]<<endl;
// 	cout<<"-----> xUL: "<<B_mwdc_v_rd_xUL[j]<<endl;
// 	cout<<"-----> xUR: "<<B_mwdc_v_rd_xUR[j]<<endl;
//         cout<<endl;
// 	cout<<"-----> good: "<<B_mwdc_v_rd_good[j]<<endl;
// 	cout<<"-----> chi2: "<<B_mwdc_v_rd_chi2[j]<<endl;
// 	cout<<"-----> pos: "<<B_mwdc_v_rd_pos[j]<<endl;
// 	cout<<"-----> slope: "<<B_mwdc_v_rd_slope[j]<<endl;
//         cout<<"************************************************"<<endl;
// #endif
// 	 Double_t y[4] = {B_mwdc_v_rd_zL[j],
// 			  B_mwdc_v_rd_zU[j],
// 			  B_mwdc_v_rd_zU[j], 
// 			  B_mwdc_v_rd_zL[j]};

//         Double_t x[4] = {B_mwdc_v_rd_xLL[j],
// 			 B_mwdc_v_rd_xUL[j], 
// 			 B_mwdc_v_rd_xUR[j], 
// 			 B_mwdc_v_rd_xLR[j]};
	
// 	if (fTextButtonRoads->IsOn()) vmrd->Show(j,x,y);

//         if (B_mwdc_v_rd_good[j]>0 && fTextButtonTrackProj->IsOn() )
// 	{
// 		// If road is good, then we draw the calculated track.
// 		double tz1 = -0.4; // in meters
// 		double tz2 = 1.2; // in meters
// 		double tx1 = B_mwdc_v_rd_slope[j]*tz1 + B_mwdc_v_rd_pos[j];
// 		double tx2 = B_mwdc_v_rd_slope[j]*tz2 + B_mwdc_v_rd_pos[j];
		
// 		vtr[j]->DrawMe(tx1,tz1,tx2,tz2);
// 	}
//     }
      c2->Draw();
      c2->Update(); 
    }

// //_________________________ Lets handle Y-projection view ______________________________

  if (fTextButtonProj->IsOn() && fTextButtonYPlane->IsOn())
    {
    
// #if DEBUG_LEVEL >= 3
//     cout<<"Plane X1 has been hit ... times: "<<B_mwdc_x1_nhits<<endl;
//     cout<<"Plane X1p has been hit ... times: "<<B_mwdc_x1p_nhits<<endl;
//     cout<<"Plane X2 has been hit ... times: "<<B_mwdc_x2_nhits<<endl;
//     cout<<"Plane X2p has been hit ... times: "<<B_mwdc_x2p_nhits<<endl;
//     cout<<endl;
//     cout<<endl;
// #endif

      c4->cd();
      y1->clear();  
      y1p->clear();
      y2->clear();  
      y2p->clear();

      c4->Update();


      if (title != NULL) delete title;
      title = new TLatex(0.02,0.96, graph_title);
      title->SetTextSize(0.03);
      title->Draw();   


      for (int i = 0; i<Ndata_H_dc_1y1_tdchits; i++)
	{

	  y1->SetWire(H_dc_1y1_tdchits[i],H_dc_1y1_time[i]); 
	}
    
      for (int i = 0; i<Ndata_H_dc_1y2_tdchits; i++)
	{

	  y1p->SetWire(H_dc_1y2_tdchits[i],H_dc_1y2_time[i]); 
	}

      for (int i = 0; i<Ndata_H_dc_2y1_tdchits; i++)
	{
	  
	  y2->SetWire(H_dc_2y1_tdchits[i],H_dc_2y1_time[i]); 
	}
      
      for (int i = 0; i<Ndata_H_dc_2y2_tdchits; i++)
	{
	  
	  y2p->SetWire(H_dc_2y2_tdchits[i],H_dc_2y2_time[i]); 
	}

//     xmrd->Clear();
//     for (int q=0; q< MAX_ROADS_NUM; q++) xtr[q]->Clear();

// #if DEBUG_LEVEL >= 3
//     cout<<"Number of roads in X projection is: "<<B_mwdc_x_nroads<<endl;
//     cout<<"Number of good roads in X projection is: "<<B_mwdc_x_ngood<<endl;
// #endif

//     for (int j = 0; j<B_mwdc_x_nroads; j++)
//     { 
// #if DEBUG_LEVEL >= 3
//         cout<<"******** Data for X road No.: "<<" ************"<<endl;
//         cout<<"-----> zL: "<<B_mwdc_x_rd_zL[j]<<endl;
// 	cout<<"-----> zU: "<<B_mwdc_x_rd_zU[j]<<endl;
// 	cout<<"-----> xLL: "<<B_mwdc_x_rd_xLL[j]<<endl;
// 	cout<<"-----> xLR: "<<B_mwdc_x_rd_xLR[j]<<endl;
// 	cout<<"-----> xUL: "<<B_mwdc_x_rd_xUL[j]<<endl;
// 	cout<<"-----> xUR: "<<B_mwdc_x_rd_xUR[j]<<endl;
//         cout<<endl;
// 	cout<<"-----> good: "<<B_mwdc_x_rd_good[j]<<endl;
// 	cout<<"-----> chi2: "<<B_mwdc_x_rd_chi2[j]<<endl;
// 	cout<<"-----> pos: "<<B_mwdc_x_rd_pos[j]<<endl;
// 	cout<<"-----> slope: "<<B_mwdc_x_rd_slope[j]<<endl;
//         cout<<"************************************************"<<endl;
// #endif	
// 	 Double_t y[4] = {B_mwdc_x_rd_zL[j],
// 			  B_mwdc_x_rd_zU[j],
// 			  B_mwdc_x_rd_zU[j], 
// 			  B_mwdc_x_rd_zL[j]};

//         Double_t x[4] = {B_mwdc_x_rd_xLL[j],
// 			 B_mwdc_x_rd_xUL[j], 
// 			 B_mwdc_x_rd_xUR[j], 
// 			 B_mwdc_x_rd_xLR[j]};
	
// 	if (fTextButtonRoads->IsOn()) xmrd->Show(j,x,y);

//         if (B_mwdc_x_rd_good[j]>0 && fTextButtonTrackProj->IsOn())
// 	{
// 		// If road is good, then we draw the calculated track.
// 		double tz1 = -0.4; // in meters
// 		double tz2 = 1.2; // in meters
// 		double tx1 = B_mwdc_x_rd_slope[j]*tz1 + B_mwdc_x_rd_pos[j];
// 		double tx2 = B_mwdc_x_rd_slope[j]*tz2 + B_mwdc_x_rd_pos[j];
		
// 		xtr[j]->DrawMe(tx1,tz1,tx2,tz2);
// 	}
//     }
  
      c4->Draw();
      c4->Update(); 
//   }

    }

//_________________________ Lets handle Planar view  ______________________________

//// UNCOMMENT ALL OF THIS

  if (fTextButtonWires->IsOn())
  {
    c3->cd();

    
    if (title != NULL) delete title;
    title = new TLatex(0.02,0.02, graph_title);
    title->SetTextSize(0.03);
    title->Draw();   

    //***************** First Chamber


// #if DEBUG_LEVEL >= 3
//     cout<<"Plane U1 has been hit ... times: "<<B_mwdc_u1_nhits<<endl;
//     cout<<"Plane U1p has been hit ... times: "<<B_mwdc_u1p_nhits<<endl;
//     cout<<"Plane V1 has been hit ... times: "<<B_mwdc_v1_nhits<<endl;
//     cout<<"Plane V1p has been hit ... times: "<<B_mwdc_v1p_nhits<<endl;
//     cout<<"Plane X1 has been hit ... times: "<<B_mwdc_x1_nhits<<endl;
//     cout<<"Plane X1p has been hit ... times: "<<B_mwdc_x1p_nhits<<endl;
// #endif

    mwdc1->clear();

    /// X plane

    for(int i = 0; i<Ndata_H_dc_1x1_tdchits; i++)
      {
	
        mwdc1->x1WireHit(H_dc_1x1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1x2_tdchits; i++)
      {
	
        mwdc1->xpWireHit(H_dc_1x2_tdchits[i]);
      } 

    /// UV plane

    for(int i = 0; i<Ndata_H_dc_1u1_tdchits; i++)
      {
	
        mwdc1->uWireHit(H_dc_1u1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1v1_tdchits; i++)
      {
	
        mwdc1->vWireHit(H_dc_1v1_tdchits[i]);
      } 

    /// Y plane

    for(int i = 0; i<Ndata_H_dc_1y1_tdchits; i++)
      {
	
        mwdc1->y1WireHit(H_dc_1y1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1y2_tdchits; i++)
      {
	
        mwdc1->ypWireHit(H_dc_1y2_tdchits[i]);
      } 


//     //***************** Second Chamber

// #if DEBUG_LEVEL >= 3
//     cout<<"Plane U2 has been hit ... times: "<<B_mwdc_u2_nhits<<endl;
//     cout<<"Plane U2p has been hit ... times: "<<B_mwdc_u2p_nhits<<endl;
//     cout<<"Plane V2 has been hit ... times: "<<B_mwdc_v2_nhits<<endl;
//     cout<<"Plane V2p has been hit ... times: "<<B_mwdc_v2p_nhits<<endl;
//     cout<<"Plane X2 has been hit ... times: "<<B_mwdc_x2_nhits<<endl;
//     cout<<"Plane X2p has been hit ... times: "<<B_mwdc_x2p_nhits<<endl;
// #endif

    mwdc2->clear();

    /// X plane

    for(int i = 0; i<Ndata_H_dc_2x1_tdchits; i++)
      {
	
        mwdc2->x1WireHit(H_dc_2x1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2x2_tdchits; i++)
      {
	
        mwdc2->xpWireHit(H_dc_2x2_tdchits[i]);
      } 

    /// UV plane

    for(int i = 0; i<Ndata_H_dc_2u1_tdchits; i++)
      {
	
        mwdc2->uWireHit(H_dc_2u1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2v1_tdchits; i++)
      {
	
        mwdc2->vWireHit(H_dc_2v1_tdchits[i]);
      } 

    /// Y plane

    for(int i = 0; i<Ndata_H_dc_2y1_tdchits; i++)
      {
	
        mwdc2->y1WireHit(H_dc_2y1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2y2_tdchits; i++)
      {
	
        mwdc2->ypWireHit(H_dc_2y2_tdchits[i]);
      } 
 

    /////  ******** Now scintillation planes

// #if DEBUG_LEVEL >= 3
//    cout<<"*!* E - nhit: "<<B_tp_e_nhit<<", dE - nhit: "<<B_tp_de_nhit<<endl;
// #endif


   ///// ADD IN Multi-plane particle detection

   double Ebar_ypos[E_PN];
   double dEbar_ypos[dE_PN];
   double matchR[16];
   double matchL[16];
   // double Ebar_yposfake[E_PN];
   //double dEbar_yposfake[dE_PN];
   //int Ebar[E_PN];
   //int dEbar[dE_PN];
   s1Y->clear();
   s1X->clear();
   s2X->clear();
   s2Y->clear();
   for (int q = 0; q<E_PN; q++) Ebar_ypos[q] = 0.0;
   for (int q = 0; q<dE_PN; q++) dEbar_ypos[q] = 0.0;
	
   ///S1X
   //   for (Int_t q = 0; q<B_tp_e_nhit; q++)
   for (Int_t q = 0; q<Ndata_H_hod_1x_negtdchits; q++)
     {
     //	int bar = (int)(B_tp_e_hit_bar[q]);
     //  double ypos = B_tp_e_hit_ypos[q];
       //	Ebar_ypos[bar] = ypos;
       //  cout << Ndata_H_hod_1x_negtdchits <<"  " <<  H_hod_1x_negtdchits[q]  << endl;
       matchR[q] = H_hod_1x_negtdchits[q];
       s1X->paddleRightHit(H_hod_1x_negtdchits[q]);
       // dEbar_ypos[q] = 0.01;
       // #if DEBUG_LEVEL >= 3
       // 	cout<<"--> Bar E: "<<bar<<" Y pos: "<<ypos<<endl;
       // #endif
     }

   for (Int_t q=0;q<Ndata_H_hod_1x_postdchits;q++)
     {
     
       matchL[q]= H_hod_1x_postdchits[q];
       s1X->paddleLeftHit(H_hod_1x_postdchits[q]);
     }

   for (Int_t i=0;i<16;i++) {
     for (Int_t j=0;j<16;j++) {
       if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {
	 
	 s1X->paddleBothHit(matchR[i]);
       } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
	 s1X->paddleBothHit(matchL[i]);
       }
     }
     matchR[i]=0;
     matchL[i]=0;
   }

   ///S1Y

   //   for (int q = 0; q<B_tp_de_nhit; q++)
   for (int q = 0; q<Ndata_H_hod_1y_negtdchits; q++)
     {
       //	int bar = (int)(B_tp_de_hit_bar[q]);
       // double ypos = B_tp_de_hit_ypos[q];
       //	dEbar_ypos[bar] = ypos;
       cout << Ndata_H_hod_1y_negtdchits <<"  " <<  H_hod_1y_negtdchits[q]  << endl;
       matchR[q] = H_hod_1y_negtdchits[q];
       s1Y->paddleRightHit(H_hod_1y_negtdchits[q]);

       //  Ebar_ypos[q]=0.01;
       // #if DEBUG_LEVEL >= 3
       // 	cout<<"--> Bar dE: "<<bar<<" Y pos: "<<ypos<<endl;
       // #endif
     }
   for (int q = 0; q<Ndata_H_hod_1y_postdchits; q++)
     {
  
       matchL[q]= H_hod_1y_postdchits[q];
       s1Y->paddleLeftHit(H_hod_1y_postdchits[q]);
     }

   for (Int_t i=0;i<16;i++) {
     for (Int_t j=0;j<16;j++) {
       
       if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {
	 s1Y->paddleBothHit(matchR[i]);
       } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
	 s1Y->paddleBothHit(matchL[i]);
       }
     }
     matchR[i]=0;
     matchL[i]=0;
   }

   /// S2X PLANE
   for (Int_t q = 0; q<Ndata_H_hod_2x_negtdchits; q++)
     {
       
       matchR[q] = H_hod_2x_negtdchits[q];
       s2X->paddleRightHit(H_hod_2x_negtdchits[q]);
       
     }
   
   for (Int_t q=0;q<Ndata_H_hod_2x_postdchits;q++)
     {
     
       matchL[q]= H_hod_2x_postdchits[q];
       s2X->paddleLeftHit(H_hod_2x_postdchits[q]);
     }

   for (Int_t i=0;i<16;i++) {
     for (Int_t j=0;j<16;j++) {
       if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {
	 
	 s2X->paddleBothHit(matchR[i]);
       } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
	 s2X->paddleBothHit(matchL[i]);
       }
     }
     matchR[i]=0;
     matchL[i]=0;
   }

   /// S2Y PLANE
   for (Int_t q = 0; q<Ndata_H_hod_2y_negtdchits; q++)
     {
       
       matchR[q] = H_hod_2y_negtdchits[q];
       s2Y->paddleRightHit(H_hod_2y_negtdchits[q]);
       
     }
   
   for (Int_t q=0;q<Ndata_H_hod_2y_postdchits;q++)
     {
     
       matchL[q]= H_hod_2y_postdchits[q];
       s2Y->paddleLeftHit(H_hod_2y_postdchits[q]);
     }

   for (Int_t i=0;i<16;i++) {
     for (Int_t j=0;j<16;j++) {
       if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {
	 
	 s2Y->paddleBothHit(matchR[i]);
       } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
	 s2Y->paddleBothHit(matchL[i]);
       }
     }
     matchR[i]=0;
     matchL[i]=0;
   }

   // sE->clear();
   // sdE->clear();
  //  for (int i = 0; i<dE_PN; i++)
//      {
// // #if DEBUG_LEVEL >= 3
// // 	cout<<"| dE -L: "<<B_tp_de_LT[i]<<" dE - R: "<<B_tp_de_RT[i]<<".....YPOS: "<<dEbar_ypos[i]<<" |"<<endl;
// // #endif

// //	sdE->paddleHit(i,B_tp_de_LT[i] ,B_tp_de_RT[i] , -dEbar_ypos[i]);

//        //   sE->paddleHit(i,H_hod_1x_negtdchits[i] ,H_hod_1x_postdchits[i] , -dEbar_ypos[i]);
//      }  

//    for (int i = 0; i<E_PN; i++)
//      {
// // #if DEBUG_LEVEL >= 3
// // 	cout<<"| E -L: "<<B_tp_e_LT[i]<<" E - R: "<<B_tp_e_RT[i]<<".....YPOS:"<<Ebar_ypos[i]<<"|"<<endl;
// // #endif

// //	sE->paddleHit(i,B_tp_e_LT[i] ,B_tp_e_RT[i] , -Ebar_ypos[i]);
//        sdE->paddleHit(i,H_hod_1y_negtdchits[i] ,H_hod_1y_postdchits[i] , -Ebar_ypos[i]);
//      }  





    //****** Now we draw Trajectories through detectors
    	 
   if (Ndata_H_tr_x > 0 && fTextButtonTrack->IsOn())
     {		
       for(int q =0; q<Ndata_H_tr_x; q++)
	 {
	   // double x0 = H_tr_x[q]/100; /// [cm] to [m]
	   // double y0 = H_tr_y[q]/100;
	   // double th = H_tr_th[q];
	   // double ph = H_tr_ph[q];
	   
	   double x0 = 0.0;
	   double y0 = 0.0;
	   double th = 0.0;
	   double ph = 0.0;

	   double z1 = MWDC2_z; ///!!! This value depends of the exp. setting
		//double x1 = x0 + tan(th)*z1;
		//double y1 = y0 + tan(ph)*z1;
	   double x1 = x0 + th*z1;
	   double y1 = y0 + ph*z1;    
  
//#if DEBUG_LEVEL >= 3
	   //	cout<<"Q: "<<q<<endl;
	   //	cout<<"~~~~~>th0: "<<th<<", ph0: "<<ph<<endl;
	   //	cout<<"~~~~~>x0: "<<x0<<", y0: "<<y0<<endl;
	   //	cout<<"~~~~~>x1: "<<x1<<", y1: "<<y1<<endl;
//#endif

	   mwdc1->Track(x0,y0,q);
	   mwdc2->Track(x1,y1,q);


	   double z3 = dE_z;
		//double x3 = x0 + tan(th)*z3;
		//double y3 = y0 + tan(ph)*z3;
	   double x3 = x0 + th*z3;
	   double y3 = y0 + ph*z3;

// #if DEBUG_LEVEL >= 3	
// 		cout<<"~~~~~>x3: "<<x3<<", y3: "<<y3<<endl;
// #endif
	   s1X->Track(x3,y3,q);

	   double z4 = E_z;
		//double x4 = x0 + tan(th)*z4;
		//double y4 = y0 + tan(ph)*z4;
	   double x4 = x0 + th*z4;
	   double y4 = y0 + ph*z4;


// #if DEBUG_LEVEL >= 3	
// 		cout<<"~~~~~>x4: "<<x4<<", y4: "<<y4<<endl;
// #endif
	     s1Y->Track(x4,y4,q);

	     double z5 = s2x_z;
	     double x5 = x0 + th*z5;
	     double y5 = y0 + ph*z5;

	     s2X->Track(x5,y5,q);

	     double z6 = s2y_z;
	     double x6 = x0 + th*z6;
	     double y6 = y0 + ph*z6;

	     s2Y->Track(x6,y6,q);
	 }
       
     }  


   c3->Draw();
   c3->Update();

  }




//_________________________ Lets handle 3D view  ______________________________________

  if (fTextButton3dView->IsOn())
  {
    cout<<"Wait! I am working!"<<endl;

    c5->cd();

    if (title != NULL) delete title;
    title = new TLatex(-0.96,-0.96, graph_title);
    title->SetTextSize(0.03);
    title->Draw();   

    //***************** First chamber

//     detector->mwdc1->clear();
//     for(int i = 0; i<B_mwdc_u1_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire u1 : "<<i<<" je: "<<B_mwdc_u1_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->u1WireHit(B_mwdc_u1_hit_wire[i]);
//     } 

//     for(int i = 0; i<B_mwdc_u1p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire u1p : "<<i<<" je: "<<B_mwdc_u1p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->u2WireHit(B_mwdc_u1p_hit_wire[i]);
//     } 

//     for(int i = 0; i<B_mwdc_v1_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire v1 : "<<i<<" je: "<<B_mwdc_v1_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->v1WireHit(B_mwdc_v1_hit_wire[i]);
//     } 

//     for(int i = 0; i<B_mwdc_v1p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire v1p : "<<i<<" je: "<<B_mwdc_v1p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->v2WireHit(B_mwdc_v1p_hit_wire[i]);
//     } 


//     for(int i = 0; i<B_mwdc_x1_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire x1 : "<<i<<" je: "<<B_mwdc_x1_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->x1WireHit(B_mwdc_x1_hit_wire[i]);
//     } 


//     for(int i = 0; i<B_mwdc_x1p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire x1p : "<<i<<" je: "<<B_mwdc_x1p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc1->x2WireHit(B_mwdc_x1p_hit_wire[i]);
//     } 

//     //***************** Second chamber

   
//     detector->mwdc2->clear();
//     for(int i = 0; i<B_mwdc_u2_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire u2 : "<<i<<" je: "<<B_mwdc_u2_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->u1WireHit(B_mwdc_u2_hit_wire[i]);
//     } 

//     for(int i = 0; i<B_mwdc_u2p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire u2p : "<<i<<" je: "<<B_mwdc_u2p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->u2WireHit(B_mwdc_u2p_hit_wire[i]);
//     } 


//     for(int i = 0; i<B_mwdc_v2_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire v2 : "<<i<<" je: "<<B_mwdc_v2_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->v1WireHit(B_mwdc_v2_hit_wire[i]);
//     } 

//     for(int i = 0; i<B_mwdc_v2p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire v2p : "<<i<<" je: "<<B_mwdc_v2p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->v2WireHit(B_mwdc_v2p_hit_wire[i]);
//     } 


//     for(int i = 0; i<B_mwdc_x2_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire x2 : "<<i<<" je: "<<B_mwdc_x2_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->x1WireHit(B_mwdc_x2_hit_wire[i]);
//     } 


//     for(int i = 0; i<B_mwdc_x2p_nhits; i++)
//     {
// #if DEBUG_LEVEL >= 3	
// 	cout<<"Wire x2p : "<<i<<" je: "<<B_mwdc_x2p_hit_wire[i]<<endl;
// #endif
//         detector->mwdc2->x2WireHit(B_mwdc_x2p_hit_wire[i]);
//     } 

    // Now scintillaion plane
    detector->s1xplane->clear();
    detector->s1yplane->clear();
    detector->s2xplane->clear();
    detector->s2yplane->clear();
//     for (int i = 0; i<dE_PN; i++) detector->scintdE->paddleHit(i,B_tp_de_LT[i] ,B_tp_de_RT[i]);
//     for (int i = 0; i<E_PN; i++) detector->scintE->paddleHit(i,B_tp_e_LT[i] ,B_tp_e_RT[i]);

//     // Clear tracks
//     detector->ClearTracks();

//     // Now let's draw partial tracks through wire chambers
// #if DEBUG_LEVEL >= 3	
//     cout<<"Number of Partial Tracks: "<<B_tr_n<<endl;
// #endif

//     if (B_tr_n>0 && fTextButtonTrack->IsOn())
//     {		
// 	for(int q =0; q<B_tr_n; q++)
// 	{
// 	    if (q < 10)
// 	    {
// 		double x0 = B_tr_x[q];
// 		double y0 = B_tr_y[q];
//  		double th = B_tr_th[q];
// 		double ph = B_tr_ph[q];

// 		detector->partialTrack[q]->Track(100.0*x0, 100.0*y0, 0.0, th, ph);
// 	    }
// 	}
  
//     }

// #if FULL_TRACK > 0
//     if (B_tr_n>0 && fTextButtonTrack->IsOn()) // check if we should proceed at all
//     { 	 
//       for(int q = 0; q<B_tr_n; q++)
//       {
// 	if (q<10)  // for now we support only 10 tracks
// 	{

// 	  double BB_px  = cos(BB_angle)*B_tr_px[q] - sin(BB_angle)*B_tr_pz[q];
// 	  double BB_pz  = sin(BB_angle)*B_tr_px[q] + cos(BB_angle)*B_tr_pz[q];
// 	  double BB_py  = B_tr_py[q];
 
// #if DEBUG_LEVEL >= 3	
//           cout<<"----->"<<q<<endl;
//           cout<<"BB.tr.x: "<<B_tr_x[q]<<endl;
// 	  cout<<"BB.tr.x: "<<B_tr_y[q]<<endl;
// 	  cout<<"BB.tr.th: "<<B_tr_th[q]<<endl;
// 	  cout<<"BB.tr.ph: "<<B_tr_ph[q]<<endl;

// 	  cout<<"LAB. Momentum px: "<<B_tr_px[q]<<endl;
// 	  cout<<"LAB. Momentum py: "<<B_tr_py[q]<<endl;
// 	  cout<<"LAB. Momentum pz: "<<B_tr_pz[q]<<endl;
// 	  cout<<"------------------"<<endl;


// 	  cout<<"BB. Momentum px: "<<BB_px<<endl;
// 	  cout<<"BB. Momentum py: "<<BB_py<<endl;
// 	  cout<<"BB. Momentum pz: "<<BB_pz<<endl;
// 	  cout<<"BB. Momentum p: "<<B_tr_p[q]<<endl;

// #endif
// 	  detector->fullTrack[q]->Track(BB_px, BB_py, BB_pz, 100.0*B_tr_x[q], 100.0*B_tr_y[q], 0.0, B_tr_th[q], B_tr_ph[q]);
// 	}
// #if DEBUG_LEVEL >= 3	
// 	else cout<<"Track is not valid!"<<endl;
// #endif
//       }
//     }
// #endif
    

//     detector->mgr->GetMasterVolume()->Raytrace();	
//     c5->Update();
//     cout<<"Thanks! I am done!"<<endl;
  
//   } 
 
// #if DEBUG_LEVEL >= 3	
//   cout<<"Event number is: "<<EventNumber<<endl;
// #endif

      }
}

void EVe::doThis()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber<TotalNumberOfEvents && EventNumber>0)
  {
    DoDraw(EventNumber);
  }  
}

void EVe::doNext()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber<TotalNumberOfEvents)
  {
    EventNumber++;
    fNumberEntry1->SetNumber(EventNumber); 
    DoDraw(EventNumber);
  }  
}
void EVe::doPrevious()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber>0)
  {
    EventNumber--;
    fNumberEntry1->SetNumber(EventNumber);
    DoDraw(EventNumber);
  }  
}


void EVe::doNextGood()
{
  EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
  while (EventNumber<TotalNumberOfEvents) 
  {
    EventNumber++; // rise the number for one;
    t1->GetEntry(EventNumber);
    fNumberEntry1->SetNumber(EventNumber); // write that to the display 
    if (Ndata_H_tr_x > 0) // check if event is good (if there is at least 1 good track).
    { 
	DoDraw(EventNumber);
	break;
    }
  }  
}

void EVe::doPreviousGood()
{
  EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
  while (EventNumber>0) 
  {
    EventNumber--; // lower the number for one;
    t1->GetEntry(EventNumber);
    fNumberEntry1->SetNumber(EventNumber); // write that to the display 
    if (Ndata_H_tr_x > 0) // check if event is good (if there is at least 1 good track).
    { 
	DoDraw(EventNumber);
	break;
    }
  }  
}

void EVe::PrintToFile()
{
  // Routine written by Bryan Mofitt
  TCanvas *fCanvas = fRootEmbeddedCanvas->GetCanvas();
  gStyle->SetPaperSize(20,24);
  static TString dir("printouts");
  TGFileInfo fi;
  const char *myfiletypes[] = 
    { "All files","*",
      "PostScript files","*.ps",
      "Encapsulated PostScript files","*.eps",
      "GIF files","*.gif",
      "JPG files","*.jpg",
      0,               0 };
  fi.fFileTypes = myfiletypes;
  fi.fIniDir    = StrDup(dir.Data());

  new TGFileDialog(gClient->GetRoot(), fMainFrame, kFDSave, &fi);
  if(fi.fFilename!=NULL) fCanvas->Print(fi.fFilename);
}





