///////////////////////////////////////
/*  ScintPlane.cxx  6/17/14

    Ben Davis-Purcell

    Class that creates an array of ScintillationPaddles to create a plane

*/
///////////////////////////////////////

#include "ScintPlane.h"
#include "TMath.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

ScintPlane::ScintPlane(char* SplaneName, CStransform *trans)
{

    splaneName = SplaneName;
    // Setup size and number of paddles, converting plenth & pheight [m] into pixels
    cst = trans;
    GetVariables *hms = new GetVariables("HMS.txt");

    // Get Values used in construct n paddles for a single ScintPlane
    int numPMT = hms->GetInt("Number of paddle PMTs =");

    N = hms-> GetInt(Form("%s.PN =",splaneName));

    paddle_length = hms ->GetDouble(Form("%s.PaddleLength =",splaneName))/100.0;
    paddle_height = hms ->GetDouble(Form("%s.PaddleHeight =",splaneName))/100.0;

    double PMTlength = hms -> GetDouble ("PMTlength =");
    PMTl = cst->transLtoCL(PMTlength);

    angle = hms -> GetDouble(Form("%s.angle =",splaneName));
    cerr << "Angle for " << splaneName << " is " << angle;
    //below calculate and draw all paddles for a single scintplane
    // sx0,sy0 is the coordinate of the sicntplane in canvas
    sx0 = cst->transXtoCX(0.0);                                    ;
    sy0 = cst->transYtoCY(0.0);                                                        ;

    // cx0,cy0 is the coordinate of the lower left corner of the 1st paddle
    double cx0= - cst->transLtoCL(paddle_length/2.0);
    double cy0=  cst->transLtoCL(paddle_height*(N)/2.0 - paddle_height);

    double CL = cst->transLtoCL(paddle_length);
    double CH = cst->transLtoCL(paddle_height);


    //Consider sxpaddle length is different from sypaddle length
    sa = CL;
    sb = CH;

    for (int i=0; i<N ; i++) {
        paddle[i]=new ScintillatorPaddle(i, sx0, sy0, sa ,sb ,cx0,cy0-i*CH, numPMT, PMTl,angle);
    }
    title = new TLatex(sx0-0.1*sa, sy0-9.5*sb, splaneName);
    title->SetTextSize(0.03);
    title->Draw();

    for(int i = 0; i<MAX_TRACK_NUM; i++)
        track_circ[i] = new TEllipse(0,0,0);

    cout<<"Scintillation Plane is created!"<<endl;
}


ScintPlane::~ScintPlane()
{
    // Destructor
}

/// NEW hit method used for tdchits. Needs to be implemented in ScintPlane3D
void ScintPlane::paddleLeftHit(int padn)
{
    if(padn<0) cerr << "Negative paddle index" << padn <<" \n";
    else
        paddle[padn]->HitLeft();

}


void ScintPlane::paddleRightHit(int pad)
{
    if(pad<0) cerr << "Negative paddle index" << pad <<" \n";
    else
        paddle[pad]->HitRight();

}

void ScintPlane::paddleBothHit(int padnum)
{
    if(padnum<0) cerr << "Negative paddle index" << padnum <<" \n";
    else
        paddle[padnum]->HitBoth();
}


void ScintPlane::clear()
{
    for(int i = 0; i<N; i++) {
        paddle[i]->clear();
    }

    for(int i=0; i<MAX_TRACK_NUM; i++) {
        track_circ[i]->SetX1(0.0);
        track_circ[i]->SetY1(0.0);
        track_circ[i]->SetR1(0.0);
        track_circ[i]->SetR2(0.0);
        track_circ[i]->SetFillColor(kWhite);
        track_circ[i]->Draw();

        //delete track_index[i];
    }
}

/// Track function draws track circle to indicate particle position at scintplane
void ScintPlane::Track(double x, double y, int i)
{
    double CX =  cst->transXtoCX(x);
    double CY =  cst->transYtoCY(y);

    track_circ[i]->SetX1(CX);
    track_circ[i]->SetY1(CY);
    track_circ[i]->SetR1(0.008);
    track_circ[i]->SetR2(0.008);
    track_circ[i]->SetLineColor(1+i);
    track_circ[i]->SetLineWidth(2);
    track_circ[i]->Draw();

}

void ScintPlane :: SPHit2D(int NumL, int NumR, double poshit[], double neghit[])
{
    GetVariables *hms = new GetVariables("HMS.txt");
    // PN is number of paddles for a single scintplane
    int PN = hms->GetInt(Form("%s.PN =",splaneName));

    double matchR[PN];
    double matchL[PN];

    for (int q = 0; q<NumR; q++) {
        matchR[q] = neghit[q];
        paddleRightHit(neghit[q]-1);
    }

    for (int q=0; q<NumL; q++) {
        matchL[q]= poshit[q];
        paddleLeftHit(poshit[q]-1);
    }

    for (int i=0; i<PN; i++) {
        for (int j=0; j<PN; j++) {
            if ( (matchR[i]==matchL[j])  && (matchR[i]!=0) ) {

                paddleBothHit(matchR[i]-1);
            } else if ( (matchL[i]==matchR[j])  && (matchL[i]!=0) ) {
                paddleBothHit(matchL[i]-1);
            }
        }
        matchR[i]=0;
        matchL[i]=0;
    }
}
