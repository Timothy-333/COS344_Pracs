#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <iomanip>

#include "Vector.h"
#include "MathExceptions.h"

class Vector;

class Matrix
{
protected:
    int n;
    int m;
    double **arr;

public:
    Matrix(int, int);
    Matrix(int, int, double ** );
    Matrix(const Matrix & );
    virtual ~Matrix();
    double *&operator[](int index) const
    {
        if (index >= n || index < 0)
        {
            throw MathExceptions::InvalidIndex;
        }

        return arr[index];
    }
    Matrix operator*(const Matrix) const;
    Matrix operator*(const double) const;
    Matrix operator+(const Matrix) const;
    Matrix operator~() const;
    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                std::cout << (*this)[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    int getM() const;
    int getN() const;
};

class SquareMatrix : public Matrix
{
public:
    SquareMatrix(int );
    SquareMatrix(int , double **);
    virtual ~SquareMatrix();
    Vector solve(const Vector) const;
    double determinant() const;
    SquareMatrix operator!() const;
};

class IdentityMatrix : public SquareMatrix
{
public:
    IdentityMatrix(int _n);
    virtual ~IdentityMatrix();
};

#endif /*MATRIX_H*/