#define _GLIBCXX_USE_CXX11_ABI 0

#ifndef EXAMPLEFUNCTION_H
#define EXAMPLEFUNCTION_H
#include "Placement.h"
#include "NumericalOptimizerInterface.h"
#include <vector>

class ExampleFunction : public NumericalOptimizerInterface
{

public:
    ExampleFunction(Placement &placement) 
	:_placement(placement),
	 ModuleDensity (0), 
         BinCount(15), 
         lamda(0),
         BinCountSquare (225)
         { 
              initial();
         }
	
    
    void initial();
    void computeExp(const vector<double> &x);
    void initialFG(double &f, vector<double> &g,bool);
    double computeLSE_F(unsigned i);
    void computeLSE_G(vector<double> &g,unsigned i);
    void computeLSE(double &f, vector<double> &g,bool); 
    void evaluateFG(const vector<double> &x, double &f, vector<double> &g);
    void evaluateF(const vector<double> &x, double &f);
    void computeDensity(const vector<double> &x, double &f, vector<double> &g,bool No_g);
    double smoothDensity(unsigned a,unsigned b,bool No_g,const vector<double> &x);
    void smoothDensity_G(unsigned i);
    double computeOverlap(unsigned i);
    void setParameter(unsigned a,unsigned b,unsigned i,const vector<double> &x);
    

    unsigned dimension();
    int  lamda;
	

private:
    Placement*  pPlacement;
    Placement&  _placement;
    int         num;
    double      f0; 
    double      f1;
    double      f2;
    double      f3;
    double      Woverlap;
    double 	Hoverlap;
    double 	cofW1;
    double 	cofW2;
    double 	cofH1;
    double 	cofH2;
    double 	dX;
    double 	dY;
    double 	absdX;
    double 	absdY;
    double 	ratio;
    double 	BinCount;
    double 	BinCountSquare;
    double 	gamma;
    double 	ChipWidth;
    double 	ChipHeight;
    double 	WBin;
    double 	HBin;
    double 	ModuleDensity;
	
    vector<vector<double> > InnerExp;
    vector<double> gt;
    vector<double> ChipDensity;



};
#endif // EXAMPLEFUNCTION_H
