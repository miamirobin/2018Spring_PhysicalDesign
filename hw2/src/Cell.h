
#ifndef CELL_H_
#define CELL_H_

#include <string>
#include <vector>

class Net;
class Cell;
class Terminal;

// Class Terminal Definition
class Terminal {
 friend class Net;
 public:
  // Constructor 
  Terminal(const std::string& name, int x1, int y1): Name(name), x(x1), y(y1), pBack(NULL), pFront(NULL),  flag(0), gain(0) {}
  // Destructor
  ~Terminal() {}
  // functions for getting values
  const std::string&       GetName()    const { return Name; }
  std::vector<Net*>  GetNetList() const { return netList; }
  int                GetNetSize() const {return netList.size();}
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
  
  static void ResetFlag(){constant++;}
  static void IniFlag(){constant=1;}
  void ZeroGain(){gain=0;}
  // functions about linked list
  void insert(Cell* cell);
  void remove();
  bool IsA() const{return  group==0;}
  bool TIsA() const {return  Tgroup==0;}
  void AddNetToTerm(Net*net){netList.push_back(net);} 
  
  

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
  int               x;
  int               y;
};



// Class Net Definition
class Net {
 public:
  // Constructor 
  Net(int ID,int num): Id(ID), Num(num) {}
  // Destructor
  ~Net() {}
  // functions for getting values
  const std::string&        GetName()    const { return Name; }
  
 
 
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
  double hpwl(std::map<std::string,Cell*>&MAP) const;
  void AddCelltoNet(std::string cell){cellList.push_back(cell);}
  void AddTermtoNet(Terminal* term){termList.push_back(term);}
 private:
  std::string         Name;
  std::vector<std::string>  cellList;
  
  std::vector<Terminal*> termList;
  int                 Id;
  bool                flag;
  int                 NumA;
  int                 NumB;
  int                 Num;
};


// Class Cell Definition
class Cell {
 friend class FP;
 friend class Net;
 public:
  // Constructor 
  Cell(const std::string& name, int x, int y): Name(name), width(x), height(y), lchild(NULL), rchild(NULL),father(NULL), pnext(NULL),  flag(0), direction(1) {}
  // Destructor
  ~Cell() {}
  // functions for getting values
  const std::string&       GetName()    const { return Name; }
  std::vector<Net*>  GetNetList() const { return netList; }
  int                GetNetSize() const {return netList.size();}
  
  bool                     IsLocked()     const { return  flag==constant; }
  Cell*                    GetpBack()     const { return pnext; }
  unsigned  getX()   const;
  unsigned  getY()   const ;
  
  // functions for setting values
  void SetLock() {flag=constant;}
  void AddGain(int b){gain+=b;}
  
  
  // functions about linked list
   
  bool IsA() const{return  flag==0;}
 
  void AddNetToCell(Net*net){netList.push_back(net);} 
  void setPos(unsigned x1_, unsigned y1_, unsigned x2_, unsigned y2_);
  void turn();
  void swap(Cell*cell2);
  void insert(Cell* cell);
  bool remove();

 private:
  std::string       Name;
  std::vector<Net*> netList;
  int               group_;
  int               x1;
  int               y1;
  int               x2;
  int               y2;
  Cell*             rchild;
  Cell*             lchild;
  Cell*             father;
  unsigned          flag;
  static unsigned   constant;
  int               gain;
  int               width;
  int               height;
  Cell*             pnext;
  bool              direction;

};

#endif  // CELL_H_
