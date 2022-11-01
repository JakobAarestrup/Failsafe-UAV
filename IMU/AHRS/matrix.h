
#ifndef MYMATRIX_H_
#define MYMATRIX_H_

#include <mbed.h>

class matrix
{
public:
    matrix(){};
    matrix(uint8_t,uint8_t,float);
    matrix(uint8_t);                //define n x n Unity Matrix
    matrix(uint8_t,float);          //define n x n diagonal matrix with equal entries (e*I) 
    matrix operator*(const matrix& B); 
    matrix operator *=(const matrix& B); // calculat A'*B
    matrix operator /=(const matrix& B); 
    matrix operator +(const matrix& B);
    void operator +=(const matrix& B); 
    matrix operator -(const matrix& B); 
    void scale(float);
    matrix inv_2x2(void);
    void mcopy(matrix *);
    void printout(void);
    void put_entry(uint8_t,uint8_t,float);
    void fill_row(uint8_t i,float *e);
    void fill_col(uint8_t i,float *e);
    uint8_t getI(void);
    uint8_t getJ(void);
    virtual     ~matrix();
    float** a;
    

private:
    uint8_t I,J;
};

#endif