#define _GLIBCXX_USE_CXX11_ABI 0
#ifndef GLOBALPLACER_H
#define GLOBALPLACER_H

#include "Placement.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>

class GlobalPlacer 
{
public:
    GlobalPlacer(Placement &placement):_placement(placement){};
    void place();
    void plotPlacementResult( const string outfilename, bool isPrompt = false );
    void initialPlace(vector<double> &x,double chipwidth, double chipheight);
    double checkBoundary( double pos, double upperbound, double lowerbound, double moduleLength);
    double randPos( double lowerbound, double chiplength, double modulelength );
private:
    Placement& _placement;
    void plotBoxPLT( ofstream& stream, double x1, double y1, double x2, double y2 );
};

#endif // GLOBALPLACER_H
