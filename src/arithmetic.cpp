#include"arithmetic.h"
#include<cmath>
#include<iostream>
#include<vector>
#include<chrono>
using namespace std;
double matrix_mul()
{
    int n = 512;
    double* a = new double[n*n];
    double* b = new double[n*n];
    double* c = new double[n*n];
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            a[i*n+j] = rand();
            b[i*n+j] = rand();
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j){
            for(int k=0;k<n;++k){
                c[i*n+j] += a[i*n+k]*b[k*n+j];
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish -start;

    delete []a;
    delete []b;
    delete []c;

    return elapsed.count();
}
double conv_2D()
{
    int n = 1024, r = 5;
    double* a = new double[n*n];
    double* ans = new double[n*n];
    double* R = new double[r*r];

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=r/2;i<n-r/2;++i){
        for(int j=r/2;j<n-r/2;++j){
            for(int s=0;s<r;++s){
                for(int t=0;t<r;++t){
                    ans[(i-r/2)*n+j-r/2] += a[(s+i-r/2)*n+t+i-r/2]*R[s*r+t];  
                }
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish -start;

    delete []a;
    delete []ans;
    delete []R;

    return elapsed.count();
}
double f(double x)
{
	return log(x)*sin(x)*x;
}
double Simpson_Integral()
{
    double l = 1,r = 10;
    int n = 10000000;
    double dx = (r - l)/ n;
    double S = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=1;i<=n;++i){
        double left = l + (i-1)*dx;
        double right = l + i*dx;
        double mid = (left + right)/2;
        S += (right - left)*(f(left)+f(right)+4*f(mid))/6;
    }
    auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish -start;
    
    return elapsed.count();
}
double FFT()
{
    int l=0,limit=1;
    int n = (1<<20)-2;
    Complex* A = new Complex[2*n+100000];
    int* r = new int[2*n+100000];
    for(int i=0;i<=n;++i){
        A[i].Re = rand();
    }

    auto start = std::chrono::high_resolution_clock::now();
    while(limit<=n){
        limit<<=1;
        ++l;
    }
    for(int i=0;i<limit;++i){
        r[i]=(r[i>>1]>>1)|((i&1)<<(l-1));
    }
    for(int i=0;i<limit;++i){
        if(i<r[i]){
            swap(A[i],A[r[i]]);
        }
    }
    for(int mid=1;mid<limit;mid<<=1){
        Complex Wn(cos(PI/mid),sin(PI/mid));
        for(int R=mid<<1,j=0;j<limit;j+=R){
            Complex w(1,0);
            for(int k=0;k<mid;++k,w=w*Wn){
                Complex x=A[j+k],y=w*A[j+mid+k];
                A[j+k]=x+y;
                A[j+mid+k]=x-y;
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish -start;
    
    delete []A;
    delete []r;

    return elapsed.count();
}
double get_score(int id,double T)
{
    return 100.0/(1+pow(2.718281828,fun_a[id]+fun_b[id]*T));
}
char get_rank(double score)
{
    if(score>=90.0)return 'S';
	if(score>=80.0)return 'A';
	if(score>=70.0)return 'B';
	if(score>=60.0)return 'C';
	return 'D';
}