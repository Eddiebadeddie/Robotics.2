#include <iostream>

#include "Matrix.h"
#include "Polynomial.h"

using namespace std;

int main(){
    Cubic c = Cubic(1,1,1,1);
    cout<<c<<endl;
    Quadratic q= c.Derive();
    cout<<q<<endl;
    Linear l = q.Derive();
    cout<<l<<endl;

    

    return 0;
}