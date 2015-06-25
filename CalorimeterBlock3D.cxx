///////////////////////////////////////
/*  CalorimeterBlock3D.cxx  7/17/14

    Christian Davison & Mitchell Kerver

    Class that creates a singe calorimeter block (3D) and attaches either one or two PMTs

*/      
///////////////////////////////////////


#include "ScintillatorPaddle3D.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include "CalorimeterBlock3D.h"

using namespace std;

CalorimeterBlock3D::CalorimeterBlock3D(char* PlaneName, int index, int n,
        double length, double height, double thickness,
        TGeoVolume *paddle, int numPMT, int PMTdirection)
{



//bool  odd=false;
//bool  even=false;
    double r_PMT = 2.0;// Change to use HMS.txt
    double l_PMT = 8.0;// Change to use HMS.txt
    double L=length;
    double H=height;
    double T=thickness;

    //(x,y,z) is the coordinate of the center of this single paddle

    double x= 0;
    double y= -((-1)* H*((double)n-1)/2.0+(double)index*H);
    double z= 0;

    //Drawing a single Scintillator Paddle in z direction till the end of this constructor
    //Draw scintillator Paddle and side skirt paddle first
    TGeoBBox *scintb = new TGeoBBox(Form("%s.%d.ScintPaddle",PlaneName,index),0.9*T/2.0, 0.9*H/2.0, L/2.0);
    TGeoTranslation *transcint = new TGeoTranslation(x,y,z);
    scint = new TGeoVolume (Form("%s.%d.Paddle",PlaneName,index), scintb);
    scint ->SetLineColor(kBlack);
    paddle -> AddNode(scint,1,transcint);

    //Draw PMT on each side
std::string stringPlaneName(PlaneName);
int j=0;


//FIXMEEEE pass in new paramer indicating rotation of entire array
//See CalorimeterPlane3D with same fix
//temp use if else and calorimter names
//This if statement puts all of the pmts on the same side (left) of the calorimeter plane

if(stringPlaneName.compare("Cal4x")==0){

TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",1,PlaneName,index),
                               0.0, r_PMT , l_PMT);
                pmt1= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,1),pmt);
                pmt1->SetLineColor(kBlack);
                TGeoTranslation *pmttrans1= new TGeoTranslation("pmttrans1",x,y,z+(0.5*L+l_PMT));
                paddle->AddNode(pmt1,1,pmttrans1);
                odd=false;
                even=true;
             //   break;



}


//The else statement creates one PMT on either the left or right side depending on the index of the for loop of calorimeter planes in the Detector3D class

else{
if(numPMT==1){

    for(int i=1;i<=10;i=i+2)    {
        
         j=i+1;   


//If the index of the calorimeter plane is an even number(stored in the variable PMTdirection), then the left pmt gets created
	if(PMTdirection%2==0)
	{
	
	     TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",1,PlaneName,index),
                               0.0, r_PMT , l_PMT);
	    	pmt1= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,1),pmt);
    		pmt1->SetLineColor(kBlack);
    		TGeoTranslation *pmttrans1= new TGeoTranslation("pmttrans1",x,y,z+(0.5*L+l_PMT));
    		paddle->AddNode(pmt1,1,pmttrans1);
	    //If the index of the calorimeter plane is an even number then even gets set to true
		odd=false;
		even=true;
                break;
	}
//If the index of the calorimeter plane is an odd number(stored in the variable PMTdirection), then the right pmt gets created
	else if(PMTdirection%2!=0)
	{
		TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",2,PlaneName,index),
                                     0.0, r_PMT , l_PMT);
        pmt2= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,2),pmt);
        pmt2->SetLineColor(kBlack);
        TGeoTranslation *pmttrans2 = new TGeoTranslation("pmttrans2",x,y,z-(0.5*L+l_PMT));
        paddle->AddNode(pmt2,1,pmttrans2);     
    //If the index of the calorimeter plane is an odd number then odd gets set to true
	odd=true;
	even=false;
        break; 
	}
    }
}
}

//If the number of PMTs is 2, then both of the PMTs get created
	if(numPMT==2)
	{
	    TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",1,PlaneName,index),
                                 0.0, r_PMT , l_PMT);
   		 pmt1= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,1),pmt);
    		pmt1->SetLineColor(kBlack);
    		TGeoTranslation *pmttrans1= new TGeoTranslation("pmttrans1",x,y,z+(0.5*L+l_PMT));
   	 	paddle->AddNode(pmt1,1,pmttrans1);

    //Draw lower PMT if n_PMT=2
//        TGeoTube *pmt = new TGeoTube(Form("PMT%d_Tube_%s.%d",2,PlaneName,index),
//                                     0.0, r_PMT , l_PMT);
        pmt2= new TGeoVolume(Form("%s.%d.PMT%d",PlaneName,index,2),pmt);
        pmt2->SetLineColor(kBlack);
        TGeoTranslation *pmttrans2 = new TGeoTranslation("pmttrans2",x,y,z-(0.5*L+l_PMT));
        paddle->AddNode(pmt2,1,pmttrans2);
    //Since both PMTs need to be created, both even and odd must be set to true
	odd=true;
	even=true;
    }
}


CalorimeterBlock3D::~CalorimeterBlock3D()
{
    //Destructor
}

//Below using the same hit method used in planar view ScintPlane and Paddle Classes

void CalorimeterBlock3D::HitPaddle()
{
    scint ->SetLineColor(kGreen);
}

void CalorimeterBlock3D::HitL()
{
//The left PMT gets created only if numPMT=2 or the index of the calorimeter plane is even

    if(even==true)
	{
    	    pmt1->SetLineColor(kGreen);
	}
}
 
void CalorimeterBlock3D::HitR()
{
    //FIXME: what about single PMT case (no pmt2)?
//The right PMT gets created only if numPMT=2 or the index of the calorimeter plane is odd

    if(odd==true)
	{
   	    pmt2->SetLineColor(kGreen);
	}
}

void CalorimeterBlock3D::clear()
{
    //FIXME: what about single PMT case (no pmt2)?
if(even==true){
    pmt1->SetLineColor(kBlack);
}  
 if(odd==true){
     pmt2->SetLineColor(kBlack);
}
    scint ->SetLineColor(kBlack);
}
