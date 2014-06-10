//************************************************************************* 
//  BigBiteOgrodje3D.cxx  - 4/14/2008
// 
//  by miham
// 
//  This class creates the 3D view of Big Bite Magnet
// 
//
//************************************************************************* 

#include "BigBiteOgrodje3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>


using namespace std;

BigBiteOgrodje3D::BigBiteOgrodje3D( double x, double y, double z, TGeoVolume *paddle)
{
   
  // spodnja platforma

  TGeoTrd2 *trd1 = new TGeoTrd2(30,30,10,10,30);
  TGeoVolume *blok1 = new TGeoVolume("blok1",trd1);
  blok1->SetLineColor(kBlack);
  TGeoRotation r1;
  r1.SetAngles(90,0,90,90,0,0);
  TGeoTranslation t1(x, y+30.0, z);
  TGeoCombiTrans *comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok1,1,comb);

  t1.SetTranslation(x, y-30.0, z);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok1,2,comb);

  TGeoTrd2 *trd2 = new TGeoTrd2(30,20,10,10,10);
  TGeoVolume *blok2 = new TGeoVolume("blok2",trd2);
  blok2->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90,0,0);
  t1.SetTranslation(x+0, y+30.0, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok2,1,comb);

  t1.SetTranslation(x+0, y-30.0, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok2,2,comb);


  TGeoTrd2 *trd3 = new TGeoTrd2(30,30,10,5,5);
  TGeoVolume *blok3 = new TGeoVolume("blok3",trd3);
  blok3->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,-90,180,0);
  t1.SetTranslation(x+0, y+30.0, z-35);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok3,1,comb);

  t1.SetTranslation(x+0, y-30.0, z-35);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok3,2,comb);

  TGeoTrd2 *trd4 = new TGeoTrd2(30,30,25,20,5);
  TGeoVolume *blok4 = new TGeoVolume("blok4",trd4);
  blok4->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90,0,0);
  t1.SetTranslation(x+0, y+0.0, z-35);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok4,1,comb);


  TGeoTrd2 *trd5 = new TGeoTrd2(20,20,10,10,10);
  TGeoVolume *blok5 = new TGeoVolume("blok5",trd5);
  blok5->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-10, y+30.0, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok5,1,comb);

  t1.SetTranslation(x-10, y-30.0, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok5,2,comb);


  TGeoTrd2 *trd6 = new TGeoTrd2(25,25,10,2,10);
  TGeoVolume *blok6 = new TGeoVolume("blok6",trd6);
  blok6->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+30.0, z+60);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok6,1,comb);

  t1.SetTranslation(x-5, y-30.0, z+60);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok6,2,comb);

  TGeoTrd2 *trd7 = new TGeoTrd2(25,25,5,5,10);
  TGeoVolume *blok7 = new TGeoVolume("blok7",trd7);
  blok7->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+30.0 - 5, z+60);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok7,1,comb);

  t1.SetTranslation(x-5, y-30.0+5 , z+60);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok7,2,comb);


  TGeoTrd2 *trd8 = new TGeoTrd2(25,25,5,10,10);
  TGeoVolume *blok8 = new TGeoVolume("blok8",trd8);
  blok8->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+30.0-3, z+80);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok8,1,comb);

  t1.SetTranslation(x-5, y-30.0+3 , z+80);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok8,2,comb);

  TGeoTrd2 *trd9 = new TGeoTrd2(25,25,5,5,10);
  TGeoVolume *blok9 = new TGeoVolume("blok9",trd9);
  blok9->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+30.0-5, z+80);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok9,1,comb);

  t1.SetTranslation(x-5, y-30.0+5 , z+80);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok9,2,comb);

  TGeoTrd2 *trd10 = new TGeoTrd2(25,25,10,10,10);
  TGeoVolume *blok10 = new TGeoVolume("blok10",trd10);
  blok10->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+30.0-3, z+100);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok10,1,comb);

  t1.SetTranslation(x-5, y-30.0+3 , z+100);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok10,2,comb);

  TGeoTrd2 *trd11 = new TGeoTrd2(25,25,37,25,7);
  TGeoVolume *blok11 = new TGeoVolume("blok11",trd11);
  blok11->SetLineColor(kBlack);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-5, y+0, z+117);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok11,1,comb);


  TGeoTrd2 *trd12 = new TGeoTrd2(35,35,8,8,60);
  TGeoVolume *blok12 = new TGeoVolume("blok12",trd12);
  blok12->SetLineColor(kMagenta);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x-10, y+30.0-15, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok12,1,comb);

  t1.SetTranslation(x-10, y-30.0+15 , z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok12,2,comb);

  TGeoTrd2 *trd13 = new TGeoTrd2(35,15,8,8,60);
  TGeoVolume *blok13 = new TGeoVolume("blok13",trd13);
  blok13->SetLineColor(kMagenta);
  r1.SetAngles(90,0,90,90, 0,0);
  t1.SetTranslation(x+15, y+30.0-15, z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok13,1,comb);

  t1.SetTranslation(x+15, y-30.0+15 , z+40);
  comb = new TGeoCombiTrans(t1, r1); 
  paddle->AddNode(blok13,2,comb);

   cout<<"BigBite Magnet is Created!"<<endl;
}

BigBiteOgrodje3D::~BigBiteOgrodje3D()
{

}


