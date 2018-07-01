#ifndef FMPART_H_
#define FMPART_H_

#include <string>
#include <vector>
#include <map>
#include <climits>
#include <iostream>
#include "Cell.h"
#include <stdint.h>

class Neighbor;
class Llist;
class FP;

class Neighbor{
    friend class FP;
    Neighbor(){celllist.clear();Name2CellMap.clear();}
    Cell* root;
    std::vector<Cell*> celllist;
    std::map<std::string,Cell*> Name2CellMap;
    unsigned chipWidth;
    unsigned chipHeight;
};



// Class Llist Definition (linked list)
class Llist {
 friend class FP;
 private:
    
  // Constructor / Destructor
  Llist(): next_(NULL) {}
  ~Llist() {}
  // Functions for linked list operations
  
  void insertNext(Llist* node) {
    Llist* n = next_;
    next_ = node;
    node->next_ = n;
  }
  void removeNext() {
    next_ = next_->next_;
  }
  void set(unsigned xPos, unsigned yPos) {
    xPos_ = xPos;
    yPos_ = yPos;
  }
  // Member variables
  Llist*    next_;
  unsigned  xPos_;
  unsigned  yPos_;
};

// Class FM Definition
class FP {
 public:
  // Constructor 
  FP(){ 
      
      chipWidth=0;
      chipHeight=0;
      MaxGain=0;
      CutSize=0;
      MaxID=new int[2];
     
   }
  // Destructor
  ~FP() {
      delete MaxID;
      Initial();

   }
  // functions about setting values
  
  void SetInitialPartition();
  void GenInitialPartition();
  void SetAlpha(const std::string& str);
  void SetMaxPin();
  void SetMaxSize(){   MaxSize= (1+BalanceFactor)/2*GetCellSize() ;      }
  void SetMinSize(){   MinSize= (1-BalanceFactor)/2*GetCellSize();      }
  void SetSizeA();
  void SetBound(int x, int y){BoundX=x; BoundY=y;}
  void SetNumBlock(int x){NumBlock=x;}
  void SetNumTerminal(int x){NumTerminal=x;}
  void SetNumNet (int num){ NumNet=num;}
  // functions about building data structure
  void BuildBucket();

  // functions about getting values
  unsigned GetCellSize()const { return CellList.size();}
  unsigned GetNetSize() const { return NetList.size();} 
  unsigned GetCutSize() const { return CutSize;}
  int      GetMaxSize() const { return MaxSize;}
  int      GetMinSize() const { return MinSize;} 
  int      GetBoundX()  const {return BoundX;}
  int      GetBoundY()  const {return BoundY;}
  int      GetNumBlock()const {return NumBlock;}
  int      GetNumTerminal() const {return NumTerminal;}
  int      GetNumNet() const  {return NumNet;}
  bool     IsFit(unsigned chipWidth,unsigned chipHeight) const;
  static bool     Compare  (const Cell* p,const Cell* q);
  Cell*    InitialPlan(unsigned& chipWidth,unsigned& chipHeight);
  // functions about I/O
  void ResetNumbers();
  void reportPartition() const;
  void Initial();
  bool ReadCircuit(const std::string& filename);
  bool ReadNet(const std::string& filename);
  void WriteOutput(const std::string& filename) const;
  void Partition();
  bool UpdateCellGain();
  void InitialCellGain();
  void MoveCell(Cell* MoveCell);
  void UpdateLargestSum(Cell* cell);
  Net* AddNet(int,int);
  Cell* AddCell(const std::string&,int,int);
  Terminal* AddTerm(const std::string&,int,int);
  void UpdateMaxID();
  void InitialNetNum();
 
  void SimulatedAnnealing();
  Cell*CopyCell(Cell* cell,Neighbor* neighbor); 
  void SetPos2(Cell* cell);
  void pack(Cell*root,unsigned& chipW, unsigned& chipH);
  void packRec(Cell* node, Llist* head, unsigned& chipW, unsigned& chipH);
  double getCost(std::map<std::string,Cell*>&MAP ,bool flg,unsigned chipWidth,unsigned chipHeight) const ;
  void perturb(Neighbor*neighbor,std::vector<Cell*>&cellList);
  void rTurn(std::vector<Cell*>&cellList) ;
  void rSwap(Neighbor*neighbor,std::vector<Cell*>&cellList);
  void rInsert(Neighbor*neighbor,std::vector<Cell*>&cellList);
  void SwapCell(Neighbor*neighbor,Cell* cell1,Cell* cell2);
  void traverse(Cell* Root) const ;
  void traverseRec(Cell* Root, int depth) const;

 private:
  double                 alpha;
  int                 NumBlock;
  int                 BoundX;
  int                 BoundY;
  int                 NumTerminal;
  bool                terminate;
  double              BalanceFactor;        // balance factor
  Cell*               movecell;
  int                 NumNet;
  std::vector<Net*>   NetList;              // list of nets
  std::vector<Cell*>  CellList;             // list of cells
  std::vector<Cell*>  VecCell[10];
  std::vector<Terminal*>TermList;
  int                 CutSize;                // current cut size
  int                 BestCost;
  double              MinSize;              // min size of a group
  double              MaxSize;              // current group size
  unsigned                 chipWidth;
  unsigned                 chipHeight;
  std::vector<Cell*>  Bucket[2];
  bool                MoveANotBalanced;
  bool                MoveBNotBalanced;
  int                 MaxId;
  int                 MaxPin;
  unsigned            globalchipHeight;
  unsigned            globalchipWidth;
  unsigned            globalwl;
  std::vector<Cell*>       globalvector;
  double              globalCost;
  unsigned            CellSize;
  int                 PartialSum;
  int                 LargestSum;
  int                 GlobalLargestSum;
  int                 MaxGain;
  std::map<std::string, Cell*>        Name2CellMap;
  std::map<std::string, Terminal*>    Name2TermMap;
  int *               MaxID;
  int                 cut;
  Cell *              root;
  Llist*    contourList_;   // list of linked list nodes
  unsigned  nContour_;      // linked list memory management counter
  Cell*  globalRoot;
  std::vector<Neighbor*>  neighborList;
};

#endif    // FMPART_H_
