#include <iostream>

#include "Matrix.h"

using namespace std;

int main(){
    Matrix m = Matrix(4);

    cout<<m<<endl;

    m.ScanInValues();

    cout<<m<<endl;

    Matrix A = Matrix(4);

    A.ScanInValues();

    A = A * m;

    cout<<endl<<m<<endl;

    m = A + m;

    //cout<<"Made it out of multiplication"<<endl;
    cout<< m <<endl << endl;

    A = A * A;

    cout<< A << endl;

    Matrix sub = A.SubMatrix(2,2);

    cout<<sub<<endl;

    return 0;
}