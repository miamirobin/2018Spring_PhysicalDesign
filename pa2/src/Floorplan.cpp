

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <climits>
#include "FP.h"
#include <stdlib.h>  
#include <time.h>

#include <queue>
#include <assert.h>
#include "Cell.h"
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
using namespace std; 

unsigned Cell::constant = 1;



void FP::SimulatedAnnealing(){

 
  Cell* Root=InitialPlan(chipWidth, chipHeight);
  pack(Root,chipWidth, chipHeight);                       //traverse(Root);
  
  for (int i=0;i<10;i++){
  	Neighbor* neighbor=new Neighbor;
        
        neighbor->root=CopyCell(Root, neighbor);
        neighborList.push_back(neighbor);
  }
  globalRoot=Root;
  
  globalchipWidth=chipWidth;
  globalchipHeight=chipHeight;
  globalwl=0;
  unsigned wl = 0;
  for (unsigned i = 0; i < NumNet; ++i){
       globalwl += NetList[i]->hpwl(Name2CellMap);
  }
  globalvector=CellList;
/*
  for (int i=0;i<globalvector.size();i++){
       cout << globalvector[i]->GetName()<<"  "<<globalvector[i]->x1<<"  "<<globalvector[i]->y1<<"  "<<globalvector[i]->x2<<"  "<<globalvector[i]->y2<< std::endl;
  }
   cout<<globalchipWidth<<" "<<globalchipHeight<<endl;
*/

  bool fit = IsFit(chipWidth, chipHeight);   // cout<<"f"<<fit<<endl;
  double cost = getCost(Name2CellMap,fit,chipWidth,chipHeight);
  globalCost=cost;
  double T = cost, r = 0.90;
  unsigned P = NumBlock * 30;
  unsigned id;
  double neighCost;
  int success=0;
  while (T > 0.001 or success<1) {         
    if ( (double)clock() / CLOCKS_PER_SEC >180.0){break;}                
    for (unsigned i = 0; i < P; ++i) {          // cout<<"cost:"<<globalCost<<endl;
       //cout<<"neighcost:"<< neighCost<<endl;
      for (int j=0;j<10;j++){   //cout<<"j"<<j<<endl;  
                                //if (j==0 and i==0){traverse(neighborList[j]->root);}

	perturb(neighborList[j],neighborList[j]->celllist); 
        pack(neighborList[j]->root,neighborList[j]->chipWidth, neighborList[j]->chipHeight);     // traverse(neighborList[j]->root);
        double newCost = getCost(neighborList[j]->Name2CellMap,IsFit(neighborList[j]->chipWidth,neighborList[j]->chipHeight),neighborList[j]->chipWidth,neighborList[j]->chipHeight);
	 if (IsFit(neighborList[j]->chipWidth,neighborList[j]->chipHeight)){success++;}
         if (j==0){neighCost=newCost;}
         if (newCost < neighCost) {
          neighCost = newCost;
          id = j;
        }
      } 
     
     // cout<<(neighCost < cost) ;cout<<"c"<<cost<<"n"<<neighCost<<endl;
      if (neighCost < cost || static_cast<double>(rand()) / RAND_MAX < exp((cost - neighCost) / T)) {
       
        
        Neighbor* host= new Neighbor;
        host->root=CopyCell(neighborList[id]->root,host);
        host->chipWidth=neighborList[id]->chipWidth;
        host->chipHeight=neighborList[id]->chipHeight;
        if (neighCost < globalCost and IsFit(host->chipWidth,host->chipHeight)){    
                
               
                delete globalRoot;
                globalCost=neighCost;
		globalRoot=host->root;
		globalchipWidth=host->chipWidth;
		globalchipHeight=host->chipHeight;
                globalwl=0;
                unsigned wl = 0;                               
                for (unsigned i = 0; i < NumNet; ++i){  
                     globalwl += NetList[i]->hpwl(host->Name2CellMap);
                }
                globalvector=host->celllist;
                
                    if (neighCost==5075){
/*
                         traverse(host->root);
                         for (int i=0;i<host->celllist.size();i++){
                                cout << host->celllist[i]->GetName()<<"  "<<host->celllist[i]->x1<<"  "<<host->celllist[i]->y1<<"  "<<host->celllist[i]->x2<<"  "<<host->celllist[i]->y2<< std::endl;
                          }
                          cout<<host->chipWidth<<" "<<host->chipHeight<<endl;

        
         
                           traverse(globalRoot);
                             for (int i=0;i<host->celllist.size();i++){
                                     cout <<  globalvector[i]->GetName()<<"  "<< globalvector[i]->x1<<"  "<< globalvector[i]->y1<<"  "<< globalvector[i]->x2<<"  "<< globalvector[i]->y2<< std::endl;
                              }
                          cout<<globalchipWidth<<" "<<globalchipHeight<<endl;
*/
                       }


        }                         //cout<<"c"<<cost<<"n"<<neighCost<<endl;
 


        cost = neighCost;   
        
        for (int i=0;i<neighborList.size();i++){ 
  	         delete neighborList[i];        // traverse(neighbor->root);
        }          
        //neighborList.clear();
   //        cout<<    neighborList.size()<<endl;
        for (int i=0;i<10;i++){ 
           
  	   Neighbor* neigh=new Neighbor;           
           neigh->root=CopyCell(host->root, neigh);    
           neighborList[i]=(neigh);        // traverse(neighbor->root);
        }
        //  cout<<    neighborList.size()<<endl;*/
      }
      
    }
    std::cerr << "T = " << T << ",   Best Cost= " << globalCost << "  \r" << std::flush;
    T *= r;                      
    // T is higher than cost -> immediately cool down
    if (T > cost) {
         T = cost;
    }
    
  }
 
 // traverse(globalRoot);
 // cout<<"final cost "<<globalCost<<endl;
  /*
  for (int i=0;i<globalvector.size();i++){
       cout << globalvector[i]->GetName()<<"  "<<globalvector[i]->x1<<"  "<<globalvector[i]->y1<<"  "<<globalvector[i]->x2<<"  "<<globalvector[i]->y2<< std::endl;
  }
  cout<<globalchipWidth<<" "<<globalchipHeight<<endl; */
/*
   */
}




Cell* FP::CopyCell(Cell* cell,Neighbor* neighbor){
     Cell* ncell=new Cell(cell->GetName(),cell->width,cell->height);
     neighbor->celllist.push_back(ncell);
     neighbor->Name2CellMap[ncell->GetName()]=ncell;
     ncell->setPos(cell->x1,cell->y1,cell->x2,cell->y2);
     
     if (cell->lchild){ncell->lchild=CopyCell(cell->lchild,neighbor); ncell->lchild->father=ncell; }
     if (cell->rchild){ncell->rchild=CopyCell(cell->rchild,neighbor); ncell->rchild->father=ncell;}
     return ncell;

}

void FP::SetPos2(Cell* cell){
    cell->x2=cell->x1+cell->getX();
    cell->y2=cell->y1+cell->getY();

}

// assign x, y coordinates to blocks
void FP::pack(Cell*root,unsigned& W, unsigned& H) {
  contourList_=new Llist[NumBlock+2];
  contourList_[0].set(0, 0);
  contourList_[1].set(UINT_MAX, 0);
  
  Llist* head = contourList_;
  head->insertNext(contourList_ + 1);
  
  W =0;
  H = 0;    
  nContour_ = 2;                
  packRec(root, head,W, H);          
}

// assign x, y coordinates to a block
void FP::packRec(Cell* node, Llist* head, unsigned& W, unsigned& H) {
  unsigned blockHeight = node->getY();
  unsigned blockWidth  = node->getX();   
  unsigned x = head->xPos_;
  unsigned lastY = head->yPos_, maxY = head->yPos_;
            // cout<<44<<endl;if (!head->next_){cout<<"null"<<endl;}
  while (  head->next_->xPos_  <x + blockWidth) {   
    lastY = head->next_->yPos_;                    
    maxY = std::max(maxY, lastY);                            
    head->removeNext();
  }                                                  
  // set block position
  node->setPos(x, maxY, x + blockWidth, maxY + blockHeight);
  W = max(W, x + blockWidth);
  H = max(H, maxY + blockHeight);
  // update contour
  head->yPos_ =  blockHeight+maxY;
  if (x + blockWidth < head->next_->xPos_) {
    Llist* Lnode = contourList_ + nContour_;
    
    Lnode->set(x + blockWidth, lastY);
    head->insertNext(Lnode);
    nContour_ += 1;
  }
  // pack child
  if (node->lchild!=NULL){
    packRec(node->lchild, head->next_, W, H);
  }
  if (node->rchild!=NULL){
    packRec(node->rchild, head, W, H);
  }
}




// get current floorplan cost, include HPWL if (flag == true)
double FP::getCost(map<string,Cell*>&MAP ,bool flg,unsigned chipWidth,unsigned chipHeight) const {
  double cost = 0.0;                  // cout<<"f"<<flg;
  // unfit cost
  
  if (chipHeight > BoundY) {           //cout<<"2\n";
	cost += 1.5e10 * (chipHeight - BoundY);
  }
  if ( chipWidth > BoundX ) {          //cout<<"1\t";
	cost += 1.5e10 * (chipWidth  - BoundX );
  }
  // alpha * area            
  cost += alpha * chipWidth * chipHeight;               //cout<<NumNet<<endl; 
  // (1 - alpha) * HPWL
  if (flg){
      unsigned wl = 0;
      for (unsigned i = 0; i < NumNet; ++i){  //cout<<i<<endl;cout<<NetList.size();
        wl += NetList[i]->hpwl(MAP);
      }                                        //cout<<NumNet<<endl;
      cost += (1 - alpha) * wl;
  }
  return cost;
}




// check if current floorplan fits the boundary

bool FP::IsFit(unsigned chipWidth,unsigned chipHeight) const{
     if (chipWidth > BoundX or chipHeight > BoundY){return false;}
     else {return  true;}

}

bool FP:: Compare (const Cell* p,const Cell* q) {
      /*if(p->width>q->width){return true; }
      else if (p->width==q->width){return p->height>q->height;}
      else {return false;}*/
      return p->width*p->height>q->width*q->height;

}

Cell* FP::InitialPlan(unsigned& chipWidth,unsigned& chipHeight){
	Cell* Root;  
	sort (CellList.begin(), CellList.end(), Compare);
        int i=1; 
        
        Root=CellList[0];
        int width=CellList[0]->width;
        int height=CellList[0]->height;
        Cell* top =Root;
        while(true){
          
          if(width+CellList[i]->width<=BoundX){
		CellList[i-1]->lchild=CellList[i];
                CellList[i]->father=CellList[i-1];
		width+=CellList[i]->width;
		i++;
          }
	  else {
		
		top->rchild=CellList[i];
                CellList[i]->father=top;
		top=CellList[i];
                height+=CellList[i]->height;
                width=CellList[i]->width;
		i++;
          }
         /*

          
          CellList[i-1]->lchild=CellList[i];
          CellList[i]->father=CellList[i-1];
          i++;*/
	  if(i==CellList.size()){break;}
       }    
       chipWidth=width;
       chipHeight=height;
       return Root;
}





// perturb the tree using three methods
void FP::perturb(Neighbor*neighbor,vector<Cell*>&cellList) {
  unsigned r = rand() % 10;            //cout<<"r"<<r<<endl;
  if (r <= 1) {
    rTurn(cellList);
  } else if (r <= 5) {
     rSwap(neighbor,cellList);
  } else {
     rInsert(neighbor,cellList);
  }
}

// randomly turn a block
void FP::rTurn(vector<Cell*>&cellList) {
  int n = 0, i = rand() % NumBlock;
  cellList[i]->turn();
   
}

// randomly swap two blocks
void FP::rSwap(Neighbor*neighbor,vector<Cell*>&cellList) {
  
 
  int  i = rand() % NumBlock;
  int j = rand() % NumBlock;
  while (i==j){
     j = rand() % NumBlock;
  }
  SwapCell(neighbor,cellList[i],cellList[j]); 
  int n=rand()% 2;
  if (n==1){cellList[i]->turn();}
  n=rand()% 2;
  if (n==1){cellList[j]->turn();}
  
}


void FP::SwapCell(Neighbor*neighbor,Cell* cell1,Cell* cell2){ //cout<<neighbor->root->GetName()<<" "<<cell1->GetName()<<cell2->GetName();
     Cell* left1 = cell1->lchild;
     Cell* right1= cell1->rchild;
     Cell* top1 = cell1->father;
     Cell* left2 = cell2->lchild;
     Cell* right2= cell2->rchild;
     Cell* top2 =  cell2->father;          
     if (cell1->rchild==cell2){
                                                




         if (top1){
          if ( top1->lchild==cell1){   
	      top1->lchild=cell2;
        
          }                               
          else {    top1->rchild=cell2;}   
        }
        else {

           neighbor->root=cell2;

        }  
          cell2->father=top1;
          cell1->father=cell2;
          cell2->rchild=cell1;          
          cell2->lchild=left1;
          if(left1){left1->father=cell2;}
          cell1->rchild=right2;
          if (right2){right2->father=cell1;}
          cell1->lchild=left2;
          if(left2){left2->father=cell1;}      
          
     }
     else if (cell1->lchild==cell2){            
         if (top1){
          if ( top1->lchild==cell1){   
	      top1->lchild=cell2;
        
          }                               
          else {    top1->rchild=cell2;} 
       }  
        else {

           neighbor->root=cell2;

        }
         cell2->father=top1;

          cell1->father=cell2;
          cell2->lchild=cell1;          
          cell2->rchild=right1;
          if(right1){right1->father=cell2;}
          cell1->rchild=right2;
          if (right2){right2->father=cell1;}
          cell1->lchild=left2;
          if(left2){left2->father=cell1;}
     }
     else if (cell2->lchild==cell1){            

                                            
        if (top2){
          if (top2->lchild==cell2){ 
        	top2->lchild=cell1;
        
          }
          else { top2->rchild=cell1;}  
        }
        else {

           neighbor->root=cell1;

        }
         
         cell1->father=top2;

          cell1->lchild=cell2;
          cell2->father=cell1;
          cell1->rchild=right2;
          if (right2){right2->father=cell1;}
                  
          cell2->lchild=left1;
          if(left1){left1->father=cell2;}
          cell2->rchild=right1;
          if (right1){right1->father=cell2;}
          
          
     }
     else if (cell2->rchild==cell1){              


                                           
        if (top2){
          if (top2->lchild==cell2){ 
        	top2->lchild=cell1;
        
          }
          else { top2->rchild=cell1;}  
        }
        else {

           neighbor->root=cell1;

        }
         
         cell1->father=top2;




          cell1->rchild=cell2;
          cell2->father=cell1;
          cell1->lchild=left2;
          if (left2){left2->father=cell1;}
                  
          cell2->lchild=left1;
          if(left1){left1->father=cell2;}
          cell2->rchild=right1;
          if (right1){right1->father=cell2;}
     }
    
     

     else {                                     

        if (top1){
          if ( top1->lchild==cell1){   
	      top1->lchild=cell2;
        
          }                               
          else {    top1->rchild=cell2;}  
        } 
        else {
            
           neighbor->root=cell2;

        }
         cell2->father=top1;
         cell2->rchild=right1;
         if (right1){right1->father=cell2;}
         cell2->lchild=left1;
         if(left1){left1->father=cell2;}
                                         
        if (top2){
          if (top2->lchild==cell2){ 
        	top2->lchild=cell1;
        
          }
          else { top2->rchild=cell1;}  
        }
        else {
            
           neighbor->root=cell1; 
           
        }
         
         cell1->father=top2;
         cell1->rchild=right2;
         if (right2){right2->father=cell1;}
         cell1->lchild=left2;
         if(left2){left2->father=cell1;}  

                                                
   
     
     }
    
}



// randomly remove a block and insert to another place
void FP::rInsert(Neighbor*neighbor,vector<Cell*>&cellList) {
 int  i = rand() % NumBlock; 
 if (cellList[i]==neighbor->root){   
       if (cellList[i]->lchild){SwapCell(neighbor,cellList[i], cellList[i]->lchild);}
       else if (cellList[i]->rchild){SwapCell(neighbor,cellList[i], cellList[i]->rchild);}
 }
 while (cellList[i]->lchild!=NULL and cellList[i]->rchild!=NULL ){
       if (cellList[i]->lchild){SwapCell(neighbor,cellList[i], cellList[i]->lchild);}
       else if (cellList[i]->rchild){SwapCell(neighbor,cellList[i], cellList[i]->rchild);}
 }
 cellList[i]->remove() ;  
 
 int j = rand() % NumBlock; 
 while (i==j){
     j = rand() % NumBlock;
 }
 cellList[j]->insert(cellList[i]);
 j = rand()%2; 
 if (j==1){cellList[i]->turn();}
 
  
}






