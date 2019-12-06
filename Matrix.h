#include <iostream>

using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
    private:
        int size;
        double** matrix;

        
        void CopyMatrix(const Matrix &m){
            if(size != m.size){
                cout<<"Could not copy matrix. Dimensions are wrong"<<endl;
                return;
            }

            CopyMatrix(m.matrix);
        }

        void CopyMatrix(double** m){
            for(int i = 0; i < size; ++i){
                for(int j = 0; j < size; ++j){
                    cout<<"This matrix["<<i<<" , " <<j<<"] = " <<  matrix[i][j] << endl;
                    cout<<"That matrix["<<i<<" , " <<j<<"] = " <<  m[i][j] << endl;
                    matrix[i][j] = m[i][j];
                    cout<<"Now this matrix["<<i<<" , " <<j<<"] = " <<  matrix[i][j] << endl << endl;
                }
            }
        }

    
    public:
    //-----------------CONSTRUCTORS/DESTRUCTORS-----------------------------------------------
        Matrix(){
            size = 0;
            matrix = NULL;
        }

        Matrix(int n):
            size(n){
                matrix = NULL;
                SetUpMatrix();
        }
        
        Matrix(const Matrix &m){
            size = m.size;
            
            matrix = new double* [size];
            for(int i = 0; i < size; ++i){
                matrix[i] = new double [size];
                for(int j = 0; j < size; ++j){
                    matrix[i][j] = m.matrix[i][j];
                }
            }
        }

        ~Matrix(){
            for(int i = 0; i < size; ++i){
                delete[] matrix[i];
            }
            delete[] matrix;
        }

    //-----------------UTILITIES-------------------------------------------------
        //Creates a matrix excluding the x row and y column
        Matrix &SubMatrix(int x, int y){
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
        double Determinant(){
            if(size == 2){
                return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
            }
            
            double sum = 0;

            for(int i = 0; i < size; ++i){
                Matrix s = SubMatrix(0,i);

                if( i%2 == 0){
                    sum += s.Determinant();    
                }
                else{
                    sum -= s.Determinant();
                }
            }

            return sum;
        }

        void ScanInValues(){
            for(int i = 0; i < size; ++i){
                for(int j = 0; j < size; ++j){
                    cout<<"matrix["<<i<<" , " <<j<<"] = ";
                    cin>>matrix[i][j];
                }
            }
        }
        
        void SetUpMatrix(){
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
        //------------OPERATORS-------------------------------------------------
        Matrix &operator= (const Matrix& m){
            cout<<"ASSIGNMENT"<<endl;
            size = m.size;
            CopyMatrix(m);
            return *this;
        }

        friend Matrix &operator*(const Matrix &left, const Matrix &right){
            cout<<"MULTIPLYING"<<endl;
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

        friend Matrix &operator+(Matrix const &left, Matrix const &right){
            cout<<"ADDING"<<endl;
            Matrix *ret = new Matrix(left.size);
            
            if(left.size != right.size){
                cout<<"ERROR: matrices sizes are not compatible. Did not multiply"<<endl;
                return *ret;
            }

            cout << "left: \n" << left << endl;
            cout << "right: \n" << right << endl;

            for(int i = 0; i < left.size; ++i){
                for(int j = 0; j < left.size; ++j){
                    cout<<"matrix["<<i<<" , " <<j<<"] = " <<  left.matrix[i][j] <<"\n";
                    cout<<"m.matrix["<<i<<" , " <<j<<"] = " <<  right.matrix[i][j] <<"\n";
                    ret->matrix[i][j] = left.matrix[i][j] + right.matrix[i][j];
                    cout<<"matrix["<<i<<" , " <<j<<"] = " <<  ret->matrix[i][j] <<"\n\n";
                }
            }
            cout << endl << endl;
            return *ret;
        }

        friend ostream &operator<<(ostream &out, const Matrix &m){
            for(int i = 0; i< m.size ; ++i){
                out<<"[";
                for(int j = 0; j < m.size; ++j){
                    out<<m.matrix[i][j]<<", ";
                }
                out<<"]"<<endl;
            }
            return out;
        }
};
#endif