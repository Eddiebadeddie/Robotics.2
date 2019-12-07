#include "Matrix.h"

Matrix::Matrix(){
    size = 0;
    matrix = NULL;
}

Matrix::Matrix(int n):
    size(n){
        matrix = NULL;
        SetUpMatrix();
}

Matrix::Matrix(const Matrix &m){
    size = m.size;
    
    matrix = new double* [size];
    for(int i = 0; i < size; ++i){
        matrix[i] = new double [size];
        for(int j = 0; j < size; ++j){
            matrix[i][j] = m.matrix[i][j];
        }
    }
}

Matrix::Matrix(Polynomial &x_0, Polynomial &v_0, Polynomial &x_1, Polynomial &v_1){
    matrix = NULL;
    size = 4;

    SetUpMatrix();

    for(int i = 0; i < size; ++i){
        for(int j =0; j < size; ++j){
            if(i == 0)
                matrix[i][j] = x_0.coefficients[j];
            if(i == 1)
                matrix[i][j] = v_0.coefficients[j];
            if(i == 2)
                matrix[i][j] = x_1.coefficients[j];
            if(i == 3)
                matrix[i][j] = v_1.coefficients[j];
            
        }
    }
}

Matrix::Matrix(char i){
    switch(i){
        case 'I':
        case 'i':
            break;
        default:
            throw RuntimeException("Not a valid operation");
    }

    size = 4;
    matrix = NULL;
    SetUpMatrix();

    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if(i == j)
                matrix[i][j] = 1;
        }
    }
}

Matrix::~Matrix(){
    for(int i = 0; i < size; ++i){
        delete[] matrix[i];
    }
    delete[] matrix;
}

//-----------------UTILITIES-------------------------------------------------
//Creates a matrix excluding the x row and y column
Matrix &Matrix::SubMatrix(int x, int y){
    int new_size = size - 1;

    Matrix *sub = new Matrix(new_size);

    for(int i = 0, sub_i = 0; i < size; ++i){
        if(i == y){
            continue;
        }
        for(int j = 0, sub_j = 0; j < size; ++j){
            if(j == x){
                continue;
            }

            sub->matrix[sub_i][sub_j] = matrix[i][j];
            ++sub_j;
        }
        ++sub_i;
    }

    return *sub;
}

//Gets the determinant of the matrix
double Matrix::Determinant(){
    if(size == 2){
        return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
    }
    
    double sum = 0;

    for(int i = 0; i < size; ++i){
        Matrix s = SubMatrix(0,i);

        if(matrix[0][i] == 0)
            continue;

        if( i%2 == 0){
            sum += (matrix[0][i] * s.Determinant());    
        }
        else{
            sum -= (matrix[0][i] * s.Determinant());
        }
    }

    return sum;
}

void Matrix::ScanInValues(){
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            cout<<"matrix["<<i<<" , " <<j<<"] = ";
            cin>>matrix[i][j];
        }
    }
}

void Matrix::SetUpMatrix(){
    if(matrix != NULL){
        cout<<"This matrix already exists"<<endl;
        return;
    }

    matrix = new double* [size];
    for(int i = 0; i < size; ++i){
        matrix[i] = new double[size];
        for(int j = 0; j < size; ++j){
            matrix[i][j] = 0;
        }
    }
}

double Matrix::At(int i, int j){
    return matrix[i][j];
}

Matrix &Matrix::Coffactor(){
    Matrix *ret = new Matrix(4);

    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            Matrix c = SubMatrix(i,j);
            ret->matrix[i][j] = c.Determinant();
        }
    }

    return *ret;
}

Matrix &Matrix::Adjugate(){
    Matrix *ret = new Matrix(*this);
    ret->Coffactor();

    for(int  i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            if((i+j)%2 != 0 && ret->matrix[i][j] != 0){
                ret->matrix[i][j] *= -1;
            }
        }
    }

    for(int  i = 0; i < size; ++i){
        for(int j = i; j < size; ++j){
            if(i == j)
                continue;
            double temp = ret->matrix[i][j];
            ret->matrix[i][j] = ret->matrix[j][i];
            ret->matrix[j][i] = temp; 
        }
    }
    
    return *ret;
}
//------------OPERATORS-------------------------------------------------
Matrix &Matrix::operator= (const Matrix& m){
    size = m.size;
    CopyMatrix(m);
    return *this;
}

Matrix &operator*(const Matrix &left, const Matrix &right){
    //cout<<"MULTIPLYING"<<endl;
    Matrix *ret = new Matrix(left.size);
    if(left.size != right.size){
        cout<<"ERROR: matrices sizes are not compatible. Did not multiply"<<endl;
        return *ret;
    }

    for(int i = 0; i < left.size; ++i){
        for(int j = 0; j < left.size; ++j){
            double sum = 0;
            for (int k = 0; k < left.size; ++k){
                sum += (left.matrix[i][k] * right.matrix[k][j]);
            }
            cout<<"matrix["<<i<<" , " <<j<<"] = " << sum<<"\t";
            ret->matrix[i][j] = sum;
        }
        //cout<<"iterator"<<endl;
    }
    cout << endl << endl;
    return *ret;
}

Matrix &operator* (const double left, const Matrix &right){
    Matrix *ret = new Matrix(right);

    for(int  i = 0; i < right.size; ++i){
        for(int j = 0; j < right.size; ++j){
            right.matrix[i][j] *= left;
        }
    }

    return *ret;
}

Matrix &operator+(Matrix const &left, Matrix const &right){
    //cout<<"ADDING"<<endl;
    Matrix *ret = new Matrix(left.size);
    
    if(left.size != right.size){
        cout<<"ERROR: matrices sizes are not compatible. Did not multiply"<<endl;
        return *ret;
    }

    //cout << "left: \n" << left << endl;
    //cout << "right: \n" << right << endl;

    for(int i = 0; i < left.size; ++i){
        for(int j = 0; j < left.size; ++j){
            //cout<<"matrix["<<i<<" , " <<j<<"] = " <<  left.matrix[i][j] <<"\n";
            //cout<<"m.matrix["<<i<<" , " <<j<<"] = " <<  right.matrix[i][j] <<"\n";
            ret->matrix[i][j] = left.matrix[i][j] + right.matrix[i][j];
            //cout<<"matrix["<<i<<" , " <<j<<"] = " <<  ret->matrix[i][j] <<"\n\n";
        }
    }
    //cout << endl << endl;
    return *ret;
}

ostream &operator<<(ostream &out, const Matrix &m){
    for(int i = 0; i< m.size ; ++i){
        out<<"[";
        for(int j = 0; j < m.size; ++j){
            out<<m.matrix[i][j]<<", ";
        }
        out<<"]"<<endl;
    }
    return out;
}

void Matrix::CopyMatrix(const Matrix &m){
    if(size != m.size){
        cout<<"Could not copy matrix. Dimensions are wrong"<<endl;
        return;
    }

    CopyMatrix(m.matrix);
}

void Matrix::CopyMatrix(double** m){
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            cout<<"This matrix["<<i<<" , " <<j<<"] = " <<  matrix[i][j] << endl;
            cout<<"That matrix["<<i<<" , " <<j<<"] = " <<  m[i][j] << endl;
            matrix[i][j] = m[i][j];
            cout<<"Now this matrix["<<i<<" , " <<j<<"] = " <<  matrix[i][j] << endl << endl;
        }
    }
}
//---------------------------------------------------------
RowVector::RowVector(){
    column = new double[4];
    for(int i = 0; i < size; ++i){
        column[i] = 0;
    }
}

RowVector::RowVector(const RowVector &r){
    column = new double[4];

    for(int i = 0; i < size; ++i){
        column[i] = r.column[i];
    }
}

RowVector::~RowVector(){
    delete[] column;
}

void RowVector::CopyColumn(const RowVector &r){
    for(int i = 0; i < size; ++i){
        column[i] = r.column[i];
    }
}

void RowVector::ScanInValues(){
    for(int i = 0; i < size; ++i){
        cout<<"column[" << i << "] : ";
        cin >> column[i];
    }
}

RowVector& RowVector::operator= (RowVector &right){
    CopyColumn(right);
}

bool RowVector::operator== (const RowVector &right){
    for(int i = 0; i < size; ++i){
        if(right.column[i] != column[i])
            return false;
    }
    return true;
}

RowVector &operator*  (Matrix &m, const RowVector &r){
    RowVector *ret = new RowVector();
    for(int i = 0; i < r.size; ++i){
        double sum = 0;
        
        for(int j = 0; j < r.size; ++j){
            sum += (m.At(i,j) + r.column[j]);
        }

        ret->column[i] = sum;
    }

    return *ret;
}

ostream &operator<< (ostream &out, const RowVector &r){
    for(int i = 0; i < r.size; ++i){
        out << "[";
        out<<r.column[i];
        out<< "]"<<endl;
    }

    return out;
}

Cubic &RowVector::CreateCubic(){
    double temp[4];

    for(int i = 0; i < size; ++i){
        temp[i] = column[i];
    }

    Cubic *ret = new Cubic(temp[0],temp[1], temp[2], temp[3]);
    ret->Allocate();
    return *ret;
}

//---------------------------------------------------------
Linear::Linear(){
    degree = 1;
}

Linear::Linear(int x_0, int x_1){
    degree = 1;
    coefficients.push_back(x_0);
    coefficients.push_back(x_1);
}

Linear::~Linear(){
    cout<<"~Linear"<<endl;
    coefficients.clear();
}

ostream &operator<< (ostream &out, const Linear &c){
    out<<"a(t) = ";
    for(int i = c.degree; i >= 0; --i){
        if(c.coefficients[i] == 0)
            continue;
        else if(i == 0)
            out<<c.coefficients[i];
        else
            out<<c.coefficients[i]<<"t^"<<i<<" + ";
    }
    return out;
}        

double Linear::operator()(double t){
    cout<<"a("<<t<<") = ";
    
    double sum = 0;
    for(int i = 0; i <= degree; ++i){
        sum += (coefficients[i] * Pow(t, i));
    }

    cout<<sum<<endl;
    return sum;
}

//-----------------------------------------------------

Quadratic::Quadratic(){
    degree = 2;
}

Quadratic::Quadratic(int x_0, int x_1, int x_2){
    degree = 2;
    coefficients.push_back(x_0);
    coefficients.push_back(x_1);
    coefficients.push_back(x_2);
}

Quadratic::~Quadratic(){
    cout<<"~Quadratic"<<endl;
    coefficients.clear();
}

ostream &operator<< (ostream &out, const Quadratic &c){
    out<<"v(t) = ";
    for(int i = c.degree; i >= 0; --i){
        if(c.coefficients[i] == 0)
            continue;
        else if(i == 0)
            out<<c.coefficients[i];
        else
            out<<c.coefficients[i]<<"t^"<<i<<" + ";
    }
    return out;
}

Linear &Quadratic::Derive(){
    double temp[2];

    for(int i = 1; i <= 2; ++i){
        temp[i-1] = (coefficients[i] * i);
    }

    Linear *ret = new Linear(temp[0], temp[1]);
    ret->Allocate();
    
    return *ret;
}

double Quadratic::operator()(double t){
    cout<<"v("<<t<<") = ";
    
    double sum = 0;
    for(int i = 0; i <= degree; ++i){
        sum += (coefficients[i] * Pow(t, i));
    }

    cout<<sum<<endl;
    return sum;
}

//---------------------------------------------------

Cubic::Cubic() : Polynomial(){}

Cubic::Cubic(double w, double x, double y, double z) : Polynomial(w,x,y,z){}

Cubic::Cubic(const RowVector &r){
    for(int i = 0; i < r.size; ++i){
        coefficients.push_back(r.column[i]);
    }
}

Cubic::~Cubic(){
    cout<<"~Cubic"<<endl;
    coefficients.clear();
}

Quadratic &Cubic::Derive(){
    double temp[3];

    for(int i = 1; i <= 3; ++i){
        temp[i-1] = (coefficients[i] * i);
    }

    Quadratic *ret = new Quadratic(temp[0], temp[1], temp[2]);
    ret->Allocate();
    
    return *ret;
}

void Cubic::push_back(double num){
    if(coefficients.size() == degree + 1){
        cout<<"Error: coefficients is full"<<endl;
        return;
    }

    coefficients.push_back(num);
}

ostream &operator<< (ostream &out, const Cubic &c){
    out<<"x(t) = ";
    for(int i = c.degree; i >= 0; --i){
        if(c.coefficients[i] == 0)
            continue;
        else if(i == 0)
            out<<c.coefficients[i];
        else
            out<<c.coefficients[i]<<"t^"<<i<<" + ";
    }
    return out;
}

double Cubic::operator()(double t){
    cout<<"x("<<t<<") = ";
    
    double sum = 0;
    for(int i = 0; i <= degree; ++i){
        sum += (coefficients[i] * Pow(t, i));
    }

    cout<<sum<<endl;
    return sum;
}