
#ifndef CELL_H_
#define CELL_H_

#include <string>
#include <vector>

class Net;
class Cell;

// Class Net Definition
class Net {
 public:
  // Constructor 
  Net(const std::string& name): Name(name) ,NumA(0), NumB(0){}
  // Destructor
  ~Net() {}
  // functions for getting values
  const std::string&        GetName()    const { return Name; }
  std::vector<Cell*> GetCellList() const { return cellList; }

  
  int                       GetNumA() const {return NumA;}
  int                       GetNumB() const {return NumB;}
  void setFlag(bool f) { flag = f; }
  void AddNumA(int k){NumA+=k;} 
  void AddNumB(int k){NumB+=k;}                     
  void ZeroNumA(){NumA=0;} 
  void ZeroNumB(){NumB=0;}  
  void report() const;
  // functions for setting values
 
  void SetFlag(bool f) { flag = f; }
  bool       GetFlag() const { return flag; }
  
  void AddCelltoNet(Cell*cell){cellList.push_back(cell);}
 private:
  std::string         Name;
  std::vector<Cell*>  cellList;
  
  bool                flag;
  int                 NumA;
  int                 NumB;
};


// Class Cell Definition
class Cell {
 public:
  // Constructor 
  Cell(const std::string& name): Name(name), pBack(NULL), pFront(NULL),Tgroup(2), group(2), Ggroup(2),  flag(0), gain(0) {}
  // Destructor
  ~Cell() {}
  // functions for getting values
  const std::string&       GetName()    const { return Name; }
  std::vector<Net*>  GetNetList() const { return netList; }
  unsigned                      GetGgroup()   const { return Ggroup; }
  unsigned                      Getgroup()   const { return group; }
  unsigned                      GetTgroup()   const { return Tgroup; }
  bool                     IsLocked()     const { return  flag==constant; }
  Cell*                    GetpBack()     const { return pBack; }
  Cell*                    GetpFront()     const { return pFront; }
  int                      GetGain()       const {return  gain;}

  
  // functions for setting values
  void SetLock() {flag=constant;}
  void AddGain(int b){gain+=b;}
  void Setgroup(unsigned b){group=b;}
  void SetTgroup(unsigned b){Tgroup=b;}
  void SetGgroup(unsigned b){Ggroup=b;}
  static void ResetFlag(){constant++;}
  static void IniFlag(){constant=1;}
  void ZeroGain(){gain=0;}
  // functions about linked list
  void insert(Cell* cell);
  void remove();
  bool IsA() const{return  group==0;}
  bool TIsA() const {return  Tgroup==0;}
  void AddNetToCell(Net*net){netList.push_back(net);} 
  
  

 private:
  std::string       Name;
  std::vector<Net*> netList;
  int               group_;
  unsigned          Ggroup;
  unsigned          Tgroup;
  unsigned          group;
  Cell*             pBack;
  Cell*             pFront;
  unsigned          flag;
  static unsigned   constant;
  int               gain;
};

#endif  // CELL_H_
