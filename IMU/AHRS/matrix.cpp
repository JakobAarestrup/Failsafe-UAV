#include "matrix.h"
using namespace std;

matrix::matrix(uint8_t I,uint8_t J,float e)
{
    this->I = I;
    this->J = J;
    a = (float **)malloc(I * sizeof(float *)); 
    for (uint8_t i=0; i<I; i++) 
         a[i] = (float *)malloc(J * sizeof(float)); 
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<J;j++)
            a[i][j]=e;//;*/
    }
// create Unity matrix
matrix::matrix(uint8_t IJ)
{
    this->I = IJ;
    this->J = IJ;
    a = (float **)malloc(IJ * sizeof(float *)); 
    for (uint8_t i=0; i<IJ; i++) 
         a[i] = (float *)malloc(IJ * sizeof(float)); 
    for(uint8_t i=0;i<IJ;i++)
        for(uint8_t j=0;j<IJ;j++)
            a[i][j]=0.0;//;*/
    for(uint8_t i=0;i<IJ;i++)
            a[i][i]=1.0;//;*/
    }
// create diagonal matrix with Entry e
matrix::matrix(uint8_t IJ,float e)
{
    this->I = IJ;
    this->J = IJ;
    a = (float **)malloc(IJ * sizeof(float *)); 
    for (uint8_t i=0; i<IJ; i++) 
         a[i] = (float *)malloc(IJ * sizeof(float)); 
    for(uint8_t i=0;i<IJ;i++)
        for(uint8_t j=0;j<IJ;j++)
            a[i][j]=0.0;//;*/
    for(uint8_t i=0;i<IJ;i++)
            a[i][i]=e;//;*/
    }

// display matrix on screen
void matrix::printout(void)
{
    for(uint8_t i=0;i<I;i++)
        {
        for(uint8_t j=0;j<J;j++)
            printf("%.8e ",a[i][j]);
        printf("..\r\n");
        }
    }

matrix::~matrix(void) {
    for (uint8_t i=0; i<I; i++) 
        delete a[i];
    delete [] a;
//    printf("Matrix is being deleted\r\n");
    }
uint8_t matrix::getI(void)
    {return I;}
uint8_t matrix::getJ(void)
    {return J;}


matrix matrix::operator*(const matrix& B) {
    if (J != B.I) {
        printf("Matrices shapes mismatch");
    }
    matrix C(I, B.J,0.0);
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<B.J;j++)
            for(uint8_t k=0;k<J;k++)
                C.a[i][j]+=(a[i][k]*B.a[k][j]);
    return C;
}
// calculate A'*B 
matrix matrix::operator *=(const matrix& B) {
    if (I != B.I) {
        printf("Matrices shapes mismatch");
    }
    matrix C(J, B.J,0.0);
    for(uint8_t j1=0;j1<J;j1++)
        for(uint8_t j=0;j<B.J;j++)
            for(uint8_t k=0;k<I;k++)
                C.a[j1][j]+=(a[k][j1]*B.a[k][j]);
    return C;
}
// calculate A*B' 
matrix matrix::operator /=(const matrix& B) {
    if (J != B.J) {
        printf("Matrices shapes mismatch");
    }
    matrix C(I, B.I,0.0);
    for(uint8_t i=0;i<I;i++)
        for(uint8_t i1=0;i1<B.I;i1++)
            for(uint8_t k=0;k<J;k++)
                C.a[i][i1]+=(a[i][k]*B.a[i1][k]);
    return C;
}
// calc Matrix C=A+B
matrix matrix::operator+(const matrix& B) {
    matrix C(I, J,0.0);
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<B.J;j++)
            C.a[i][j]=(a[i][j]+B.a[i][j]);
    return C;
}
// calc Matrix A=A+B
void matrix::operator+=(const matrix& B) {
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<B.J;j++)
            a[i][j]+=B.a[i][j];
}
// calc Matrix C=A-B
matrix matrix::operator-(const matrix& B) {
    matrix C(I, J,0.0);
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<B.J;j++)
            C.a[i][j]=(a[i][j]-B.a[i][j]);
    return C;
}

// calc Matrix inv(A), A must be 2x2
matrix matrix::inv_2x2(void) {
    matrix C(2, 2,0.0);
    float idet=1/(a[0][0]*a[1][1]-a[0][1]*a[1][0]);
    C.a[0][0] = idet * a[1][1];
    C.a[0][1] = -idet * a[0][1];
    C.a[1][0] = -idet * a[1][0];
    C.a[1][1] = idet * a[0][0];
    return C;
}
void matrix::scale(float sc)
{
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<J;j++)
            a[i][j]*=sc;
    }
void matrix::put_entry(uint8_t i,uint8_t j,float e)
{
    a[i][j]=e;
    }
// Fill Column
void matrix::fill_row(uint8_t i,float *e)
{
    for(uint8_t j=0;j<J;j++)
        a[i][j]=e[j];
    }
    
// Fill Row
void matrix::fill_col(uint8_t j,float *e)
{
    for(uint8_t i=0;i<I;i++)
        a[i][j]=e[i];
    }
// Copy matrix
void matrix::mcopy(matrix *B)
{
    for(uint8_t i=0;i<I;i++)
        for(uint8_t j=0;j<J;j++)
            a[i][j]=B->a[i][j];
    
    }