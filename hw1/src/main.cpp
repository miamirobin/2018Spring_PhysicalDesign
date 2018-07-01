#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <climits>
#include "myUsage.h"
#include "FM.h"

using namespace std;

int main(int argc, char** argv) {

    MyUsage myuse;
    FM* fm = new FM;
    if (!fm->ReadCircuit(argv[1])){
        cout<<"Parsing Error!\n";
        return 0;
    }
    else {cout<<"Parsing Success!\n";}
    fm->Partition();
    fm->WriteOutput(argv[2]);
    delete fm;
    cout<<endl;
    myuse.report(true,true);

}
