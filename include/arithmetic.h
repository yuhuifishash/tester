#ifndef FLOAT_H
#define FLOAT_H
#include<cmath>
const double PI = acos(-1.0);
class Complex
{
public:
    double Re;
    double Im;
    Complex(double a=0,double b=0){
        Re=a;Im=b;
    }
    Complex operator-(){
        return Complex(-Re,-Im);
    }
    Complex operator+(const Complex& b){
        return Complex(Re+b.Re,Im+b.Im);
    }
    Complex operator-(const Complex& b){
        return Complex(Re-b.Re,Im-b.Im);
    }
    Complex operator*(const Complex& b){
        double A=Re*b.Re-Im*b.Im;
        double B=Im*b.Re+Re*b.Im;
        return Complex(A,B);
    }
};
const double fun_a[4] = {-3.7,-1.8,-4.1,-1.8};
const double fun_b[4] = {1,1,1,1};//  1/(1+e^(a+bx))
double matrix_mul();
double conv_2D();
double Simpson_Integral();
double FFT();
double get_score(int id,double T);
char get_rank(double score);
#endif