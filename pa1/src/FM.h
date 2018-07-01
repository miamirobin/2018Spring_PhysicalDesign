#ifndef FMPART_H_
#define FMPART_H_

#include <string>
#include <vector>
#include <map>
#include <climits>
#include <iostream>
#include "Cell.h"


// Class FM Definition
class FM {
 public:
  // Constructor 
  FM(){ 
      MaxPin=0;
      MoveANotBalanced=false;
      MoveBNotBalanced=false;
      terminate=false; 
      PartialSum=0;
      LargestSum=0;
      GlobalLargestSum=0;
      MaxGain=0;
      CutSize=0;
      MaxID=new int[2];
       SizeA=0;
       TSizeA=0;
       GSizeA=0;
       SizeB=0;
       TSizeB=0;
      GSizeB=0;
      cut=0;
   }
  // Destructor
  ~FM() {
      delete MaxID;
      Initial();

   }
  // functions about setting values
  
  void SetInitialPartition();
  void GenInitialPartition();
  void SetMaxPin();
  void SetMaxSize(){   MaxSize= (1+BalanceFactor)/2*GetCellSize() ;      }
  void SetMinSize(){   MinSize= (1-BalanceFactor)/2*GetCellSize();      }
  void SetSizeA();
  
  // functions about building data structure
  void BuildBucket();

  // functions about getting values
  unsigned GetCellSize()const { return CellList.size();}
  unsigned GetNetSize() const { return NetList.size();} 
  unsigned GetCutSize() const { return CutSize;}
  int      GetMaxSize() const { return MaxSize;}
  int      GetMinSize() const { return MinSize;} 
  bool IsBalanced();
  // functions about I/O
  void ResetNumbers();
  void reportPartition() const;
  void Initial();
  bool ReadCircuit(const std::string& filename);
  void WriteOutput(const std::string& filename) const;
  void Partition();
  bool UpdateCellGain();
  void InitialCellGain();
  void MoveCell(Cell* MoveCell);
  void UpdateLargestSum(Cell* cell);
  Net* AddNet(const std::string&);
  Cell* AddCell(const std::string&);
  void UpdateMaxID();
  void InitialNetNum();

 private:
  
  
  

  bool                terminate;
  double              BalanceFactor;        // balance factor
  Cell*               movecell;
 
  std::vector<Net*>   NetList;              // list of nets
  std::vector<Cell*>  CellList;             // list of cells
  int                 CutSize;                // current cut size
  int                 BestCost;
  double              MinSize;              // min size of a group
  double              MaxSize;              // current group size
  
  std::vector<Cell*>  Bucket[2];
  bool                MoveANotBalanced;
  bool                MoveBNotBalanced;
  int                 MaxId;
  int                 MaxPin;
  unsigned            SizeA;
  unsigned            TSizeA;
  unsigned            GSizeA;
  unsigned            SizeB;
  unsigned            TSizeB;
  unsigned            GSizeB;
  unsigned            CellSize;
  int                 PartialSum;
  int                 LargestSum;
  int                 GlobalLargestSum;
  int                 MaxGain;
  std::map<std::string, Cell*>        Name2CellMap;
  int *               MaxID;
  int                 cut;
};

#endif    // FMPART_H_
