#include <iostream>
#include <vector>
#include <string>

using namespace std;

#ifndef POLY_H
#define POLY_H

class RuntimeException{
    private:
        string message;
    public:
        RuntimeException(const string er):
            message(er){}

        string Display(){
            return message;
        }
};

class Polynomial{

    public:
        int degree;
        vector<double> coefficients;

        Polynomial(){
            degree = 3;
        }

        Polynomial(int t) {
            degree = 3;
            if(t == 0){
                coefficients.push_back(1);
                coefficients.push_back(0);
                coefficients.push_back(0);
                coefficients.push_back(0);
            }
            else{
                coefficients.push_back(1);
                coefficients.push_back(t);
                coefficients.push_back(Pow(t,2));
                coefficients.push_back(Pow(t,3));
                
            }
        }

        Polynomial(double x_0, double x_1, double x_2, double x_3){
            degree = 3;
            coefficients.push_back(x_0);
            coefficients.push_back(x_1);
            coefficients.push_back(x_2);
            coefficients.push_back(x_3);
        }

        Polynomial &Derive(){
            Polynomial *p = new Polynomial();

            p->coefficients.push_back(0);
            p->coefficients.push_back(1);
            p->coefficients.push_back(2 * coefficients[2]);
            p->coefficients.push_back(3 * coefficients[3]);

            return *p;
        }

        void DegreeCheck(int num){
            if (num < 0)
                throw RuntimeException("ERROR: Cannot have a negative degree");
        }

        double Pow(double base, int pow){
            if(pow == 0)
                return 1;
            else
                return base * Pow(base, pow - 1);
        }
        
        friend ostream &operator<< (ostream &out, const Polynomial &p){
            for(int i = p.degree ; i >= 0; --i){
                out << p.coefficients[i];
                if(i != 0){
                    out << "t^" << i  <<" + ";
                }
            }

            return out;
        }
};

#endif