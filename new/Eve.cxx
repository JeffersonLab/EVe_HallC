///////////////////////////////////////
/*  Eve.h  6/13/14
    
    Ben Davis-Purcell

    Event Display main class

*/
///////////////////////////////////////

#include "Eve.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <TApplication.h>
#include <TROOT.h>

#include "Eve_DB.h"



using namespace std;

Eve::Eve(const TGWindow *p, UInt_t w, UInt_t h)
{
   // main frame
   fMainFrame = new TGMainFrame(p, w, h, kMainFrame | kVerticalFrame);
   fMainFrame->SetLayoutBroken(kTRUE);
   fMainFrame->SetWindowName("Event Viewer - EVE");
   fMainFrame->Connect("CloseWindow()", "Eve", this, "MyCloseWindow()");

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
  
   // fTextButtonWires = new TGRadioButton(fGroupFrame3,"Planar view");
   // fGroupFrame3->AddFrame(fTextButtonWires, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   // fTextButtonWires->MoveResize(8,24,106,20);

   // fTextButton3dView = new TGRadioButton(fGroupFrame3,"3D View");
   // fGroupFrame3->AddFrame(fTextButton3dView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   // fTextButton3dView->MoveResize(8,40,106,20);

   // fTextButtonProj = new TGRadioButton(fGroupFrame3,"Projection view");
   // fGroupFrame3->AddFrame(fTextButtonProj, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   // fTextButtonProj->MoveResize(8,56,106,20);

   // fTextButtonWires->SetState(kButtonDown);

   fMainFrame->AddFrame(fGroupFrame3, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame3->MoveResize(w-16-128-128, 100,128,100);


   /// Wire Chambers buttons to be added




   /// Option buttons to show various things to be added





   //________________________ Buttons___________________________________

   fNumberEntry1 = new TGNumberEntry(fMainFrame, (Double_t) 0,8,-1,(TGNumberFormat::EStyle) 5);
   fMainFrame->AddFrame(fNumberEntry1, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry1->MoveResize(w - 260, 350,100,30);

   TGTextButton *fTextButtonSE = new TGTextButton(fMainFrame,"Show this Event");
   fMainFrame->AddFrame(fTextButtonSE, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonSE->MoveResize(w-150, 350,100,30);

   TGTextButton *fTextButtonNext = new TGTextButton(fMainFrame,"Next");
   fMainFrame->AddFrame(fTextButtonNext, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonNext->MoveResize(w-150, 390,100,30);

   TGTextButton *fTextButtonPrevious = new TGTextButton(fMainFrame,"Previous");
   fMainFrame->AddFrame(fTextButtonPrevious, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonPrevious->MoveResize(w -260, 390 ,100,30);

   TGTextButton *fTextButtonNextGood = new TGTextButton(fMainFrame,"Next Good");
   fMainFrame->AddFrame(fTextButtonNextGood, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButtonNextGood->MoveResize(w - 150, 430, 100,30);

   TGTextButton *fTextButtonPreviousGood = new TGTextButton(fMainFrame,"Previous Good");
   fMainFrame->AddFrame(fTextButtonPreviousGood, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
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
   fMainFrame->Resize(w,h);


   EventNumber = 0;
   //cst = new CStransform(2.2, 0.5, 0.3);


   fTextButtonNext->Connect("Clicked()", "Eve", this, "doNext()");
   fTextButtonPrevious->Connect("Clicked()", "Eve", this, "doPrevious()");
   fTextButtonSE->Connect("Clicked()", "Eve", this, "doThis()");
   fTextButtonNextGood->Connect("Clicked()", "Eve", this, "doNextGood()");
   fTextButtonPreviousGood->Connect("Clicked()", "Eve", this, "doPreviousGood()");
   fTextButtonPrint->Connect("Clicked()", "Eve", this, "PrintToFile()");




}


   /// Destructor
Eve::~Eve()
{
  fMainFrame->Cleanup();
  delete fMainFrame;
}

void Eve::MyCloseWindow()
{
  fMainFrame->SendCloseMessage();
  gApplication->Terminate();
}

//// Ignore all views except for scint planar for now


/// void Eve::Create3DView() {  }


/// void Eve::CreateUprojection() {  }


/// void Eve::CreateVprojection() {  }


/// void Eve::CreateXprojection() {  }


void Eve::CreateWires()
{
  fRootEmbeddedCanvas->AdoptCanvas(c3);
  c3->cd();
  c3->Clear();

  CStransform *dE_cst = new CStransform(canvas_length, canvas_dE_posx, canvas_dE_posy);
  CStransform *E_cst = new CStransform(canvas_length, canvas_E_posx, canvas_E_posy);

  sdE = new ScintPlane((char*)"dE-plane", dE_PN, dE_length, dE_height, dE_cst);
  sE = new ScintPlane((char*)"E-plane", E_PN, E_length, E_height, E_cst);
  
  TArrow *xaxis = new TArrow(0.95, 0.15, 0.95, 0.02, 0.02, "|>");
  xaxis->Draw();
  TArrow *yaxis = new TArrow(0.95, 0.15, 0.82, 0.15, 0.02, "|>");
  yaxis->Draw();
  
  TLatex *xtext = new TLatex(0.965,0.03, "x");
  xtext->SetTextSize(0.03);
  xtext->Draw();  
  
  TLatex *ytext = new TLatex(0.83, 0.122, "y");
  ytext->SetTextSize(0.03);
  ytext->Draw(); 
  
  c3->Update();
}



void Eve::initRun(char *filename)
{
   cout << "Reading file " << endl;
   TFile *f1 = new TFile(filename, "r");
   t1 = (TTree*) f1->Get("T");
   TotalNumberOfEvents =  t1->GetEntries();

   THaRun* runinfo = (THaRun*)gROOT->FindObject("Run_Data");
   if(runinfo==NULL) run_number = 0;
   else run_number = runinfo->GetNumber(); 
   cout<<"RUN #: "<<run_number<<endl;

   /// Insert all pointers from tree (t1) to specific branches here

   /////projection wires
   // t1->SetBranchAddress( "BB.tr.n", &B_tr_n);
   // t1->SetBranchAddress( "BB.tr.x", &B_tr_x);
   // t1->SetBranchAddress( "BB.tr.y", &B_tr_y);
   // t1->SetBranchAddress( "BB.tr.ph", &B_tr_ph);
   // t1->SetBranchAddress( "BB.tr.th", &B_tr_th);
   
   t1->SetBranchAddress( "BB.tp.e.nhit", &B_tp_e_nhit);
   t1->SetBranchAddress( "BB.tp.e.LT", &B_tp_e_LT);
   t1->SetBranchAddress( "BB.tp.e.RT", &B_tp_e_RT);
   t1->SetBranchAddress( "BB.tp.e.hit_ypos", &B_tp_e_hit_ypos);
   t1->SetBranchAddress( "BB.tp.e.hit_bar", &B_tp_e_hit_bar);
   
   t1->SetBranchAddress( "BB.tp.de.nhit", &B_tp_de_nhit);
   t1->SetBranchAddress( "BB.tp.de.LT", &B_tp_de_LT);
   t1->SetBranchAddress( "BB.tp.de.RT", &B_tp_de_RT);
   t1->SetBranchAddress( "BB.tp.de.hit_ypos", &B_tp_de_hit_ypos);
   t1->SetBranchAddress( "BB.tp.de.hit_bar", &B_tp_de_hit_bar);
   
}



void Eve::DoDraw(int event)
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

   /// u-projection view

   /// v-projection view

   /// x-projection view


   /// Planar view
    
   //   if (fTextButtonWires->IsOn())
   //   {
       c3->cd();

    
       if (title != NULL) delete title;
       title = new TLatex(0.02,0.02, graph_title);
       title->SetTextSize(0.03);
       title->Draw();  
  
   ////////MWDC1
   ////////MWDC2

   //////// Scintillation Planes

       double Ebar_ypos[E_PN];
       double dEbar_ypos[dE_PN];

       for (int q = 0; q<E_PN; q++) Ebar_ypos[q] = 0.0;
       for (int q = 0; q<dE_PN; q++) dEbar_ypos[q] = 0.0;
	
       for (Int_t q = 0; q<B_tp_e_nhit; q++)
	 {
	   int bar = (int)(B_tp_e_hit_bar[q]);
	   double ypos = B_tp_e_hit_ypos[q];
	   Ebar_ypos[bar] = ypos;
	 }
       for (int q = 0; q<B_tp_de_nhit; q++)
	 {
	   int bar = (int)(B_tp_de_hit_bar[q]);
	   double ypos = B_tp_de_hit_ypos[q];
	   dEbar_ypos[bar] = ypos;
	 }

       sE->clear();
       sdE->clear();
       for (int i = 0; i<dE_PN; i++)
	 {
	   sdE->paddleHit(i,B_tp_de_LT[i] ,B_tp_de_RT[i] , -dEbar_ypos[i]);
	 }  

       for (int i = 0; i<E_PN; i++)
	 {
	   sE->paddleHit(i,B_tp_e_LT[i] ,B_tp_e_RT[i] , -Ebar_ypos[i]);	
	 }  

       /// Trajectories through detectors



       c3->Draw();
       c3->Update();
       //   }



   /// 3D view


}


void Eve::doThis()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber<TotalNumberOfEvents && EventNumber>0)
  {
    DoDraw(EventNumber);
  }  
}

void Eve::doNext()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber<TotalNumberOfEvents)
  {
    EventNumber++;
    fNumberEntry1->SetNumber(EventNumber); 
    DoDraw(EventNumber);
  }  
}

void Eve::doPrevious()
{
  EventNumber = fNumberEntry1->GetNumber();
  if (EventNumber>0)
  {
    EventNumber--;
    fNumberEntry1->SetNumber(EventNumber);
    DoDraw(EventNumber);
  }  
}


void Eve::doNextGood()
{
  EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
  while (EventNumber<TotalNumberOfEvents) 
  {
    EventNumber++; 
    t1->GetEntry(EventNumber);
    fNumberEntry1->SetNumber(EventNumber); // write to the display 
 
   // if (B_tr_n > 0.0) // check if event is good (if there is at least 1 good track).
    // { 
    // 	DoDraw(EventNumber);
    // 	break;
    // }

  }  
}

void Eve::doPreviousGood()
{
  EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
  while (EventNumber>0) 
  {
    EventNumber--;
    t1->GetEntry(EventNumber);
    fNumberEntry1->SetNumber(EventNumber); // write to the display 

    // if (B_tr_n > 0.0) // check if event is good (if there is at least 1 good track).
    // { 
    // 	DoDraw(EventNumber);
    // 	break;
    // }

  }  
}

void Eve::PrintToFile()
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

