#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"
#include "MathExceptions.h"

class Matrix;

class Vector
{
private:
    int n;       // size of the vector
    double *arr; // array containing the elements in the vector

public:
    Vector(int);
    Vector(int, double*);
    ~Vector();
    Vector(const Vector &);
    Vector operator+(const Vector) const;
    Vector operator-(const Vector) const;
    Vector operator*(const double) const;
    double operator*(const Vector) const;
    double &operator[](int index) const
    {
        if (index < 0 || index >= n)
        {
            throw MathExceptions::InvalidIndex;
        }
        return arr[index];
    }
    double magnitude() const;
    operator Matrix() const;
    Vector crossProduct(const Vector) const;
    Vector unitVector() const;
    int getN() const;
    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            std::cout<< arr[i] << "\n";
        }
    };
};

#endif /*VECTOR_H*/