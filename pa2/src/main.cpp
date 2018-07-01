#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <climits>
#include "myUsage.h"
#include "FP.h"

using namespace std;

int main(int argc, char** argv) {

    MyUsage myuse;
    FP* fm = new FP;
    fm->SetAlpha(argv[1]);
    if (!fm->ReadCircuit(argv[2])){
        cout<<"Parsing Error!\n";
        return 0;
    }
    if (!fm->ReadNet(argv[3])){
        cout<<"Parsing Error!\n";
        return 0;
    }
    
    fm->SimulatedAnnealing();
    fm->WriteOutput(argv[4]);
    delete fm;
    cout<<endl;
    myuse.report(true,true);

}
