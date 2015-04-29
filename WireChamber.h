#ifndef ROOT_WireChamber
#define ROOT_WireChamber

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "CStransform.h"
#include "WirePlane2D.h"

#include <map>
#include <string>
#include <cstring>
#include <vector>

#define MAX_TRACK_NUM 77

using namespace std;

class WireChamber
{
public:
    WireChamber (char* chamberName, CStransform *trans);

    virtual ~WireChamber();
    void ChamberHit();
    void WireHit(string planeName, int wireNum);
    void clear();
    void Track(double x, double y, int i);

protected:
    TPolyLine *box1;
    TPolyLine *box2;

    TLatex *title;
    CStransform *cst;
    double a,b;

    std::map <string, WirePlane2D> wirePlanes;

    TEllipse *track_circ[MAX_TRACK_NUM];
};

#endif
