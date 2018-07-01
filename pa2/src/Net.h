#ifndef NET_H_
#define NET_H_

#include <string>
#include <vector>




// Class Net Definition
class Net {
 public:
  // Constructor 
  Net(const std::string& name): Name(name) ,NumA(0), NumB(0){}
  // Destructor
  ~Net() {}
  // functions for getting values
  const std::string&        GetName()    const { return Name; }
  std::vector<Cell*>& GetCellList() const { return cellList; }

  bool                      getFlag() const { return flag_; }
  int                       GetNumA() const {return NumA;}
  int                       GetNumB() const {return NumB;}

  void AddNumA(int k){NumA+=k;} 
  void AddNumB(int k){NumB+=k;}                     

  void report() const;
  // functions for setting values
 
  void SetFlag(bool f) { flag = f; }
  
  
  void AddCelltoNet(Cell*cell);
 private:
  std::string         Name;
  std::vector<Cell*>  cellList;
  
  bool                flag;
  int                 NumA;
  int                 NumB;
};


#endif  // NET_H_
