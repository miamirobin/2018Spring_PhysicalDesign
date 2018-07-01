#include "ExampleFunction.h"
#include <cmath>
#include <cstdlib>
#include <vector>
// minimize 3*x^2 + 2*x*y + 2*y^2 + 7




void ExampleFunction::initial(){
       
        num=_placement.numModules();
        BinCount=sqrt(BinCountSquare);
	ChipWidth =_placement.boundryRight() - _placement.boundryLeft();
	ChipHeight = _placement.boundryTop() - _placement.boundryBottom();
        double ChipArea=(_placement.boundryRight() - _placement.boundryLeft()) * 1.0*( _placement.boundryTop() - _placement.boundryBottom());
	gamma = (ChipHeight+ChipWidth)/2.0 / 600;
	
	WBin = ChipWidth*1.0 / BinCount;
	HBin = ChipHeight*1.0 / BinCount;
	
	
	InnerExp.resize( num);
	ChipDensity.resize(BinCountSquare);
        double TotalArea=0.0;
        for(unsigned i = 0; i < num; ++i){
		TotalArea += _placement.module(i).area();
	}
	ModuleDensity =TotalArea/ChipArea;


}


void ExampleFunction::computeExp(const vector<double> &x){
    //compute exp(X_k/gamma)
        double posX,posY;
        unsigned p,q;
	for(int i = 0; i < num; ++i){
                p=2*i;
                q=2*i+1;
		if (!_placement.module(i).isFixed()){	
                        InnerExp[i].clear();
			InnerExp[i].push_back(exp(x[p] / gamma));
			InnerExp[i].push_back(exp(x[p] / gamma*(-1)));
			InnerExp[i].push_back(exp(x[q] / gamma));
			InnerExp[i].push_back(exp(x[q] / gamma*(-1)));
                     
                     
		}
                else{
                        posX=_placement.module(i).centerX();
                        posY=_placement.module(i).centerY();

                        InnerExp[i].clear();
                        InnerExp[i].push_back(exp( posX / gamma));
			InnerExp[i].push_back(exp( posX / gamma*(-1)));
			InnerExp[i].push_back(exp( posY / gamma));
			InnerExp[i].push_back(exp( posY / gamma*(-1)));
                    
                       
		}
	}




}


void ExampleFunction::initialFG(double &f, vector<double> &g,bool No_g){
   //initial f & g
	f = 0;
        if (!No_g){
            gt.resize( 2*num);
	    for(int i = 0; i < g.size(); ++i){
		g[i] = 0;
                gt[i] = 0;
	    }
        }
        for(int i = 0; i < BinCountSquare; ++i){
		ChipDensity[i] = 0;
	}


}

double ExampleFunction::computeLSE_F(unsigned i){
         //compute each Σexp(X_k/ gamma) 
		for(unsigned j = 0; j < _placement.net(i).numPins(); ++j){
                        
                        unsigned ID =_placement.net(i).pin(j).moduleId();
                       
			f0 += InnerExp[ID][0];
			f1 += InnerExp[ID][1];
			f2 += InnerExp[ID][2];
			f3 += InnerExp[ID][3];
		}
        
		//compute all four Σexp
		return log(f0) + log(f1) + log(f2) + log(f3);



}

void ExampleFunction::computeLSE_G(vector<double> &g,unsigned i){
       unsigned p,q;
       //compute gradient of function
		for(unsigned j = 0; j < _placement.net(i).numPins(); j++){
			
			unsigned ID =_placement.net(i).pin(j).moduleId();
			
                        p=2*ID;
                        q=p+1;
			if(!_placement.module(i).isFixed()){
				g[p] += InnerExp[ID][0] / gamma / f0;
				g[p] += InnerExp[ID][1] / gamma / f1*(-1);
				g[q] += InnerExp[ID][2]/ gamma / f2;
				g[q] += InnerExp[ID][3] / gamma /f3*(-1);
			}
                        else{
				
                                g[p] =0;
                                g[q] = 0;
			}
		}


}


void ExampleFunction::computeLSE(double &f, vector<double> &g, bool No_g){

     //compute LSE and its gradient
	for(unsigned i = 0; i <  _placement.numNets(); ++i){
		f0 =0;
                f1 =0;
                f2 =0;
                f3 =0;
       
		
                f+=computeLSE_F(i);
                if (!No_g){
			computeLSE_G(g,i);
                }
		
        
	}


}


void ExampleFunction::setParameter(unsigned wc,unsigned hc,unsigned i,const vector<double> &x){

             
					dX = x[2*i]  - _placement.boundryLeft()- (wc+0.52)*WBin;					
					dY = x[2*i+1]  - _placement.boundryBottom()- (hc+0.52)*HBin;
                                        absdX = abs(dX);
					absdY = abs(dY);
                                        double width=_placement.module(i).width();
                                        double height=_placement.module(i).height();
                                        cofW1 = 1 / (width+WBin   ) / (2 * WBin + width)*4;
					cofW2 = 1 / (WBin+0)/  (2 * WBin +  width)*4;
					cofH1 = 1 / (height+HBin) / (2 * HBin +  height)*4;
					cofH2 = 1 / (HBin+0) / (2 * HBin +  height)*4;                         
					
			
					
					


}




void ExampleFunction::smoothDensity_G(unsigned i){
                                       double width=_placement.module(i).width();
                                       double height=_placement.module(i).height();
                                       unsigned p=2*i;
                                       unsigned q=p+1;
                                       if (absdX > (WBin +  width / 2.0)){
                                             gt[p] = 0;

                                        }
                                        else {
                                             if (absdX <= (WBin+  width) / 2.0){
						 gt[p] = ratio * ((-2.0) * cofW1 * dX) * Hoverlap;
					     }
                                             else {
                                                double temp =0;
                                                if (dX<=0){
                                                   temp=(dX + (WBin +  width / 2.0));
                                                }

                                                else {
                                                   temp=(dX - (WBin +  width / 2.0));
                                                }
                                                
						gt[p] = ratio * 2.0 * cofW2 *    Hoverlap* temp;
						
						
						
                                             }
                                        }
			
			                 if (absdY > (HBin +  height / 2.0)){
                                              gt[q] = 0;

                                        }
                                        else {
                                             if (absdY <= (HBin + height) / 2.0){
						 gt[q] = ratio * ((-2) * cofH1 * dY) * Woverlap;
					     }
                                             else {
                                                double temp =0;
                                                if (dY<=0){
                                                   temp=(dY + (HBin + height/ 2.0));
                                                }

                                                else {
                                                   temp=(dY - (HBin +  height / 2.0));
                                                }
                                                
						gt[q] = ratio * 2.0 * cofH2 * temp * Woverlap;					
						
						
                                             }
                                        }




}


double ExampleFunction::smoothDensity(unsigned wc,unsigned hc,bool No_g,const vector<double> &x){
                       for (int i = 0; i < num; ++i){
		 		
		              if(! _placement.module(i).isFixed()){
					setParameter( wc, hc,i,x);
					                                   
                                        //calculate overlap length
					ChipDensity[wc+BinCount*hc ] += computeOverlap(i);
                                        if (!No_g){
                                              smoothDensity_G(i);
                                        }
					
					
				}			
			}
                       return lamda * pow( (ChipDensity[wc+BinCount * hc ] - ModuleDensity),2);


}

void ExampleFunction::computeDensity(const vector<double> &x, double &f, vector<double> &g,bool No_g){

        
    if(lamda!=0){
        		
	

	for (int wx = 0; wx < BinCount; ++wx){
		for (int hy = 0; hy < BinCount; ++hy){
			
			//calculate (lamda * Σ((Db(x, y)-Tb)^2)) part in objective function
			f += smoothDensity( wx,  hy,No_g,x);
			if (!No_g){
			    for(int k = 0; k < num; ++k){
				g[k*2  ] += gt[2 * k ]* 2.0 * (lamda *ChipDensity[hy*BinCount  + wx] - lamda *ModuleDensity)  ;
				g[1+2 * k ] += gt[2 * k + 1]*  2.0 * (lamda *ChipDensity[hy*BinCount  + wx] - lamda *ModuleDensity) ;
			    }
                        } 
		}
	}
   }
   else {
      return ;
   }

}


double ExampleFunction::computeOverlap(unsigned i){
                                        double width=_placement.module(i).width();
                                        double height=_placement.module(i).height();
                                        ratio =  _placement.module(i).area()/WBin / HBin;
                                        //smoothing bin density	
					if (absdX > (WBin + width / 2.0)){
						Woverlap = 0;
                                                
					}
                                        else {
                                             if (absdX <= (WBin+ width) / 2.0){
						Woverlap = 1 - cofW1 * pow(absdX,2);
					     }
                                             else {
                                                Woverlap = cofW2 * pow((absdX - (WBin + width / 2.0)),2);
                                             }
                                        }
					//smoothing bin density	
					
                 			if (absdY > (HBin +  height / 2.0)){
						Hoverlap = 0;
					}
                                        else {
                                             if (absdY <= (HBin+ height) / 2.0){
						Hoverlap = 1 - cofH1 * pow(absdY,2);
					     }
                                             else {
                                                Hoverlap = cofH2 * pow((absdY - (HBin +  height / 2.0)),2) ;
                                             }
                                        }
                                        
                                        return Hoverlap * Woverlap*ratio;
}


void ExampleFunction::evaluateFG(const vector<double> &x, double &f, vector<double> &g)
{
    /*
    f = x[0]*x[0]*x[0]+3*x[0]*x[0] + 2*x[0]*x[1] + 2*x[1]*x[1] + 7; // objective function
    g[0] = 3*x[0]*x[0]+6*x[0] + 2*x[1]; // gradient function of X
    g[1] = 2*x[0] + 4*x[1]; // gradient function of Y
    */
  
        
	
	
        initialFG(f,g,0);
	computeExp(x);
	computeLSE(f,g,0);
        computeDensity(x,f,g,0);
	
	
}

void ExampleFunction::evaluateF(const vector<double> &x, double &f)
{
      /*
     f = x[0]*x[0]*x[0]+3*x[0]*x[0] + 2*x[0]*x[1] + 2*x[1]*x[1] + 7; // objective function
     */
        vector<double> g;
        initialFG(f,g,1);
	computeExp(x);
	computeLSE(f,g,1);
        computeDensity(x,f,g,1);
	
	

      

}

unsigned ExampleFunction::dimension()
{
    return num * 2; // num_blocks*2 
    // each two dimension represent the X and Y dimensions of each block
}
