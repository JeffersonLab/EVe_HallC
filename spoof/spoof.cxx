#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"


void spoof() {
  TFile fo("test.root", "recreate");
  TTree t1("T", "Spoofed data.");

  Double_t tr_x, tr_y, tr_ph, tr_th;
  Int_t ev, N;

  t1.Branch("Ndata.P.tr.x", &N, "Ndata.P.tr.x/I");
  t1.Branch("P.tr.x", &tr_x, "P.tr.x/D");
  t1.Branch("Ndata.P.tr.y", &N, "Ndata.P.tr.y/I");
  t1.Branch("P.tr.y", &tr_y, "P.tr.y/D");
  t1.Branch("Ndata.P.tr.th", &N, "Ndata.P.tr.th/I");
  t1.Branch("P.tr.th", &tr_th, "P.tr.th/D");
  t1.Branch("Ndata.P.tr.ph", &N, "Ndata.P.tr.ph/I");
  t1.Branch("P.tr.ph", &tr_ph, "P.tr.ph/D");
  t1.Branch("Ndata.ev", &N, "Ndata.ev/I");
  t1.Branch("ev", &ev, "ev/I");

  N = 1;

  // Event 0:
  ev = 0;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 1:
  ev = 0;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 2:
  ++ev;
  tr_x = -40.0;
  tr_y = -40.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 3:
  ++ev;
  tr_x = -40.0;
  tr_y = 40.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 4:
  ++ev;
  tr_x = 40.0;
  tr_y = 40.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 5:
  ++ev;
  tr_x = 40.0;
  tr_y = -40.0;
  tr_th = 0.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 6:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = -1.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 7:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 0.0;
  tr_ph = 1.0;
  t1.Fill();

  // Event 8:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 1.0;
  tr_ph = 0.0;
  t1.Fill();

  // Event 9:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 0.0;
  tr_ph = -1.0;
  t1.Fill();

  // Event 10:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = 1.0;
  tr_ph = 1.0;
  t1.Fill();

  // Event 11:
  ++ev;
  tr_x = 0.0;
  tr_y = 0.0;
  tr_th = -1.0;
  tr_ph = -1.0;
  t1.Fill();

  // Event 12:
  ++ev;
  tr_x = 20.0;
  tr_y = 20.0;
  tr_th = 0.5;
  tr_ph = 0.5;
  t1.Fill();

  // Event 13:
  ++ev;
  tr_x = -20.0;
  tr_y = -20.0;
  tr_th = -0.5;
  tr_ph = -0.5;
  t1.Fill();

  // Event 14:
  ++ev;
  tr_x = 20.0;
  tr_y = -20.0;
  tr_th = -0.5;
  tr_ph = 0.5;
  t1.Fill();

  // Event 15:
  ++ev;
  tr_x = -20.0;
  tr_y = 20.0;
  tr_th = 0.5;
  tr_ph = -0.5;
  t1.Fill();



  t1.Write();
}
