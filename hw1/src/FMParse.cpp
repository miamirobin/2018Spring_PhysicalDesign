
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <cassert>
#include <ctime>
#include <map>
#include <set>
#include "FM.h"
#include "parse.h"

#define VERBOSE 0
using namespace std;





void FM::Initial() {
  for (unsigned i = 0; i < NetList.size(); ++i) {
 	delete NetList[i];
  }
  
  for (unsigned i = 0; i < CellList.size(); ++i){
	delete CellList[i];
  }
  CellList.clear();
  NetList.clear();
  Bucket[0].clear();
  Bucket[1].clear();
}


void FM::BuildBucket(){
    Bucket[0].clear();
    Bucket[1].clear();
    for (unsigned i = 0; i < 2*MaxPin+1; ++i) {
            Cell* cell1=new Cell("null");
            Cell* cell2=new Cell("null");
            Bucket[0].push_back(cell1);
            Bucket[1].push_back(cell2);
    }


}

void FM::SetMaxPin()  {
  
  for (unsigned i = 0; i < CellList.size(); ++i) {
     if (CellList[i]->GetNetList().size()>MaxPin){
        MaxPin = CellList[i]->GetNetList().size();
     }
  }

   
}

Net* FM::AddNet(const string& str){
    Net* net=new Net(str);
    NetList.push_back(net);
        
    return net;
}


Cell* FM::AddCell(const string& str){
    Cell* cell;
    
    map<string,Cell*>::const_iterator it;
    it=Name2CellMap.find(str);
    if(it== Name2CellMap.end()){
           cell=new Cell(str); 
           
           Name2CellMap[str]=cell;
           CellList.push_back(cell);
    }
    else{
         cell=Name2CellMap[str];
    }
    
    return cell;
}


bool FM::ReadCircuit(const string& filename) {

  

  ifstream ifs(filename.c_str());
  string str;
  Net* net;
  Cell* cell;
  std::vector<std::string> tokens;
  Initial();                
  str = readUntil(ifs, '\n');  
  BalanceFactor=(StrToDouble(str));         
  while (!ifs.eof()) {
    str = readUntil(ifs, ';');
    tokens = split(str, " ;\t\n\r");
    unsigned n = tokens.size();
    if (n == 0) {continue;}
   
    if (tokens[0] == "NET") {
     
      assert(n >= 2);
      net=AddNet(tokens[1]);
      set<string> s;
      for (unsigned i = 2; i < n; ++i){
        if (s.find(tokens[i]) == s.end()) {
             s.insert(tokens[i]);
       } 
        else {continue;}
        //if (tokens[1]=="n53"){cout<<tokens[i]<<endl;}

        cell=AddCell(tokens[i]);
        net->AddCelltoNet(cell);
        cell->AddNetToCell(net);
      } 
    }
    else {
         return false;
    }
  }
  ifs.close();

  SetMaxPin();
  CellSize=CellList.size();
  BuildBucket();
  SetMinSize();
  SetMaxSize();
  
  return true;
  
}

void FM::WriteOutput(const std::string& filename) const {
  int A=0;
  int B=0;
  int cutsize=0;
  for (unsigned i = 0; i < CellList.size(); ++i){
     if (CellList[i]->GetGgroup()==0){ A++ ;}
     else {B++;}
  }
  
  for (unsigned i=0;i<NetList.size() ;i++){
       NetList[i]->ZeroNumA();
       NetList[i]->ZeroNumB();
       for (int j=0;j<NetList[i]->GetCellList().size();j++){
         if (NetList[i]->GetCellList()[j]->GetGgroup()==0){
                
                NetList[i]->AddNumA(1);
              
           }

           else {
                NetList[i]->AddNumB(1);
           }
           
           
       }
      if ( NetList[i]->GetNumA()>0 and  NetList[i]->GetNumB()>0){
                     cutsize++;

      }

  }

  std::ofstream ofs(filename.c_str());
  ofs << "Cutsize = " << cutsize << std::endl;
  ofs << "G1 " << A << std::endl;
  for (unsigned i = 0; i < CellList.size(); ++i){
     if (CellList[i]->GetGgroup()==0){
           ofs << CellList[i]->GetName() << " ";
     }
  }
  ofs << ";" << std::endl;
  ofs << "G2 " << B << std::endl;
  for (unsigned i = 0; i < CellList.size(); ++i){
     if (CellList[i]->GetGgroup()==1){
         ofs <<  CellList[i]->GetName() << " ";
     }
  }
  ofs << ";" << std::endl;
  ofs.close();
}


