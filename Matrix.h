#include <iostream>

#include "Polynomial.h"

using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

class RowVector;

class Linear : public Polynomial{
    public:
        Linear();
        Linear(int x_0, int x_1);

        ~Linear();

        friend ostream &operator<< (ostream &out, const Linear &c);       

        double operator()(double t);
};

class Quadratic : public Polynomial{
    public:
        Quadratic();

        Quadratic(int x_0, int x_1, int x_2);

        ~Quadratic();

        friend ostream &operator<< (ostream &out, const Quadratic &c);

        Linear &Derive();

        double operator()(double t);
};

class Cubic : public Polynomial{
    public:
        Cubic();
        
        Cubic(double w, double x, double y, double z);

        Cubic(const RowVector &r);

        ~Cubic();

        Quadratic &Derive();

        void push_back(double num);

        friend ostream &operator<< (ostream &out, const Cubic &c);

        double operator()(double t);
};

class Matrix{
    protected:
        int size;
        double** matrix;

        
        void CopyMatrix(const Matrix &m);

        void CopyMatrix(double** m);

    
    public:
    //-----------------CONSTRUCTORS/DESTRUCTORS-----------------------------------------------
        Matrix();

        Matrix(int n);
        
        Matrix(const Matrix &m);

        Matrix(Polynomial &x_0, Polynomial &v_0, Polynomial &x_1, Polynomial &v_1);

        Matrix(char i);

        ~Matrix();

    //-----------------UTILITIES-------------------------------------------------
        //Creates a matrix excluding the x row and y column
        Matrix &SubMatrix(int x, int y);

        //Gets the determinant of the matrix
        double Determinant();

        void ScanInValues();
        
        void SetUpMatrix();

        double At(int i, int j);

        Matrix &Coffactor();

        Matrix &Adjugate();
        //------------OPERATORS-------------------------------------------------
        Matrix &operator= (const Matrix& m);

        friend Matrix &operator*(const Matrix &left, const Matrix &right);

        friend Matrix &operator* (const double left, const Matrix &right);

        friend Matrix &operator+(Matrix const &left, Matrix const &right);

        friend ostream &operator<<(ostream &out, const Matrix &m);
};

class RowVector: public Matrix{
    public:
        double *column;
        int size = 4;

        RowVector();

        RowVector(const RowVector &r);

        ~RowVector();

        void CopyColumn(const RowVector &r);

        void ScanInValues();

        RowVector& operator= (RowVector &right);

        bool operator== (const RowVector &right);

        friend RowVector &operator*  (Matrix &m, const RowVector &r);

        friend ostream &operator<< (ostream &out, const RowVector &r);

        Cubic &CreateCubic();
};
#endif