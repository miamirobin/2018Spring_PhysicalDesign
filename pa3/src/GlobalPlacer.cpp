#include "GlobalPlacer.h"
#include "ExampleFunction.h"
#include "NumericalOptimizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>


// initial place
void GlobalPlacer::initialPlace(vector<double> &x,double chipwidth, double chipheight){
	vector<double> temp;
        temp.resize(x.size(),0);
        double Hpwl;
        unsigned p,q;
        Module module;
    	for (int it=0;it<10;it++){
            for (unsigned i = 0; i < _placement.numModules(); ++i){
                p=2 * i;
                q=p+1;
                module=_placement.module(i);
		if(!module.isFixed()) { 
			
			
			x[p] = randPos( _placement.boundryLeft(), chipwidth, module.width() );
                        x[q] = randPos( _placement.boundryBottom(), chipheight, module.height() );
			
			module.setPosition(x[p], x[q]);
                }
	     }
             if (it==0){
                     return;
                     
                     Hpwl=_placement.computeHpwl();
                     for (int n=0;n<x.size();n++){

                           temp[n]=x[n];
                     }

             }
             else {
                  if (_placement.computeHpwl() < Hpwl){
                       Hpwl=_placement.computeHpwl();
                       for (int n=0;n<x.size();n++){

                           temp[n]=x[n];
                       } 

                  }

             }    


             cout<<"HPWL: "<<Hpwl<<endl;

        }
        for (unsigned i = 0; i < _placement.numModules(); ++i){
                p=2 * i;
                q=p+1;
                module=_placement.module(i);
		if(!module.isFixed()) { 
			
			
			x[p] = temp[p];
                        x[q] = temp[q];
			
			module.setPosition(x[p], x[q]);
                }
	}
        


}



double GlobalPlacer::randPos( double lowerbound, double chiplength, double modulelength ){


      return   lowerbound+( rand() % (int)(chiplength - modulelength));

}



double GlobalPlacer::checkBoundary( double pos, double upperbound, double lowerbound, double moduleLength){

    
			if( moduleLength > upperbound-pos){ 
		                return	upperbound - moduleLength;
			}
			else if(  moduleLength >pos- lowerbound){
				return	lowerbound;
			}
                        else {

                               return pos;
                        }



}




void GlobalPlacer::place()
{
	///////////////////////////////////////////////////////////////////
	// The following example is only for analytical methods.
	// if you use other methods, you can skip and delete it directly.
	//////////////////////////////////////////////////////////////////
  //eturn;
   
        ExampleFunction ef(_placement); // require to define the object function and gradient function
        vector<double> x;   // solution vector, size: num_blocks*2 
                            // each 2 variables represent the X and Y dimensions of a block
        int  iteration;
        double chipwidth = _placement.boundryRight() - _placement.boundryLeft();
	double chipheight = _placement.boundryTop() - _placement.boundryBottom();
        x.resize(ef.dimension(),0); // solution vector, size: num_blocks*2 
        
        initialPlace(x,chipwidth,chipheight); // initialize the solution vector
	NumericalOptimizer no(ef);// require to define the object function and gradient function

        
        
        unsigned p,q;
        ef.lamda=0;
        iteration =0;
      
        while (true){
               
		
		no.setX(x);//set initial solution
		
                no.setNumIteration(150);  // user-specified parameter

		no.setStepSizeBound(max(chipheight,chipwidth)/1.0*7);// user-specified parameter
		no.solve(); // Conjugate Gradient solver
		
		for(int i = 0; i < _placement.numModules(); ++i){

                        p=2 * i;
                        q=p + 1;
			

			x[p] = checkBoundary( no.x(p), _placement.boundryRight(), _placement.boundryLeft(), _placement.module(i).width() );
			x[q] = checkBoundary( no.x(q), _placement.boundryTop(), _placement.boundryBottom(), _placement.module(i).height() ); 
                        _placement.module(i).setPosition(x[p], x[q]);


		}
	        iteration++;
                if (iteration>2){

                   return;
                }
	        ef.lamda += 2500;
          
	
	
	}



	
         cout << "Current solution:" << endl;
         for (unsigned i = 0; i < no.dimension(); i++) {
            cout << "x[" << i << "] = " << no.x(i) << endl;
         }
         cout << "Objective: " << no.objective() << endl;
         cout << "Current solution:" << endl;
         for (unsigned i = 0; i < no.dimension(); i++) {
            cout << "x[" << i << "] = " << no.x(i) << endl;
         }
         cout << "Objective: " << no.objective() << endl;

}


void GlobalPlacer::plotPlacementResult( const string outfilename, bool isPrompt )
{
    ofstream outfile( outfilename.c_str() , ios::out );
    outfile << " " << endl;
    outfile << "set title \"wirelength = " << _placement.computeHpwl() << "\"" << endl;
    outfile << "set size ratio 1" << endl;
    outfile << "set nokey" << endl << endl;
    outfile << "plot[:][:] '-' w l lt 3 lw 2, '-' w l lt 1" << endl << endl;
    outfile << "# bounding box" << endl;
    plotBoxPLT( outfile, _placement.boundryLeft(), _placement.boundryBottom(), _placement.boundryRight(), _placement.boundryTop() );
    outfile << "EOF" << endl;
    outfile << "# modules" << endl << "0.00, 0.00" << endl << endl;
    for( size_t i = 0; i < _placement.numModules(); ++i ){
        Module &module = _placement.module(i);
        plotBoxPLT( outfile, module.x(), module.y(), module.x() + module.width(), module.y() + module.height() );
    }
    outfile << "EOF" << endl;
    outfile << "pause -1 'Press any key to close.'" << endl;
    outfile.close();

    if( isPrompt ){
        char cmd[ 200 ];
        sprintf( cmd, "gnuplot %s", outfilename.c_str() );
        if( !system( cmd ) ) { cout << "Fail to execute: \"" << cmd << "\"." << endl; }
    }
}

void GlobalPlacer::plotBoxPLT( ofstream& stream, double x1, double y1, double x2, double y2 )
{
    stream << x1 << ", " << y1 << endl << x2 << ", " << y1 << endl
           << x2 << ", " << y2 << endl << x1 << ", " << y2 << endl
           << x1 << ", " << y1 << endl << endl;
}
