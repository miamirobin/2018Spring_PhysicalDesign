
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <cassert>
#include <ctime>
#include <map>
#include <set>
#include "FP.h"
#include "parse.h"
#include <time.h>
#define VERBOSE 0

using namespace std;





void FP::Initial() {
  for (unsigned i = 0; i < NetList.size(); ++i) {
 	delete NetList[i];
  }
  
  for (unsigned i = 0; i < TermList.size(); ++i){
	delete TermList[i];
  }
  CellList.clear();
  NetList.clear();
  TermList.clear();
}


void FP::BuildBucket(){
    Bucket[0].clear();
    Bucket[1].clear();
   

}

void FP::SetAlpha(const string& str)  {
  
  alpha=(StrToDouble(str)); 
   
}

Net* FP::AddNet(int id,int degree){
    Net* net=new Net(id,degree);
    NetList.push_back(net);
        
    return net;
}

Terminal* FP::AddTerm(const string& str,int a,int b){
    Terminal* terminal;
    
    map<string,Terminal*>::const_iterator it;
    it=Name2TermMap.find(str);
    if(it== Name2TermMap.end()){
           terminal=new Terminal(str,a,b); 
           
           Name2TermMap[str]=terminal;
           TermList.push_back(terminal);
    }
    else{
         terminal=Name2TermMap[str];
    }
    
    return terminal;
}

Cell* FP::AddCell(const string& str,int a,int b){
    Cell* cell;
    
    map<string,Cell*>::const_iterator it;
    it=Name2CellMap.find(str);
    if(it== Name2CellMap.end()){
           cell=new Cell(str,a,b); 
           
           Name2CellMap[str]=cell;
           CellList.push_back(cell);
         
    }
    else{
         cell=Name2CellMap[str];
    }
    
    return cell;
}

bool FP::ReadNet(const string& filename){
  
  ifstream ifs(filename.c_str());
  string str;
  Net * net;
  int netID=0;
  int count;
  Cell* cell;
  Terminal* terminal;
  std::vector<std::string> tokens;
   
          
  while (!ifs.eof()) {                    
    str = readUntil(ifs, '\n');
    tokens = split(str, " :;\t\n\r");
    unsigned n = tokens.size();                                      
    if (n == 0) {continue;}
    if (tokens[0]=="NumNets"){
    	SetNumNet(Str2Int(tokens[1]));       

    }
    else if (tokens[0] == "NetDegree") {
     	net=AddNet(netID,Str2Int(tokens[1]));
     	netID++;

    }
    else{
    	map<string,Cell*>::const_iterator itc;
    	map<string,Terminal*>::const_iterator itt;
        itc=Name2CellMap.find(tokens[0]);
        if (itc!=Name2CellMap.end()){
        	cell=itc->second;
        	net->AddCelltoNet(tokens[0]);
        	cell->AddNetToCell(net);

        }
        else {
        	itt=Name2TermMap.find(tokens[0]);
        	terminal=itt->second;
        	net->AddTermtoNet(terminal);
        	terminal->AddNetToTerm(net);


        }
        
    
       
    }
   
  }
  ifs.close();
  //cout<<NetList.size();
   
  
  return true;
  

}


bool FP::ReadCircuit(const string& filename) {

  

  ifstream ifs(filename.c_str());
  string str;
  int a,b,c;
  Cell* cell;
  Terminal* terminal;
  std::vector<std::string> tokens;
  Initial();                
       
  while (!ifs.eof()) {
    str = readUntil(ifs, '\n');
    tokens = split(str, " :;\t\n\r");
    unsigned n = tokens.size();
    if (n == 0) {continue;}
    if (tokens[0] =="Outline"){
        a=Str2Int(tokens[1]);
        b=Str2Int(tokens[2]);
    	SetBound(a,b);
    	

    }
    else if (tokens[0] =="NumBlocks"){
        a=Str2Int(tokens[1]);
    	SetNumBlock(a);
    	

    }
    else if (tokens[0] =="NumTerminals"){
	a=Str2Int(tokens[1]);
    	SetNumTerminal(a);
    }
    else if (tokens[1] =="terminal"){
        a=Str2Int(tokens[2]);
        b=Str2Int(tokens[3]);
    	terminal=AddTerm(tokens[0],a,b);

    }
    else {
       assert(n == 3);
       a=Str2Int(tokens[1]);
       b=Str2Int(tokens[2]);
       if (b>a){int temp=b;b=a;a=temp;}
       cell=AddCell(tokens[0],a,b);
        
        
    }
    
    
  }
  ifs.close();

  
  
  return true;
  
}

void FP::WriteOutput(const std::string& filename) const {
  

  std::ofstream ofs(filename.c_str());
  ofs <<  globalCost << std::endl;
  ofs << globalwl << std::endl;
 
  ofs << globalchipWidth*globalchipHeight << std::endl;
  ofs << globalchipWidth<<" " << globalchipHeight << std::endl;
  ofs << (double)clock() / CLOCKS_PER_SEC /1.5<< endl;
  for (int i=0;i<globalvector.size();i++){
       ofs << globalvector[i]->GetName()<<"  "<<globalvector[i]->x1<<"  "<<globalvector[i]->y1<<"  "<<globalvector[i]->x2<<"  "<<globalvector[i]->y2<< std::endl;
  }

  ofs.close();
}


