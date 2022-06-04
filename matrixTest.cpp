#include<iostream>
#include "Matrix.h"
using namespace std;

int main(){
    int a[] = {9,2,3,5,-1,6,4,0,-2};

    //declaring a matrix of 3X3
    matrix<int> m(a,9, 3, 3);
    cout<<"Matrix m:\n"<<m;
    cout<<"Matrix element m[1][1]: "<<m[1][1];

    //different ways of declaring matrices 
    matrix<float> m1;
    matrix<int> m2(3,3),m3;

    //adding matrices
    m2 = m + m;
    cout<<"Addition of matrices(m2):\n"<<m2;

    //product of matrices
    m3 = m.product(m2);
    cout<<"product of matrices m & m2:\n"<<m3;

    //Transpose of matrix
    m2 = m.transpose();
    cout<<"Transpose of matrix m:\n"<<m2;

    //determinant of matrix
    int d = deter(m);
    cout<<"Determinant of Matrix m:\n"<<d<<"\n\n";

    //Inverse of matrix
    m1 = inverse(m);
    cout<<"Inverse of matrix:\n"<<m1;
    
    //removing and returning last row   
    int *as = m.popRow();
    cout<<"popped row from matrix m:\n";
    for(int i=0; i<m.getnCol(); i++)
        cout<<as[i]<<endl;
    cout<<"\n\nMatrix after popping a row:\n"<<m;

    int x[5] = {2,4,6,8,0};
    int y[5] = {5,9,13,17,1};
    //declaring a simple linear regression matrix 
    LRmatrix<int> m4(x,y,5);
    cout<<"Matrix m3 (for Simple linear regression):\n";
    m4.display();

    //regression coefficient
    Coefficient c = m4.lm();
    cout<<c;

    //prediction
    cout<<"\nPrediction for x = 10: "<<predict(10,c);
    
    return 0;
}

