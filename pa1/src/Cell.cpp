


#include <string>
#include <vector>
#include <map>
#include <climits>
#include "Cell.h"






void Cell::insert(Cell* cell) {
  Cell* next=pBack;
  pBack = cell;
  cell->pFront = this;
  cell->pBack=next;
  if(next){next->pFront=cell;}
}

void Cell::remove() {
  Cell* next = pBack;
  Cell* front= pFront;
  if (front){front->pBack = next;}
  if (next){next->pFront = front;}
  pBack=NULL;
  pFront=NULL;
}

