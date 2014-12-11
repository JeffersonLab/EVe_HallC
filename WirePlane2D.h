#ifndef ROOT_WirePlane2D
#define ROOT_WirePlane2D

#include "TCanvas.h"
#include "TPolyLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "CStransform.h"
#include "EVe_DB.h"
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class WirePlane2D{
public:
    WirePlane2D(string planeName, double Height, double Width, double wireAngle, int nWires, int wirecolor, CStransform *trans, double offset, int index);
    virtual ~WirePlane2D();
    void WireHit(int Num);
    void clear();

protected:
    int WireNum;
    int color;
    vector<TLine*> wires;

    CStransform *cst;
    TEllipse *circ;
};

#endif
