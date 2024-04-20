#include "Matrix.h"

Matrix::Matrix(int _n, int _m)
{
    if (_n <= 0 || _m <= 0)
    {
        throw MathExceptions::InvalidMatrixSize;
    }
    n = _n;
    m = _m;
    arr = new double *[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            arr[i][j] = 0;
        }
    }
}
Matrix::Matrix(int _n, int _m, double ** _arr)
{
    if (_n <= 0 || _m <= 0)
    {
        throw MathExceptions::InvalidMatrixSize;
    }
    n = _n;
    m = _m;
    arr = _arr;
}
Matrix::Matrix(const Matrix & _arr)
{
    n = _arr.n;
    m = _arr.m;
    arr = new double *[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            arr[i][j] = _arr.arr[i][j];
        }
    }
}
Matrix::~Matrix()
{
    if(arr == nullptr)
    {
        return;
    }
    for (int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}
int Matrix::getM() const
{
    return m;
}
int Matrix::getN() const
{
    return n;
}
SquareMatrix::SquareMatrix(int _n): Matrix(_n, _n)
{
    if (_n <= 0)
    {
        throw MathExceptions::InvalidMatrixSize;
    }
    n = _n;
    m = _n;
    arr = new double *[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            arr[i][j] = 0;
        }
    }
}
SquareMatrix::SquareMatrix(int _n, double ** _arr): Matrix(_n, _n, _arr)
{
    if (_n <= 0)
    {
        throw MathExceptions::InvalidMatrixSize;
    }
    n = _n;
    m = _n;
    arr = _arr;
}
SquareMatrix::~SquareMatrix()
{
    for (int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
    arr = nullptr;
}
IdentityMatrix::IdentityMatrix(int _n): SquareMatrix(_n)
{
    if (_n <= 0)
    {
        throw MathExceptions::InvalidMatrixSize;
    }
    n = _n;
    m = _n;
    arr = new double *[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            arr[i][j] = 0;
        }
        arr[i][i] = 1;
    }
}
IdentityMatrix::~IdentityMatrix()
{
    for (int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}
Matrix Matrix::operator*(const Matrix _arr) const
{
    if (m != _arr.n)
    {
        throw MathExceptions::InvalidMatrixMultiplication;
    }
    Matrix* result = new Matrix(n, _arr.m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < _arr.m; j++)
        {
            for (int k = 0; k < m; k++)
            {
                result->arr[i][j] += arr[i][k] * _arr.arr[k][j];
            }
        }
    }
    return *result;
}
Matrix Matrix::operator*(const double scalar) const
{
    Matrix* result = new Matrix(n, m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            result->arr[i][j] = arr[i][j] * scalar;
        }
    }
    return *result;
}
Matrix Matrix::operator+(const Matrix _arr) const
{
    if (n != _arr.n || m != _arr.m)
    {
        throw MathExceptions::InvalidMatrixAddition;
    }
    Matrix* result = new Matrix(n, m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            result->arr[i][j] = arr[i][j] + _arr.arr[i][j];
        }
    }
    return *result;
}
Matrix Matrix::operator~() const
{
    Matrix* result = new Matrix(m, n);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result->arr[i][j] = arr[j][i];
        }
    }
    return *result;
}
double SquareMatrix::determinant() const
{
    double det = 0;
    int sign = 1;
    if(n == 1)
    {
        det = arr[0][0];
    }
    else if(n == 2)
    {
        det = arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0];
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            SquareMatrix* temp = new SquareMatrix(n - 1);
            for(int j = 1; j < n; j++)
            {
                for(int k = 0; k < n; k++)
                {
                    if(k < i)
                    {
                        temp->arr[j - 1][k] = arr[j][k];
                    }
                    else if(k > i)
                    {
                        temp->arr[j - 1][k - 1] = arr[j][k];
                    }
                }
            }
            det += sign * arr[0][i] * temp->determinant();
            sign = -sign;
            delete temp;
        }
    }
    return det;
}
SquareMatrix SquareMatrix::operator!() const
{
    SquareMatrix* resultMatrix = new SquareMatrix(n);
    int sign = 1;
    double det = determinant();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            SquareMatrix* temp = new SquareMatrix(n - 1);
            for (int k = 0; k < n; k++)
            {
                for (int l = 0; l < n; l++)
                {
                    if (k < i && l < j)
                    {
                        temp->arr[k][l] = arr[k][l];
                    }
                    else if (k < i && l > j)
                    {
                        temp->arr[k][l - 1] = arr[k][l];
                    }
                    else if (k > i && l < j)
                    {
                        temp->arr[k - 1][l] = arr[k][l];
                    }
                    else if (k > i && l > j)
                    {
                        temp->arr[k - 1][l - 1] = arr[k][l];
                    }
                }
            }
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            resultMatrix->arr[j][i] = (sign * temp->determinant()) / det;
            delete temp;
        }
    }
    return *(resultMatrix);
}
Vector SquareMatrix::solve(const Vector _arr) const
{
    double det = determinant();
    if ((int) det == 0)
    {
        throw MathExceptions::UnsolvableSystemOfLinearEquations;
    }
    if (_arr.getN() == 1)
    {
        Vector result = Vector(1);
        result[0] = _arr[0] / arr[0][0];
        return result;
    }
    Matrix matrixResult = !(*this) * _arr;
    Vector result = Vector(n);
    for (int i = 0; i < n; i++)
    {
        result[i] = matrixResult[i][0];
    }
    return result;
}
