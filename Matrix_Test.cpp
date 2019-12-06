#include <iostream>

#include "Matrix.h"
#include "Polynomial.h"

using namespace std;

int main(){
    Polynomial q_0 = Polynomial(0);
    Polynomial q_1 = Polynomial(1);

    Polynomial v_0 = q_0.Derive();
    Polynomial v_1 = q_1.Derive();

    Matrix m = Matrix(q_0, v_0, q_1, v_1);

    cout<<m << endl;

    double det = m.Determinant();

    cout<<det<<endl;

    return 0;
}