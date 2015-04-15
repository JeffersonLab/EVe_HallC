//************************************************************************* 
//  EVe.cxx  - 4/14/2008
// 
//  by miham
// 
//  This is the main class of the event display. 
// 
//  Updated by Ben Davis-Purcell - 8/26/14
//
//*************************************************************************


// FIXME: FULL tracks are not working yet properly because
// precise positions of the target, chambers etc. are unknown 

#include "EVe.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <TApplication.h>
#include <TROOT.h>

#include <map>
#include <string>
#include <vector>

#define DEBUG_LEVEL  0

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


   //______________ First group of buttons____________________
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


   //______________ Second group of buttons____________________
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

   //______________ Third group of buttons____________________
   // Here are check buttons for options 
   TGButtonGroup *fGroupFrame2 = new TGButtonGroup(fMainFrame,"Options", kVerticalFrame);
   fGroupFrame2->SetLayoutBroken(kTRUE);
   //  fTextButtonRoads = new TGCheckButton(fGroupFrame2,"Show Roads");
   //fGroupFrame2->AddFrame(fTextButtonRoads, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fTextButtonRoads->MoveResize(8,24,106,20);

   //fTextButtonRoads->SetState(kButtonDown);

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
  
   for (int q = 0; q<MAX_ROADS_NUM; q++) utr[q] = new Track(cst);
      
   for (int q = 0; q<MAX_ROADS_NUM; q++) vtr[q] = new Track(cst);

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

     GetVariables *BHvar = new GetVariables("BH.txt");

     x1 = new WirePlane((char*)"X1",BHvar->GetDouble("RDC1.x.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("x1.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,-1);
   
     x1p = new WirePlane((char*)"X1p",BHvar->GetDouble("RDC1.xp.NumWires ="),
			 -0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0), 
			 cst->transYtoCY(BHvar->GetDouble("x1p.dist =")),
			 cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,+1);

     x2 = new WirePlane((char*)"X2",BHvar->GetDouble("RDC2.x.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("x2.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,-1);
  
     x2p = new WirePlane((char*)"X2p",BHvar->GetDouble("RDC2.xp.NumWires ="),
			 -0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0), 
			 cst->transYtoCY(BHvar->GetDouble("x2p.dist =")),
			 cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,+1);

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
     
     GetVariables *BHvar = new GetVariables("BH.txt");

     u1 = new WirePlane((char*)"U1",BHvar->GetDouble("RDC1.u.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("u1.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,-1);
     v1 = new WirePlane((char*)"V1",BHvar->GetDouble("RDC1.v.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0),
			cst->transYtoCY(0.002+BHvar->GetDouble("v1.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,+1);

     u2 = new WirePlane((char*)"U2",BHvar->GetDouble("RDC2.u.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("u2.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,-1);
     v2 = new WirePlane((char*)"V2",BHvar->GetDouble("RDC2.v.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(0.002+BHvar->GetDouble("v2.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,+1);

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
     
     GetVariables *BHvar = new GetVariables("BH.txt");

     y1 = new WirePlane((char*)"Y1",BHvar->GetDouble("RDC1.y.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("y1.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,-1);
     y1p = new WirePlane((char*)"Y1p",BHvar->GetDouble("RDC1.yp.NumWires ="),
			 -0.5*cst->transLtoCL(BHvar->GetDouble("RDC1.Height ="))+cst->transXtoCX(0.0), 
			 cst->transYtoCY(BHvar->GetDouble("y1p.dist =")),
			 cst->transLtoCL(BHvar->GetDouble("RDC1.Height =")),1.0,1.0,1);

     y2 = new WirePlane((char*)"Y2",BHvar->GetDouble("RDC2.y.NumWires ="),
			-0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0), 
			cst->transYtoCY(BHvar->GetDouble("y2.dist =")),
			cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,-1);
     y2p = new WirePlane((char*)"Y2p",BHvar->GetDouble("RDC2.yp.NumWires ="),
			 -0.5*cst->transLtoCL(BHvar->GetDouble("RDC2.Height ="))+cst->transXtoCX(0.0),
			 cst->transYtoCY(BHvar->GetDouble("y2p.dist =")),
			 cst->transLtoCL(BHvar->GetDouble("RDC2.Height =")),1.0,1.0,+1);

     c4->Update();
   }
}


void EVe::CreateWires()
{
   fRootEmbeddedCanvas->AdoptCanvas(c3);
   c3->cd();
   c3->Clear();
   
   //below are data used for positioning Chambers2D view in canvas, can be changed in BH.txt
   GetVariables *vars = new GetVariables("BH.txt");
   double canvasL = vars ->GetDouble("canvasL =");
   double RDC1X = vars ->GetDouble("canvas.RDC1.x =");
   double RDC1Y = vars ->GetDouble("canvas.RDC1.y =");
   double RDC2X = vars ->GetDouble("canvas.RDC2.x =");
   double RDC2Y = vars ->GetDouble("canvas.RDC2.y =");
   
   double LDC1X = vars ->GetDouble("canvas.LDC1.x =");
   double LDC1Y = vars ->GetDouble("canvas.LDC1.y =");
   double LDC2X = vars ->GetDouble("canvas.LDC2.x =");
   double LDC2Y = vars ->GetDouble("canvas.LDC2.y =");

 
   int NScintPlanes = vars->GetInt("Number of Scint Planes =");

   string PN[6]={"x", "y", "u", "v", "yp", "xp"};   
   vector<string> planeNames(&PN[0],&PN[0]+6);

   CStransform *RDC1_cst = new CStransform(canvasL, RDC1X, RDC1Y);
   CStransform *RDC2_cst = new CStransform(canvasL, RDC2X, RDC2Y);

   CStransform *LDC1_cst = new CStransform(canvasL, LDC1X, LDC1Y);
   CStransform *LDC2_cst = new CStransform(canvasL, LDC2X, LDC2Y);
   
   RDC1 = new WireChamber((char*)"RDC1",RDC1_cst);

   RDC2 = new WireChamber((char*)"RDC2",RDC2_cst);
   
   LDC1 = new WireChamber((char*)"LDC1",LDC1_cst);

   LDC2 = new WireChamber((char*)"LDC2",LDC2_cst);
  
/// Variables to generate scintillator planes planar view   

    double CLsX = vars -> GetDouble("canvas.Ls.x =");
    double CLsY = vars -> GetDouble("canvas.Ls.y =");

    double CRsX = vars -> GetDouble("canvas.Rs.x =");
    double CRsY = vars -> GetDouble("canvas.Rs.y =");
  
   CStransform *Ls_cst = new CStransform(canvasL, CLsX, CLsY);
   CStransform *Rs_cst = new CStransform(canvasL, CRsX, CRsY);

   Ls = new ScintPlane((char*)"Ls", Ls_cst);

   Rs = new ScintPlane((char*)"Rs", Rs_cst);

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

/*
void EVe::SetBranchAddress(TTree* T, const char* bname, Double_t** addr, int size)
      {
        //Double_t *tmp = (Double_t*) malloc(size*sizeof(Double_t));  (unsigned int)
        Double_t *tmp = (Double_t*) malloc(size*sizeof(Double_t));          
               
        if( tmp == NULL ) {
           cerr << Form("-!-> (%s:%d) Can't allocate memory for bname '%s'",
                          __FILE__, __LINE__, bname) << endl;
           exit(1);
        } else {
	  //ver 1 , uninitialized numbers	   
	  T->SetBranchAddress( bname, &tmp);
           *addr = tmp;
          
	  //ver2 , after hit still get 0 for all numbers, negtive index, no red or green shown
          //for (int i=0; i<size ;i++)
	  //  tmp[i]=0;
          //T->SetBranchAddress( bname, &tmp);
          //*addr = tmp;
          
          //ver3 ,same as ver2
          //for (int i=0; i<size ;i++)
	  //  tmp[i]=0;
          //*addr = tmp;
          //T->SetBranchAddress( bname, &tmp);
          
           //ver4 ,same as ver2 before call hitfunc, show event seg fault
          //for (int i=0; i<size ;i++)
	  //  tmp[i]=0;
          //T->SetBranchAddress( bname, &tmp);
          //addr = &tmp;

          //ver5, unsigned int to int, other use ver2, same as ver2
          //for (int i=0; i<size ;i++)
	  //  tmp[i]=0;
          //T->SetBranchAddress( bname, &tmp);
          //*addr = tmp;
          
        }
        //cerr << " Size of the scintarray is " << size << " , and size of *tmp is " << sizeof(*tmp) << " , and size of *addr is " << sizeof(*addr) << endl;
        for (int i=0; i<size ; i++)
	  cerr << "Value for "<< bname << "  " << i << "th number of addr is " << (*addr)[i] << endl;
        // cerr << "Now the " << bname << " has " << (double) (*addr) << " hitted." << endl ;
      }

*/

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

   /// Branch Addresses for all root Tree leaves that are needed 

   /// Wire Chamber Nhits
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

   /// UV Wire plane hits and hit times
   t1->SetBranchAddress( "H.dc.1u1.tdchits", &H_dc_1u1_tdchits);
   t1->SetBranchAddress( "H.dc.1v1.tdchits", &H_dc_1v1_tdchits);
   t1->SetBranchAddress( "H.dc.2u1.tdchits", &H_dc_2u1_tdchits);
   t1->SetBranchAddress( "H.dc.2v1.tdchits", &H_dc_2v1_tdchits);

   t1->SetBranchAddress( "H.dc.1u1.time", &H_dc_1u1_time);
   t1->SetBranchAddress( "H.dc.1v1.time", &H_dc_1v1_time);
   t1->SetBranchAddress( "H.dc.2u1.time", &H_dc_2u1_time);
   t1->SetBranchAddress( "H.dc.2v1.time", &H_dc_2v1_time);

   /// X Wire plane hits and hit times
   t1->SetBranchAddress( "H.dc.1x1.tdchits", &H_dc_1x1_tdchits);
   t1->SetBranchAddress( "H.dc.1x2.tdchits", &H_dc_1x2_tdchits);
   t1->SetBranchAddress( "H.dc.2x1.tdchits", &H_dc_2x1_tdchits);
   t1->SetBranchAddress( "H.dc.2x2.tdchits", &H_dc_2x2_tdchits);

   t1->SetBranchAddress( "H.dc.1x1.time", &H_dc_1x1_time);
   t1->SetBranchAddress( "H.dc.1x2.time", &H_dc_1x2_time);
   t1->SetBranchAddress( "H.dc.2x1.time", &H_dc_2x1_time);
   t1->SetBranchAddress( "H.dc.2x2.time", &H_dc_2x2_time);

   /// Y Wire plane hits and hit times
   t1->SetBranchAddress( "H.dc.1y1.tdchits", &H_dc_1y1_tdchits);
   t1->SetBranchAddress( "H.dc.1y2.tdchits", &H_dc_1y2_tdchits);
   t1->SetBranchAddress( "H.dc.2y1.tdchits", &H_dc_2y1_tdchits);
   t1->SetBranchAddress( "H.dc.2y2.tdchits", &H_dc_2y2_tdchits);

   t1->SetBranchAddress( "H.dc.1y1.time", &H_dc_1y1_time);
   t1->SetBranchAddress( "H.dc.1y2.time", &H_dc_1y2_time);
   t1->SetBranchAddress( "H.dc.2y1.time", &H_dc_2y1_time);
   t1->SetBranchAddress( "H.dc.2y2.time", &H_dc_2y2_time);

   /// TRACKS
   t1->SetBranchAddress( "Ndata.H.tr.x", &Ndata_H_tr_x);
   t1->SetBranchAddress( "H.tr.x", &H_tr_x);
   t1->SetBranchAddress( "H.tr.y", &H_tr_y);
   t1->SetBranchAddress( "H.tr.ph", &H_tr_ph);
   t1->SetBranchAddress( "H.tr.th", &H_tr_th); 
  

   /// Scint Planes
   /// TDC hits for all planes -- pos = left PMT, neg = right PMT

   /// S1X
   /*
   t1->SetBranchAddress( "Ndata.H.hod.1x.negtdchits", &Ndata_H_hod_1x_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.1x.postdchits", &Ndata_H_hod_1x_postdchits);
   t1->SetBranchAddress( "H.hod.1x.negtdchits", &H_hod_1x_negtdchits);
   t1->SetBranchAddress( "H.hod.1x.postdchits", &H_hod_1x_postdchits);
   
   
   //SetBranchAddress(t1, (const char*)"H.hod.1x.negtdchits",&H_hod_1x_negtdchits,HMS->GetInt("s1x.PN ="));
   //SetBranchAddress(t1, (const char*)"H.hod.1x.postdchits",&H_hod_1x_postdchits,HMS->GetInt("s1x.PN ="));
   
   for(int i=0;i<Ndata_H_hod_1x_negtdchits; i++)
     cerr << "After Call the number of " << i << "th of 1xnegtdchits is " << (Double_t)(H_hod_1x_negtdchits[i]) << endl;
   for(int i=0;i<Ndata_H_hod_1x_postdchits; i++)
     cerr << "After Call the number of " << i << "th of 1xpostdchits is " << (Double_t)(H_hod_1x_postdchits[i]) << endl;
   */
   
   ///Rs
   t1->SetBranchAddress( "Ndata.H.hod.1y.negtdchits", &Ndata_H_hod_1y_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.1y.postdchits", &Ndata_H_hod_1y_postdchits);
   t1->SetBranchAddress( "H.hod.1y.negtdchits", &H_hod_1y_negtdchits);
   t1->SetBranchAddress( "H.hod.1y.postdchits", &H_hod_1y_postdchits);
   //SetBranchAddress(t1, (const char*)"H.hod.1y.negtdchits",&H_hod_1y_negtdchits,HMS->GetInt("Rs.PN ="));
   //SetBranchAddress(t1, (const char*)"H.hod.1y.postdchits",&H_hod_1y_postdchits,HMS->GetInt("Rs.PN ="));
    
   
   /* for(int i=0;i<16; i++)
     cerr << "After Call the number of " << i << "th of 1ynegtdchits is " << H_hod_1y_negtdchits[i] << endl;
    for(int i=0;i<16; i++)
    cerr << "After Call the number of " << i << "th of 1ypostdchits is " << H_hod_1y_postdchits[i] << endl;

   /*
   ///S2X
   t1->SetBranchAddress( "Ndata.H.hod.2x.negtdchits", &Ndata_H_hod_2x_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.2x.postdchits", &Ndata_H_hod_2x_postdchits);
   //t1->SetBranchAddress( "H.hod.2x.negtdchits", &H_hod_2x_negtdchits);
   //t1->SetBranchAddress( "H.hod.2x.postdchits", &H_hod_2x_postdchits);  
   SetBranchAddress(t1, (const char*)"H.hod.2x.negtdchits",&H_hod_2x_negtdchits,HMS->GetInt("s2x.PN ="));
   SetBranchAddress(t1, (const char*)"H.hod.2x.postdchits",&H_hod_2x_postdchits,HMS->GetInt("s2x.PN ="));
   /*
    for(int i=0;i<16; i++)
     cerr << "After Call the number of " << i << "th of 2xnegtdchits is " << H_hod_2x_negtdchits[i] << endl;
    for(int i=0;i<16; i++)
    cerr << "After Call the number of " << i << "th of 2xpostdchits is " << H_hod_2x_postdchits[i] << endl; */
   /*
   ///S2Y
   t1->SetBranchAddress( "Ndata.H.hod.2y.negtdchits", &Ndata_H_hod_2y_negtdchits);
   t1->SetBranchAddress( "Ndata.H.hod.2y.postdchits", &Ndata_H_hod_2y_postdchits);
   //t1->SetBranchAddress( "H.hod.2y.negtdchits", &H_hod_2y_negtdchits);
   //t1->SetBranchAddress( "H.hod.2y.postdchits", &H_hod_2y_postdchits);
   SetBranchAddress(t1, (const char*)"H.hod.2y.negtdchits",&H_hod_2y_negtdchits,HMS->GetInt("s2y.PN ="));
   SetBranchAddress(t1, (const char*)"H.hod.2y.postdchits",&H_hod_2y_postdchits,HMS->GetInt("s2y.PN ="));
   /*
   for(int i=0;i<16; i++)
     cerr << "After Call the number of " << i << "th of 2ynegtdchits is " << H_hod_2y_negtdchits[i] << endl;
   for(int i=0;i<16; i++)
   cerr << "After Call the number of " << i << "th of 2ypostdchits is " << H_hod_2y_postdchits[i] << endl;*/

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

    c1->Draw();
    c1->Update();
  }

// //_________________________ Lets handle UV-projection view ______________________________

  if (fTextButtonProj->IsOn() && fTextButtonUVPlane->IsOn())
    {

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

      c2->Draw();
      c2->Update(); 
    }

// //_________________________ Lets handle Y-projection view ______________________________

  if (fTextButtonProj->IsOn() && fTextButtonYPlane->IsOn())
    {

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
  
      c4->Draw();
      c4->Update(); 
    }

//_________________________ Lets handle Planar view  ______________________________

  if (fTextButtonWires->IsOn())
  {
    c3->cd();

    
    if (title != NULL) delete title;
    title = new TLatex(0.02,0.02, graph_title);
    title->SetTextSize(0.03);
    title->Draw();   

    //***************** First Wire Chamber

    RDC1->clear();
    
    GetVariables* BHvars= new GetVariables("BH.txt");
    
    int x1NW= BHvars->GetInt("RDC1.x.NumWires =");
    int v1NW= BHvars->GetInt("RDC1.v.NumWires =");
    int yp1NW= BHvars->GetInt("RDC1.yp.NumWires =");

    /// X plane

    for(int i = 0; i<Ndata_H_dc_1x1_tdchits; i++)
      {
	
        RDC1->WireHit("x",x1NW+1-H_dc_1x1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1x2_tdchits; i++)
      {
	
        RDC1->WireHit("xp",H_dc_1x2_tdchits[i]);
      } 

    /// UV plane

    for(int i = 0; i<Ndata_H_dc_1u1_tdchits; i++)
      {
	
        RDC1->WireHit("u",H_dc_1u1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1v1_tdchits; i++)
      {
	
        RDC1->WireHit("v",v1NW+1-H_dc_1v1_tdchits[i]);
      } 

    /// YP,XP plane

    for(int i = 0; i<Ndata_H_dc_1y1_tdchits; i++)
      {
	
        RDC1->WireHit("y",H_dc_1y1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_1y2_tdchits; i++)
      {
	
        RDC1->WireHit("yp",yp1NW+1-H_dc_1y2_tdchits[i]);
      } 


   //***************** Second Wire Chamber

    RDC2->clear();

    
    int x2NW= BHvars->GetInt("RDC2.x.NumWires =");
    int v2NW= BHvars->GetInt("RDC2.v.NumWires =");
    int yp2NW= BHvars->GetInt("RDC2.yp.NumWires =");

    /// X plane

    for(int i = 0; i<Ndata_H_dc_2x1_tdchits; i++)
      {
	
        RDC2->WireHit("x",x2NW+1-H_dc_2x1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2x2_tdchits; i++)
      {
	
        RDC2->WireHit("xp",H_dc_2x2_tdchits[i]);
      } 

    /// UV plane

    for(int i = 0; i<Ndata_H_dc_2u1_tdchits; i++)
      {
	
        RDC2->WireHit("u",H_dc_2u1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2v1_tdchits; i++)
      {
	
        RDC2->WireHit("v",v2NW+1-H_dc_2v1_tdchits[i]);
      } 

    /// Y plane

    for(int i = 0; i<Ndata_H_dc_2y1_tdchits; i++)
      {
	
        RDC2->WireHit("y",H_dc_2y1_tdchits[i]);
      } 
    
    for(int i = 0; i<Ndata_H_dc_2y2_tdchits; i++)
      {
	
        RDC2->WireHit("yp",yp2NW+1-H_dc_2y2_tdchits[i]);
      } 
    
    LDC1->clear();
    LDC2->clear();
   
    /*
    s1X->SPHit2D(Ndata_H_hod_1x_postdchits,Ndata_H_hod_1x_negtdchits,H_hod_1x_postdchits,H_hod_1x_negtdchits);
    for(int i=0;i<16; i++)
      cerr << "After hit the number of " << i << "th of 1xnegtdchits is " << (Double_t)(H_hod_1x_negtdchits[i]) << endl;
    for(int i=0;i<16; i++)
      cerr << "After hit the number of " << i << "th of 1xpostdchits is " << (Double_t)(H_hod_1x_postdchits[i]) << endl;
    
    
    s1Y->SPHit2D(Ndata_H_hod_1y_postdchits,Ndata_H_hod_1y_negtdchits,H_hod_1y_postdchits,H_hod_1y_negtdchits);
    s2X->SPHit2D(Ndata_H_hod_2x_postdchits,Ndata_H_hod_2x_negtdchits,H_hod_2x_postdchits,H_hod_2x_negtdchits);
    s2Y->SPHit2D(Ndata_H_hod_2y_postdchits,Ndata_H_hod_2y_negtdchits,H_hod_2y_postdchits,H_hod_2y_negtdchits);
    */
    
    
    Ls->clear();
    Rs->clear();
 
    Rs->SPHit2D(Ndata_H_hod_1x_postdchits,Ndata_H_hod_1x_negtdchits,H_hod_1x_postdchits,H_hod_1x_negtdchits);

    Ls->SPHit2D(Ndata_H_hod_1y_postdchits,Ndata_H_hod_1y_negtdchits,H_hod_1y_postdchits,H_hod_1y_negtdchits);
    

//****** Now we draw Trajectories through detectors, since data only used for HMS, now commented out
/*    	 
   if (Ndata_H_tr_x > 0 && fTextButtonTrack->IsOn())
     {		
       for(int q =0; q<Ndata_H_tr_x; q++)
	 {
	   GetVariables *hms = new GetVariables("HMS.txt");
           double z1 = hms->GetDouble("MWDC2.z =");
	   double z3 = hms->GetDouble("s1x.z =");
	   double z4 = hms->GetDouble("s1y.z =");
	   double z5 = hms->GetDouble("s2x.z =");
	   double z6 = hms->GetDouble("s2y.z =");

	   /// Real track information
	   double x0 = -H_tr_y[q]/100; /// [cm] to [m]
	   double y0 = -H_tr_x[q]/100;
	   double th = -H_tr_ph[q];
	   double ph = -H_tr_th[q];

	    
	   double x1 = x0 + th*z1;
	   double y1 = y0 + ph*z1;    


	   RDC1->Track(x0,y0,q);
	   RDC2->Track(x1,y1,q);


	  
	   double x3 = x0 + th*z3;
	   double y3 = y0 + ph*z3;
	   
	   s1X->Track(x3,y3,q);
	   
	   ;
	   double x4 = x0 + th*z4;
	   double y4 = y0 + ph*z4;
	   
	   s1Y->Track(x4,y4,q);
	   
	   
	   double x5 = x0 + th*z5;
	   double y5 = y0 + ph*z5;
	   
	   s2X->Track(x5,y5,q);
	   
	   
	   double x6 = x0 + th*z6;
	   double y6 = y0 + ph*z6;
	   
	   s2Y->Track(x6,y6,q);
	 }
       
     }  

*/
   c3->Draw();
   c3->Update();
   
  
  

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
    GetVariables* HMSvars= new GetVariables("HMS.txt");
    int x1NW= HMSvars->GetInt("MWDC1.x.NumWires =");
    int v1NW= HMSvars->GetInt("MWDC1.v.NumWires =");
    int y1NW= HMSvars->GetInt("MWDC1.y.NumWires =");

    detector->MWDC1->clear();
    //X,Xp,Plane
    for(int i = 0; i<Ndata_H_dc_1x1_tdchits; i++)
        detector->MWDC1->WireHit3D("x",x1NW+1-H_dc_1x1_tdchits[i]);
    for(int i = 0; i<Ndata_H_dc_1x2_tdchits; i++)
        detector->MWDC1->WireHit3D("xp",H_dc_1x2_tdchits[i]);
    /// UV plane
    for(int i = 0; i<Ndata_H_dc_1u1_tdchits; i++)
        detector->MWDC1->WireHit3D("u",H_dc_1u1_tdchits[i]);
    for(int i = 0; i<Ndata_H_dc_1v1_tdchits; i++)
    detector->MWDC1->WireHit3D("v",v1NW+1-H_dc_1v1_tdchits[i]);
    /// Y,XP plane
    for(int i = 0; i<Ndata_H_dc_1y1_tdchits; i++)
      detector->MWDC1->WireHit3D("y",H_dc_1y1_tdchits[i]);
    for(int i = 0; i<Ndata_H_dc_1y2_tdchits; i++)
      detector->MWDC1->WireHit3D("yp",y1NW+1-H_dc_1y2_tdchits[i]);

    detector->MWDC2->clear();
    
    //X,Xp,Plane
    for(int i = 0; i<Ndata_H_dc_2x1_tdchits; i++)
        detector->MWDC2->WireHit3D("x",x1NW+1-H_dc_2x1_tdchits[i]);
    for(int i = 0; i<Ndata_H_dc_2x2_tdchits; i++)
        detector->MWDC2->WireHit3D("xp",H_dc_2x2_tdchits[i]);
    /// UV plane
    for(int i = 0; i<Ndata_H_dc_2u1_tdchits; i++)
        detector->MWDC2->WireHit3D("u",H_dc_2u1_tdchits[i]);

    
    for(int i = 0; i<Ndata_H_dc_2v1_tdchits; i++)
        detector->MWDC2->WireHit3D("v",v1NW+1-H_dc_2v1_tdchits[i]);
    /// Y,XP plane
    for(int i = 0; i<Ndata_H_dc_2y1_tdchits; i++)
        detector->MWDC2->WireHit3D("y",H_dc_2y1_tdchits[i]);
    for(int i = 0; i<Ndata_H_dc_2y2_tdchits; i++)
        detector->MWDC2->WireHit3D("yp",y1NW+1-H_dc_2y2_tdchits[i]);
    
     // Now scintillaion plane
    detector->Rsplane->clear();
    detector->Lsplane->clear();
 
    detector->Rsplane->SPHit(Ndata_H_hod_1y_postdchits,Ndata_H_hod_1y_negtdchits,H_hod_1y_postdchits,H_hod_1y_negtdchits, (char*)"Rs");
    detector->Lsplane->SPHit(Ndata_H_hod_2y_postdchits,Ndata_H_hod_2y_negtdchits,H_hod_2y_postdchits,H_hod_2y_negtdchits, (char*)"Ls");


     // Clear tracks
     detector->ClearTracks();

     // Now let's draw partial tracks through wire chambers

     if (Ndata_H_tr_x>0 && fTextButtonTrack->IsOn())
     {
       if((unsigned int)Ndata_H_tr_x > 10)
        {
	  //detector->TrackList.size set to 10
            for(int q =0; q<Ndata_H_tr_x; q++)
            {
                double x0 = H_tr_x[q];
                double y0 = H_tr_y[q];
                double th = H_tr_th[q];
                double ph = H_tr_ph[q];

		detector->TrackList[q]->Enable(q,x0,y0,th,ph);
            }
        }

       else if(10>(unsigned int)Ndata_H_tr_x)
        {
	  for(unsigned int q =0; q<10; q++)
            {

	      if(q<(unsigned int)Ndata_H_tr_x)
                {
                    double x0 = H_tr_x[q];
                    double y0 = H_tr_y[q];
                    double th = H_tr_th[q];
                    double ph = H_tr_ph[q];
                    detector->TrackList[q]->Enable(q,x0,y0,th,ph);
                }
                else
                    detector->TrackList[q]->Disable();
            }
        }

     }
   c5->Update();
      }
  }}

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
