#include "Vector.h"

Vector::Vector(int size)
{
    if (size <= 0)
    {
        throw MathExceptions::InvalidVectorSize;
    }
    this->n = size;
    arr = new double[size];
    for (int j = 0; j < size; j++)
    {
        arr[j] = 0;
    }
}
Vector::Vector(int size, double* elements)
{
    if (size <= 0)
    {
        throw MathExceptions::InvalidVectorSize;
    }
    this->n = size;
    arr = elements;
}
Vector::~Vector()
{
    delete[] arr;
}
Vector::Vector(const Vector & v)
{
    this->n = v.n;
    arr = new double[n];
    for (int j = 0; j < n; j++)
    {
        arr[j] = v.arr[j];
    }
}
Vector::operator Matrix() const
{
    Matrix m = Matrix(n, 1);
    for (int i = 0; i < n; i++)
    {
        m[i][0] = arr[i];
    }
    return m;
}
int Vector::getN() const
{
    return n;
}
Vector Vector::operator+(const Vector v) const
{
    if (v.n != n)
    {
        throw MathExceptions::InvalidVectorAddition;
    }
    Vector result = Vector(n);
    for (int i = 0; i < n; i++)
    {
        result.arr[i] = arr[i] + v.arr[i];
    }
    return result;
}
Vector Vector::operator-(const Vector v) const
{
    if (v.n != n)
    {
        throw MathExceptions::InvalidVectorSize;
    }
    Vector result = Vector(n);
    for (int i = 0; i < n; i++)
    {
        result.arr[i] = arr[i] - v.arr[i];
    }
    return result;
}
Vector Vector::operator*(const double scalar) const
{
    Vector result = Vector(n);
    for (int i = 0; i < n; i++)
    {
        result.arr[i] = arr[i] * scalar;
    }
    return result;
}
double Vector::operator*(const Vector v) const
{
    if (v.n != n)
    {
        throw MathExceptions::InvalidDotProduct;
    }
    double result = 0;
    for (int i = 0; i < n; i++)
    {
        result += arr[i] * v.arr[i];
    }
    return result;
}
double Vector::magnitude() const
{
    double result = 0;
    for (int i = 0; i < n; i++)
    {
        result += arr[i] * arr[i];
    }
    return sqrt(result);
}
Vector Vector::crossProduct(const Vector v) const
{
    if (v.n != n || n != 3)
    {
        throw MathExceptions::InvalidCrossProduct;
    }
    Vector result = Vector(3);
    result.arr[0] = arr[1] * v.arr[2] - arr[2] * v.arr[1];
    result.arr[1] = -(arr[0] * v.arr[2] - arr[2] * v.arr[0]);
    result.arr[2] = arr[0] * v.arr[1] - arr[1] * v.arr[0];
    return result;
}
Vector Vector::unitVector() const
{
    double mag = magnitude();
    if (mag == 0)
    {
        throw MathExceptions::InvalidUnitVector;
    }
    return *this * (1 / mag);
}
