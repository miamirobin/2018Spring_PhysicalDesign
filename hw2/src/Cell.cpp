


#include <string>
#include <vector>
#include <map>
#include <climits>
#include "Cell.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
unsigned Cell::getX() const{
   if(direction){return height;}
   else {return width;}
  
}


unsigned Cell::getY() const {
   if(direction){return width;}
   else {return height;}
  
}




void Cell::setPos(unsigned x1_, unsigned y1_, unsigned x2_, unsigned y2_) {
    x1 = x1_; y1 = y1_; x2 = x2_; y2 = y2_;
  }


void Cell::turn(){
    direction=direction^1;


}




void Cell::insert(Cell* cell) {
   Cell* left1 = lchild;
   Cell* right1= rchild;
   int r=rand()%2;
   if(r==0){
	lchild=cell;
	cell->father=this;
        if(left1){left1->father=cell; cell->lchild=left1;}
   }
   else {
	rchild=cell;
	cell->father=this;
        if(right1){right1->father=cell; cell->rchild=right1;}
  }
  
}

bool Cell::remove() {
  
  Cell* top =  father;        //cout<<GetName()<<endl;
/*
  if (!top){return false;}
  if (lchild and rchild){return false;}*/
   /*while (true){
     if (lchild and rchild){        cout<<1;
       
      Cell* childL = lchild ;
      Cell* childR =  rchild;
      lchild = childL->lchild;
      if (lchild) {lchild->father = this;}
      rchild = childL->rchild;
      if (rchild) {rchild->father = this;}
      childL->lchild = this;
      childL->rchild = childR;
      childR->father = childL;
      childL->father = father;
      
      if (this == father->lchild) {
            father->lchild = childL;
      } 
      else {
            father->rchild = childL;
      }
     
      father = childL;
    

     }*/

     if (top->lchild==this){     
	
        
        if (lchild){lchild->father=top;top->lchild=lchild;lchild=NULL;}
        else if (rchild){rchild->father=top;top->lchild=rchild;rchild=NULL;}
        else {top->lchild=NULL;}
        //break;
     }
     else {       
        if (lchild){lchild->father=top;top->rchild=lchild;lchild=NULL;}
        else if (rchild){rchild->father=top;top->rchild=rchild;rchild=NULL;}
        else {top->rchild=NULL;}
        //break;
     }  
  
  father=NULL;
  return true;
}

