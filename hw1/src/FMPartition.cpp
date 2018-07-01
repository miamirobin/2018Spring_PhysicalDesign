

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <climits>
#include "FM.h"
#include <stdlib.h>  
#include <queue>
#include <assert.h>
#include "Cell.h"
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std; 

unsigned Cell::constant = 1;


bool FM::IsBalanced(){
    if (SizeA>=MinSize and SizeA<=MaxSize){
	return true;
    }
    else {
        return false;
    }
}




void FM::SetInitialPartition(){
   for (unsigned j=0;j<CellList.size() ;j++){



        
         if  (  SizeA>=CellList.size()/2){
                CellList[j]->Setgroup(1);
                CellList[j]->SetTgroup(1);
                CellList[j]->SetGgroup(1);
                SizeB ++;
                TSizeB++;
                GSizeB++; 
                
                
          }
          else {
                CellList[j]->Setgroup(0);
                CellList[j]->SetTgroup(0);
                CellList[j]->SetGgroup(0);
                 SizeA ++;
                TSizeA++;
                GSizeA++;
                             
               


          }

   }
             
   
   Cell::ResetFlag();  
  
}
void FM::InitialNetNum(){
      
    int cutsize=0;
    for (unsigned i=0;i<NetList.size() ;i++){
        NetList[i]->ZeroNumA();
        NetList[i]->ZeroNumB();
        for (int j=0;j<NetList[i]->GetCellList().size();j++){
                                  
                 if(NetList[i]->GetCellList()[j]->GetGgroup()==0){NetList[i]->AddNumA(1);}
                 else {NetList[i]->AddNumB(1);}
        }
        if ( NetList[i]->GetNumA()>0 and  NetList[i]->GetNumB()>0){
                     cutsize++;

        }
    }
   CutSize= cutsize;


}
void FM::InitialCellGain(){
    unsigned Tn, Fn;
    int gain;   
    MaxID[0]=MaxPin*2+1;
    MaxID[1]=MaxPin*2+1;
    for (int i=0;i< CellList.size();i++){
          assert(CellList[i]->GetTgroup()!=2);
          CellList[i]->ZeroGain();
          gain=0;           
          for (int j=0;j<CellList[i]->GetNetList().size();j++){
              
              if ( CellList[i]->GetGgroup()==0){
                   Fn=CellList[i]->GetNetList()[j]->GetNumA();
                   Tn=CellList[i]->GetNetList()[j]->GetNumB();
                   if (Fn==1){gain++;}
                   if (Tn==0){gain--;}

              }
              else {
                   Fn=CellList[i]->GetNetList()[j]->GetNumB();
                   Tn=CellList[i]->GetNetList()[j]->GetNumA();
                   if (Fn==1){gain++;}
                   if (Tn==0){gain--;}

              }
             
          }

          CellList[i]->AddGain(gain);          
          if ( CellList[i]->GetGgroup()==0){
                
                Bucket[0][MaxPin-gain]->insert(CellList[i]);
                if (MaxPin-gain <MaxID[0]){
                    
                   MaxID[0]=MaxPin-gain;
                }
             
          }
          else {
                 CellList[i]->remove();
                Bucket[1][MaxPin-gain]->insert(CellList[i]);
                 if (MaxPin-gain <MaxID[1]){
                    
                   MaxID[1]=MaxPin-gain;
                }
          }
                   
    }     
  
}


bool FM::UpdateCellGain(){
    UpdateMaxID();
       
   

    if ((MaxID[0]==2*MaxPin+1 )and (MaxID[1]==2*MaxPin+1 )){
        
        return false;

    }
             
    if (TSizeA+1>MaxSize){ 
      if (MaxID[0]<2*MaxPin+1){
         MoveCell(Bucket[0][MaxID[0]]->GetpBack() );
      }
      else {
         return false;
      }
    }
    else if (TSizeA-1<MinSize){ 
      if (MaxID[1]<2*MaxPin+1){
         MoveCell(Bucket[1][MaxID[1]]->GetpBack() );
      }
      else {
         return false;
      }
    }
    else     {

       if(MaxID[0]<MaxID[1]){
          MoveCell(Bucket[0][MaxID[0]]->GetpBack() );
       }
       else if (MaxID[0]>MaxID[1]){
         
           MoveCell(Bucket[1][MaxID[1]]->GetpBack() );
       }
       else{
          if ( abs(TSizeA+1-CellList.size()/2.0)<abs(TSizeA-1-CellList.size()/2.0) ){

              MoveCell(Bucket[1][MaxID[1]]->GetpBack() );

          }
          else {
              MoveCell(Bucket[0][MaxID[0]]->GetpBack() );

          }

       }

    }
    return true;
           
  
}


void FM::UpdateMaxID(){
   while(MaxID[0]<MaxPin*2+1){
         if(Bucket[0][MaxID[0]]->GetpBack()==NULL){
                  MaxID[0]++;
         }
         else {
                  break;
        }
   }
   while(MaxID[1]<MaxPin*2+1){
         if(Bucket[1][MaxID[1]]->GetpBack()==NULL){
                  MaxID[1]++;
         }
         else {
                  break;
        }
   }

}


class CmpNet {
 public:
  bool operator () (const Net* x, const Net* y) const {
    return x->GetCellList().size() < y->GetCellList().size();
  }
};

// Generate initial partition
void FM::GenInitialPartition() {
  
}


void FM::MoveCell(Cell*cell){
      int  Fn,Tn;
      UpdateLargestSum(cell);
       cell->SetLock();
       cell->remove();                  
       if (cell->GetTgroup()==0){
            cell->SetTgroup(1);
            for (int i=0;i<cell->GetNetList().size();i++){
                  cell->GetNetList()[i]->AddNumA(-1);
                  cell->GetNetList()[i]->AddNumB(1);
                  
                  TSizeA--;
                  TSizeB++;

                  
                 Fn=cell->GetNetList()[i]->GetNumA();
                 Tn=cell->GetNetList()[i]->GetNumB();
                  


                  if (Fn==1 or Tn==1 or Fn==0 or Tn==0){

                       for (int j=0;j<cell->GetNetList()[i]->GetCellList().size();j++){
                              if (cell->GetNetList()[i]->GetCellList()[j]->IsLocked()){continue;}
                              
                              if(cell->GetNetList()[i]->GetCellList()[j]->GetTgroup()==0){
                                      if (Fn==1){                                           
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(1);
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                          Bucket[0][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                          if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[0]){
                                                    MaxID[0]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                          }        // cout<<1<<cell->GetNetList()[i]->GetCellList()[j]->GetName()<<endl;

                                      }
                                      if (Tn==0){
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(-1);
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                           Bucket[0][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                           if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[0]){
                                                    MaxID[0]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                          }

                                      }
                              }
                              else {
                                     if (Tn==1){
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(1);
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                            Bucket[1][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                            if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[1]){
                                                    MaxID[1]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                            }       // cout<<2<<cell->GetNetList()[i]->GetCellList()[j]->GetName()<<endl;
                                         
                                     }
                                     if (Fn==0){        
                                          cell->GetNetList()[i]->GetCellList()[j]->AddGain(-1);
                                          cell->GetNetList()[i]->GetCellList()[j]->remove();
                                           Bucket[1][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                            if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[1]){
                                                    MaxID[1]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                            }

                                     }
                              }
                       }

                 }                 
                 


             
            }
       }
       else {
            cell->SetTgroup(0);
            for (int i=0;i<cell->GetNetList().size();i++){
                  cell->GetNetList()[i]->AddNumA(1);
                  cell->GetNetList()[i]->AddNumB(-1);
                  TSizeA++;
                  TSizeB--;
                                       
                  
                  Fn=cell->GetNetList()[i]->GetNumA();
                  Tn=cell->GetNetList()[i]->GetNumB();
                  if (Fn==1 or Tn==1 or Fn==0 or Tn==0){

                       for (int j=0;j<cell->GetNetList()[i]->GetCellList().size();j++){
                             if (cell->GetNetList()[i]->GetCellList()[j]->IsLocked()){continue;}

                              if(cell->GetNetList()[i]->GetCellList()[j]->GetTgroup()==0){
                                      if (Fn==1){
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(1);  
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                          Bucket[0][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                           if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[0]){
                                                    MaxID[0]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                          }         // cout<<3<<cell->GetNetList()[i]->GetCellList()[j]->GetName()<<endl;

                                      }
                                      if (Tn==0){
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(-1);
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                           Bucket[0][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                           if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[0]){
                                                    MaxID[0]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                          }

                                      }
                              }
                              else {
                                     if (Tn==1){
                                           cell->GetNetList()[i]->GetCellList()[j]->AddGain(1);
                                           cell->GetNetList()[i]->GetCellList()[j]->remove();
                                            Bucket[1][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                             if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[1]){
                                                    MaxID[1]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                            }         // cout<<4<<cell->GetNetList()[i]->GetCellList()[j]->GetName()<<endl;
                                         
                                     }
                                     if (Fn==0){        
                                          cell->GetNetList()[i]->GetCellList()[j]->AddGain(-1);
                                          cell->GetNetList()[i]->GetCellList()[j]->remove();
                                           Bucket[1][MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()]->insert(cell->GetNetList()[i]->GetCellList()[j]); 
                                            if (MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain()<MaxID[1]){
                                                    MaxID[1]=MaxPin- cell->GetNetList()[i]->GetCellList()[j]->GetGain();
                                            }

                                     }
                              }
                       }

                 }                 
                 


            }
       }
      SizeA=TSizeA;
      SizeB=TSizeB;
       
}


void FM::UpdateLargestSum(Cell* cell){
    static int o=0;
      
       PartialSum+=cell->GetGain();
       if (PartialSum>LargestSum or ((PartialSum==LargestSum) and (abs(TSizeA-CellList.size()/2.0)<abs(SizeA-CellList.size()/2.0) ))){
                
                LargestSum=PartialSum;     
                for (int i=0;i<CellList.size();i++){
                      int g=  CellList[i]->GetTgroup();
                      CellList[i]->Setgroup(g);

                }

       }

  



/*
  if (o<3){
  for (int i=0;i<CellList.size();i++){
           cout   <<" CellName:"<<CellList[i]->GetName()<<CellList[i]->GetGain()<<endl;

  }
  cout<<" CellName:"<<cell->GetName()<<" PartialSum:"<<PartialSum<<" LargestSum:"<<LargestSum<<" TSizeA:"<<TSizeA<<" TSizeB:"<<TSizeB<<endl; 
  } o++;
*/

}

void FM::Partition(){
    int p=0;
    
    SetInitialPartition();  
    //GenInitialPartition();
    while (true){ 
        Cell::ResetFlag(); 
        InitialNetNum();
        BuildBucket();
        InitialCellGain();

        p++;   
        cout<<"\rIeration: "<<p<<" CellSize:"<< CellSize<<" CutSize:"<<CutSize<<" SizeA:"<<GSizeA<<" SizeB:"<<GSizeB<<flush;
      	while (true){
      		
      	    if (!UpdateCellGain()){break;}
            
      		
      	}
        
        if (LargestSum<0){break;}
        else if (LargestSum==0){
              if (abs(SizeA-CellList.size()/2.0)<abs(GSizeA-CellList.size()/2.0) ){
                    GSizeA=SizeA;
                    GSizeB=SizeB;
                    for (int i=0;i<CellList.size();i++){
                      int g=CellList[i]->Getgroup();
                      CellList[i]->SetGgroup(g);

                  }

              }
              break;
        }
        else {
             GlobalLargestSum+=LargestSum;
             CutSize -= LargestSum;
             GSizeA=SizeA;
             GSizeB=SizeB;
             TSizeA=SizeA;
             TSizeB=SizeB;


             for (int i=0;i<CellList.size();i++){
                      int g=CellList[i]->Getgroup();
                      CellList[i]->SetGgroup(g);
                      CellList[i]->SetTgroup(g);

             }

        }
      
        
        ResetNumbers();
        if (CutSize!=cut){cut=CutSize;}
        else {break;}
        //InitialCellGain();
        
        
        LargestSum=0;
        PartialSum=0;
     }
}


void FM::ResetNumbers(){
    int A=0;
    int B=0;
    
  for (unsigned i = 0; i < CellList.size(); ++i){
     if (CellList[i]->GetGgroup()==0){ A++ ;
              CellList[i]->Setgroup(0);
                CellList[i]->SetTgroup(0);
                CellList[i]->SetGgroup(0);
      }
     else {
                 B++;
                CellList[i]->Setgroup(1);
                CellList[i]->SetTgroup(1);
                CellList[i]->SetGgroup(1);
     }

  }
  
    GSizeA=A;
    GSizeB=B;
    TSizeA=A;
    TSizeB=B;
    SizeA=A;
    SizeB=B;
 
}


