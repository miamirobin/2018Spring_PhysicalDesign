



#include <string>
#include <vector>
#include <map>
#include <climits>

#include<iostream>

#include "Cell.h"

using namespace std;

double Net::hpwl(map<string,Cell*>&MAP) const {
   //cout<<termList.size()<<cellList.size();
  unsigned minX, minY, maxX, maxY, centerX, centerY;      //cout<<2;
  if (termList.size()>0){
  	minX = maxX = termList[0]->x + termList[0]->x;
  	minY = maxY = termList[0]->y + termList[0]->y;
  }
  else {          // cout<<4;
	string name=cellList[0];
        Cell* cell=MAP[name];
        minX = maxX = cell->x1 + cell->x2;
  	minY = maxY = cell->y1 + cell->y2;      //cout<<5;
  }
  for (unsigned i = 0; i < termList.size(); ++i) {
    
    centerX = termList[i]->x + termList[i]->x;
    centerY = termList[i]->y + termList[i]->y;
    minX = std::min(minX, centerX);
    minY = std::min(minY, centerY);
    maxX = std::max(maxX, centerX);
    maxY = std::max(maxY, centerY);
  }
  for (unsigned i = 0; i < cellList.size(); ++i) {
    string name=cellList[i];
    Cell* cell=MAP[name];
    centerX = cell->x1 + cell->x2;
    centerY = cell->y1 + cell->y2;
    minX = std::min(minX, centerX);
    minY = std::min(minY, centerY);
    maxX = std::max(maxX, centerX);
    maxY = std::max(maxY, centerY);
  }


  return ((maxX - minX) + (maxY - minY))/2.0;
}
