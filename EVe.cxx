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
#include <algorithm>
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

    fTextButton3dView = new TGRadioButton(fGroupFrame3,"3D View");
    fGroupFrame3->AddFrame(fTextButton3dView, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fTextButton3dView->MoveResize(8,40,106,20);

    fTextButton3dView->SetState(kButtonDown);

    fMainFrame->AddFrame(fGroupFrame3, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fGroupFrame3->MoveResize(w-16-128-128, 100,128,100);


    //______________ Third group of buttons____________________
    // Here are check buttons for options
    TGButtonGroup *fGroupFrame2 = new TGButtonGroup(fMainFrame,"Options", kVerticalFrame);
    fGroupFrame2->SetLayoutBroken(kTRUE);

    fTextButtonTrack = new TGCheckButton(fGroupFrame2,"Show Tracks");
    fGroupFrame2->AddFrame(fTextButtonTrack, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fTextButtonTrack->MoveResize(8,40,106,20);

    fTextButtonTrack->SetState(kButtonDown);

    fGroupFrame2->SetLayoutManager(new TGVerticalLayout(fGroupFrame2));
    fGroupFrame2->Resize(128,96);
    fMainFrame->AddFrame(fGroupFrame2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fGroupFrame2->MoveResize(w-8-128, 100,128, 100);


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


    if (fTextButton3dView->IsOn()) Create3DView();

    fTextButton3dView->Connect("Clicked()", "EVe", this, "Create3DView()");

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
    if (fTextButton3dView->IsOn()) Create3DView();
}

void EVe::CreateXprojection() {}

void EVe::CreateUVprojection() {}

void EVe::CreateYprojection() {}


void EVe::CreateWires()
{
    fRootEmbeddedCanvas->AdoptCanvas(c3);
    c3->cd();
    c3->Clear();

    GetVariables *vars = new GetVariables("SHMS.txt");
    double canvasL = vars ->GetDouble("canvasL =");
    double CMWDC1X = vars ->GetDouble("canvas.MWDC1.x =");
    double CMWDC1Y = vars ->GetDouble("canvas.MWDC1.y =");
    double CMWDC2X = vars ->GetDouble("canvas.MWDC2.x =");
    double CMWDC2Y = vars ->GetDouble("canvas.MWDC2.y =");

    CStransform *mwdc1_cst = new CStransform(canvasL, CMWDC1X, CMWDC1Y);
    CStransform *mwdc2_cst = new CStransform(canvasL, CMWDC2X, CMWDC2Y);

    mwdc1 = new WireChamber((char*)"MWDC1", vars, mwdc1_cst);
    mwdc2 = new WireChamber((char*)"MWDC2", vars, mwdc2_cst);

    int NScintPlanes = vars->GetInt("Number of Scint Planes =");

    double Cs1xX = vars -> GetDouble("canvas.s1x.x =");
    double Cs1xY = vars -> GetDouble("canvas.s1x.y =");
    double Cs1yX = vars -> GetDouble("canvas.s1y.x =");
    double Cs1yY = vars -> GetDouble("canvas.s1y.y =");

    double Cs2xX = vars -> GetDouble("canvas.s2x.x =");
    double Cs2xY = vars -> GetDouble("canvas.s2x.y =");
    double Cs2yX = vars -> GetDouble("canvas.s2y.x =");
    double Cs2yY = vars -> GetDouble("canvas.s2y.y =");

    //ajust scintplane position in 2D canvas
    CStransform *s1x_cst = new CStransform(canvasL, Cs1xX, Cs1xY);
    CStransform *s1y_cst = new CStransform(canvasL, Cs1yX, Cs1yY);

    if (NScintPlanes == 4) {
        s2x_cst = new CStransform(canvasL, Cs2xX, Cs2xY);
        s2y_cst = new CStransform(canvasL, Cs2yX, Cs2yY);
    }

    s1X = new ScintPlane((char*)"s1x", vars, s1x_cst);
    s1Y = new ScintPlane((char*)"s1y", vars, s1y_cst);

    if (NScintPlanes == 4) {
        s2X = new ScintPlane((char*)"s2x", vars, s2x_cst);
        s2Y = new ScintPlane((char*)"s2y", vars, s2y_cst);
    }


    // In the end we plot a coordinate system

    TArrow *xaxis = new TArrow(0.02, 0.02, 0.12, 0.02, 0.02, "|>");
    xaxis->Draw();
    TArrow *yaxis = new TArrow(0.02, 0.02, 0.02, 0.12, 0.02, "|>");
    yaxis->Draw();

    TLatex *xtext = new TLatex(0.14, 0.02, "x");
    xtext->SetTextSize(0.03);
    xtext->Draw();

    TLatex *ytext = new TLatex(0.02, 0.14, "y");
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
          // *addr = tmp;

          //ver3 ,same as ver2
          //for (int i=0; i<size ;i++)
	  //  tmp[i]=0;
          // *addr = tmp;
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
          // *addr = tmp;

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
    t1->SetBranchAddress( "Ndata.P.dc.1x1.wirenum", &Ndata_H_dc_1u1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.1x2.wirenum", &Ndata_H_dc_1v1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2x2.wirenum", &Ndata_H_dc_2u1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2x1.wirenum", &Ndata_H_dc_2v1_tdchits);

    t1->SetBranchAddress( "Ndata.P.dc.1u1.wirenum", &Ndata_H_dc_1x1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.1v2.wirenum", &Ndata_H_dc_1x2_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2v2.wirenum", &Ndata_H_dc_2x1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2u1.wirenum", &Ndata_H_dc_2x2_tdchits);

    t1->SetBranchAddress( "Ndata.P.dc.1u2.wirenum", &Ndata_H_dc_1y1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.1v1.wirenum", &Ndata_H_dc_1y2_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2v1.wirenum", &Ndata_H_dc_2y1_tdchits);
    t1->SetBranchAddress( "Ndata.P.dc.2u2.wirenum", &Ndata_H_dc_2y2_tdchits);

    /// UV Wire plane hits and hit times
    t1->SetBranchAddress( "P.dc.1x1.wirenum", &H_dc_1u1_tdchits);
    t1->SetBranchAddress( "P.dc.1x2.wirenum", &H_dc_1v1_tdchits);
    t1->SetBranchAddress( "P.dc.2x2.wirenum", &H_dc_2u1_tdchits);
    t1->SetBranchAddress( "P.dc.2x1.wirenum", &H_dc_2v1_tdchits);

    t1->SetBranchAddress( "P.dc.1x1.time", &H_dc_1u1_time);
    t1->SetBranchAddress( "P.dc.1x2.time", &H_dc_1v1_time);
    t1->SetBranchAddress( "P.dc.2x2.time", &H_dc_2u1_time);
    t1->SetBranchAddress( "P.dc.2x1.time", &H_dc_2v1_time);

    /// X Wire plane hits and hit times
    t1->SetBranchAddress( "P.dc.1u1.wirenum", &H_dc_1x1_tdchits);
    t1->SetBranchAddress( "P.dc.1v2.wirenum", &H_dc_1x2_tdchits);
    t1->SetBranchAddress( "P.dc.2v2.wirenum", &H_dc_2x1_tdchits);
    t1->SetBranchAddress( "P.dc.2u1.wirenum", &H_dc_2x2_tdchits);

    t1->SetBranchAddress( "P.dc.1u1.time", &H_dc_1x1_time);
    t1->SetBranchAddress( "P.dc.1v2.time", &H_dc_1x2_time);
    t1->SetBranchAddress( "P.dc.2v2.time", &H_dc_2x1_time);
    t1->SetBranchAddress( "P.dc.2u1.time", &H_dc_2x2_time);

    /// Y Wire plane hits and hit times
    t1->SetBranchAddress( "P.dc.1u2.wirenum", &H_dc_1y1_tdchits);
    t1->SetBranchAddress( "P.dc.1v1.wirenum", &H_dc_1y2_tdchits);
    t1->SetBranchAddress( "P.dc.2v1.wirenum", &H_dc_2y1_tdchits);
    t1->SetBranchAddress( "P.dc.2u2.wirenum", &H_dc_2y2_tdchits);

    t1->SetBranchAddress( "P.dc.1u2.time", &H_dc_1y1_time);
    t1->SetBranchAddress( "P.dc.1v1.time", &H_dc_1y2_time);
    t1->SetBranchAddress( "P.dc.2v1.time", &H_dc_2y1_time);
    t1->SetBranchAddress( "P.dc.2u2.time", &H_dc_2y2_time);

    /// TRACKS
    t1->SetBranchAddress( "Ndata.P.tr.x", &Ndata_H_tr_x);
    t1->SetBranchAddress( "P.tr.x", &H_tr_x);
    t1->SetBranchAddress( "P.tr.y", &H_tr_y);
    t1->SetBranchAddress( "P.tr.ph", &H_tr_ph);
    t1->SetBranchAddress( "P.tr.th", &H_tr_th);
    //t1->SetBranchAddress( "Ndata.P.dc.x_BJ", &Ndata_H_tr_x);
    //t1->SetBranchAddress( "P.dc.x_BJ", &H_tr_x);
    //t1->SetBranchAddress( "P.dc.y_BJ", &H_tr_y);
    //t1->SetBranchAddress( "P.dc.dx_BJ", &H_tr_th);
    //t1->SetBranchAddress( "P.dc.dy_BJ", &H_tr_ph);


    // Scint Planes
    // TDC hits:
    //   X : +L; -R
    //   Y : +B; -T

    // S1X
    t1->SetBranchAddress("Ndata.P.hod.1x.negTdcCounter", &Ndata_H_hod_1x_negtdchits);
    t1->SetBranchAddress("Ndata.P.hod.1x.posTdcCounter", &Ndata_H_hod_1x_postdchits);
    t1->SetBranchAddress("P.hod.1x.negTdcCounter", &H_hod_1x_negtdchits);
    t1->SetBranchAddress("P.hod.1x.posTdcCounter", &H_hod_1x_postdchits);

    // S1Y
    t1->SetBranchAddress( "Ndata.P.hod.1y.negTdcCounter", &Ndata_H_hod_1y_negtdchits);
    t1->SetBranchAddress( "Ndata.P.hod.1y.posTdcCounter", &Ndata_H_hod_1y_postdchits);
    t1->SetBranchAddress( "P.hod.1y.negTdcCounter", &H_hod_1y_negtdchits);
    t1->SetBranchAddress( "P.hod.1y.posTdcCounter", &H_hod_1y_postdchits);

    // S2X
    t1->SetBranchAddress( "Ndata.P.hod.2x.negTdcCounter", &Ndata_H_hod_2x_negtdchits);
    t1->SetBranchAddress( "Ndata.P.hod.2x.posTdcCounter", &Ndata_H_hod_2x_postdchits);
    t1->SetBranchAddress( "P.hod.2x.negTdcCounter", &H_hod_2x_negtdchits);
    t1->SetBranchAddress( "P.hod.2x.posTdcCounter", &H_hod_2x_postdchits);

    // S2Y
    t1->SetBranchAddress( "Ndata.P.hod.2y.negTdcCounter", &Ndata_H_hod_2y_negtdchits);
    t1->SetBranchAddress( "Ndata.P.hod.2y.posTdcCounter", &Ndata_H_hod_2y_postdchits);
    t1->SetBranchAddress( "P.hod.2y.negTdcCounter", &H_hod_2y_negtdchits);
    t1->SetBranchAddress( "P.hod.2y.posTdcCounter", &H_hod_2y_postdchits);


    // Calorimeter planes
    //   PR : +L; -R

    // C1PR
    t1->SetBranchAddress("Ndata.P.cal.pr.negAdcCounter", &Ndata_H_cal_1pr_negAdcCounter);
    t1->SetBranchAddress("Ndata.P.cal.pr.posAdcCounter", &Ndata_H_cal_1pr_posAdcCounter);
    t1->SetBranchAddress("P.cal.pr.negAdcCounter", &H_cal_1pr_negAdcCounter);
    t1->SetBranchAddress("P.cal.pr.posAdcCounter", &H_cal_1pr_posAdcCounter);

    // C2FLY
    t1->SetBranchAddress("Ndata.P.cal.fly.adcCounter", &Ndata_H_cal_fly_adcCounter);
    t1->SetBranchAddress("P.cal.fly.adcCounter", &H_cal_fly_adcCounter);

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


//_________________________ Lets handle 3D view  ______________________________________

    if (fTextButton3dView->IsOn()) {
        //cout<<"Wait! I am working!"<<endl;

        c5->cd();

        if (title != NULL) delete title;
        title = new TLatex(-0.96,-0.96, graph_title);
        title->SetTextSize(0.03);
        title->Draw();

        //***************** First chamber
        GetVariables* HMSvars= new GetVariables("SHMS.txt");
        int u1NW = HMSvars->GetInt("MWDC1.u.NumWires =");
        int v1NW = HMSvars->GetInt("MWDC1.v.NumWires =");
        int yp1NW = HMSvars->GetInt("MWDC1.yp.NumWires =");
        int xp1NW = HMSvars->GetInt("MWDC1.xp.NumWires =");

        detector->MWDC1->clear();

        //detector->MWDC1->WireHit3D("x", 10);  // 1u1
        //detector->MWDC1->WireHit3D("y", 15);  // 1u2
        //detector->MWDC1->WireHit3D("u", u1NW+1-10);  // 1x1
        //detector->MWDC1->WireHit3D("v", v1NW+1-15);  // 1x2
        //detector->MWDC1->WireHit3D("yp", yp1NW+1-10);  // 1v1
        //detector->MWDC1->WireHit3D("xp", xp1NW+1-15);  // 1v2

        for (int i=0; i<Ndata_H_dc_1x1_tdchits; ++i) {  // 1u1
          detector->MWDC1->WireHit3D("x", H_dc_1x1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_1y1_tdchits; ++i) {  // 1u2
          detector->MWDC1->WireHit3D("y", H_dc_1y1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_1u1_tdchits; ++i) {  // 1x1
          detector->MWDC1->WireHit3D("u", u1NW+1-H_dc_1u1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_1v1_tdchits; ++i) {  // 1x2
          detector->MWDC1->WireHit3D("v", v1NW+1-H_dc_1v1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_1y2_tdchits; ++i) {  // 1v1
          detector->MWDC1->WireHit3D("yp", yp1NW+1-H_dc_1y2_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_1x2_tdchits; ++i) {  // 1v2
          detector->MWDC1->WireHit3D("xp", xp1NW+1-H_dc_1x2_tdchits[i]);
        }


        int u2NW = HMSvars->GetInt("MWDC2.u.NumWires =");
        int v2NW = HMSvars->GetInt("MWDC2.v.NumWires =");
        int yp2NW = HMSvars->GetInt("MWDC2.yp.NumWires =");
        int xp2NW = HMSvars->GetInt("MWDC2.xp.NumWires =");

        detector->MWDC2->clear();

        //detector->MWDC2->WireHit3D("x", 10);  // 2v2
        //detector->MWDC2->WireHit3D("y", 15);  // 2v1
        //detector->MWDC2->WireHit3D("u", u2NW+1-10);  // 2x2
        //detector->MWDC2->WireHit3D("v", v2NW+1-15);  // 2x1
        //detector->MWDC2->WireHit3D("yp", yp2NW+1-10);  // 2u2
        //detector->MWDC2->WireHit3D("xp", xp2NW+1-15);  // 2u1

        for (int i=0; i<Ndata_H_dc_2x1_tdchits; ++i) {  // 2v2
          detector->MWDC2->WireHit3D("x", H_dc_2x1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_2y1_tdchits; ++i) {  // 2v1
          detector->MWDC2->WireHit3D("y", H_dc_2y1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_2u1_tdchits; ++i) {  // 2x2
          detector->MWDC2->WireHit3D("u", u2NW+1-H_dc_2u1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_2v1_tdchits; ++i) {  // 2x1
          detector->MWDC2->WireHit3D("v", v2NW+1-H_dc_2v1_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_2x2_tdchits; ++i) {  // 2u2
          detector->MWDC2->WireHit3D("yp", yp2NW+1-H_dc_2x2_tdchits[i]);
        }
        for (int i=0; i<Ndata_H_dc_2y2_tdchits; ++i) {  // 2u1
          detector->MWDC2->WireHit3D("xp", xp2NW+1-H_dc_2y2_tdchits[i]);
        }


        detector->s1xplane->clear();
        detector->s1yplane->clear();
        detector->s2xplane->clear();
        detector->s2yplane->clear();

        //int __nData = 2;
        //double __negData[2] = {1, 3};
        //double __posData[2] = {2, 4};
        //detector->s1xplane->SPHit(__nData, __nData, __posData, __negData);
        //detector->s1yplane->SPHit(__nData, __nData, __posData, __negData);
        //detector->s2xplane->SPHit(__nData, __nData, __posData, __negData);
        //detector->s2yplane->SPHit(__nData, __nData, __posData, __negData);

        detector->s1xplane->SPHit(
          Ndata_H_hod_1x_postdchits, Ndata_H_hod_1x_negtdchits,
          H_hod_1x_postdchits, H_hod_1x_negtdchits
        );
        detector->s1yplane->SPHit(
          Ndata_H_hod_1y_postdchits, Ndata_H_hod_1y_negtdchits,
          H_hod_1y_postdchits, H_hod_1y_negtdchits
        );
        detector->s2xplane->SPHit(
          Ndata_H_hod_2x_postdchits, Ndata_H_hod_2x_negtdchits,
          H_hod_2x_postdchits, H_hod_2x_negtdchits
        );
        detector->s2yplane->SPHit(
          Ndata_H_hod_2y_postdchits, Ndata_H_hod_2y_negtdchits,
          H_hod_2y_postdchits, H_hod_2y_negtdchits
        );

        // Calorimeter : preshower.
        detector->pr[0]->clear();
        detector->pr[1]->clear();

        //int __nData = 2;
        //double __negData[2] = {1, 3};
        //double __posData[2] = {2, 4};
        //detector->pr[0]->SPHit(__nData, __nData, __negData, __negData);
        //detector->pr[1]->SPHit(__nData, __nData, __posData, __posData);

        detector->pr[0]->SPHit(
          Ndata_H_cal_1pr_negAdcCounter, Ndata_H_cal_1pr_negAdcCounter,
          H_cal_1pr_negAdcCounter, H_cal_1pr_negAdcCounter
        );
        detector->pr[1]->SPHit(
          Ndata_H_cal_1pr_posAdcCounter, Ndata_H_cal_1pr_posAdcCounter,
          H_cal_1pr_posAdcCounter, H_cal_1pr_posAdcCounter
        );

        //int __ndata = 6;
        //double __data[6] = {1, 3, 16, 17, 209, 224};

        //cout << endl << "nData : " << Ndata_H_cal_fly_adcCounter << endl;
        //for (int iData=0; iData<Ndata_H_cal_fly_adcCounter; ++ iData) {
        //  int row = ((int)H_cal_fly_adcCounter[iData]-1) % 16 + 1;
        //  int col = ((int)H_cal_fly_adcCounter[iData]-1) / 16 + 1;
        //  cout
        //    << "  " << H_cal_fly_adcCounter[iData]
        //    << " : " << col
        //    << " - " << row
        //    << " :: " << 14-col
        //    << endl;
        //}

        // Calorimeter : shower.
        for (int iCol=0; iCol<14; ++iCol) {
          detector->fly[iCol]->clear();

          int nDataFlyBar = 0;
          double dataFlyBar[1024];
          for (int iData=0; iData<Ndata_H_cal_fly_adcCounter; ++iData) {
            int row = ((int)H_cal_fly_adcCounter[iData]-1) % 16 + 1;
            int col = ((int)H_cal_fly_adcCounter[iData]-1) / 16 + 1;
            if (iCol == 14-col) {
              dataFlyBar[nDataFlyBar] = row;
              ++nDataFlyBar;
            }
          }

          detector->fly[iCol]->SPHit(
            nDataFlyBar, nDataFlyBar,
            dataFlyBar, dataFlyBar
          );
        }

        //detector->pr[1]->SPHit(
        //  Ndata_H_cal_1pr_posAdcCounter, Ndata_H_cal_1pr_posAdcCounter,
        //  H_cal_1pr_posAdcCounter, H_cal_1pr_posAdcCounter
        //);

        // Clear tracks
        detector->ClearTracks();
        cout << endl << endl << "New Tracks!" << endl;
        // Now draw tracks through the detector stack
        if (Ndata_H_tr_x>0 && fTextButtonTrack->IsOn()) {
            for(uint i=0; i < std::min((unsigned int)Ndata_H_tr_x, (unsigned int)detector->TrackList.size()); i++) {
                detector->TrackList[i]->
                Enable(i, H_tr_x[i], H_tr_y[i], H_tr_th[i], H_tr_ph[i]);
            }
        }
        c5->Update();
    }
}

void EVe::doThis()
{
    EventNumber = fNumberEntry1->GetNumber();
    if (EventNumber<TotalNumberOfEvents && EventNumber>0) {
        DoDraw(EventNumber);
    }
}

void EVe::doNext()
{
    EventNumber = fNumberEntry1->GetNumber();
    if (EventNumber<TotalNumberOfEvents) {
        EventNumber++;
        fNumberEntry1->SetNumber(EventNumber);
        DoDraw(EventNumber);
    }
}
void EVe::doPrevious()
{
    EventNumber = fNumberEntry1->GetNumber();
    if (EventNumber>0) {
        EventNumber--;
        fNumberEntry1->SetNumber(EventNumber);
        DoDraw(EventNumber);
    }
}


void EVe::doNextGood()
{
    EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
    while (EventNumber<TotalNumberOfEvents) {
        EventNumber++; // rise the number for one;
        t1->GetEntry(EventNumber);
        fNumberEntry1->SetNumber(EventNumber); // write that to the display
        if (Ndata_H_tr_x > 0) { // check if event is good (if there is at least 1 good track).
            DoDraw(EventNumber);
            break;
        }
    }
}

void EVe::doPreviousGood()
{
    EventNumber = fNumberEntry1->GetNumber();  // Start at this ev. number
    while (EventNumber>0) {
        EventNumber--; // lower the number for one;
        t1->GetEntry(EventNumber);
        fNumberEntry1->SetNumber(EventNumber); // write that to the display
        if (Ndata_H_tr_x > 0) { // check if event is good (if there is at least 1 good track).
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
    const char *myfiletypes[] = {
        "All files","*",
        "PostScript files","*.ps",
        "Encapsulated PostScript files","*.eps",
        "GIF files","*.gif",
        "JPG files","*.jpg",
        0,               0
    };
    fi.fFileTypes = myfiletypes;
    fi.fIniDir    = StrDup(dir.Data());

    new TGFileDialog(gClient->GetRoot(), fMainFrame, kFDSave, &fi);
    if(fi.fFilename!=NULL) fCanvas->Print(fi.fFilename);
}
