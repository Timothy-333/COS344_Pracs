#include <iostream>
#include "Vector.h"
#include "Matrix.h"
#include <cstdlib>

void printMatrix(Matrix mat)
{
    for (int i = 0; i < mat.getN(); i++)
    {
        for (int j = 0; j < mat.getM(); j++)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int main() 
{

    double **arr = new double *[5];
    for (int i = 0; i < 5; i++)
    {
        arr[i] = new double[5];
        for (int j = 0; j < 5; j++)
        {
            arr[i][j] = std::rand()/100000000;
        }
    }
    SquareMatrix mat1(5, arr);
    printMatrix(mat1);
    std::cout << mat1.determinant() << std::endl;
    SquareMatrix mat2 = !mat1;
    printMatrix(mat2);
    std::cout << mat1.determinant() << std::endl;

    double **arr3 = new double *[1];
    arr3[0] = new double[1];
    arr3[0][0] = std::rand()/100000000;
    SquareMatrix mat5(1, arr3);
    Vector vec1(1);
    for (int i = 0; i < 1; i++)
    {
        vec1[i] = std::rand()/100000000;
    }
    vec1.print();
    printMatrix(mat5);
    Vector vec2 = mat5.solve(vec1);
    vec2.print();

    Vector vec3(5);
    for (int i = 0; i < 5; i++)
    {
        vec3[i] = std::rand()/100000000;
    }
    double **arr2 = new double *[5];
    for (int i = 0; i < 5; i++)
    {
        arr2[i] = new double[5];
        for (int j = 0; j < 5; j++)
        {
            arr2[i][j] = std::rand()/100000000;
        }
    }
    SquareMatrix mat6(5, arr2);
    printMatrix(mat6);
    std::cout << "result: " << std::endl;
    vec3.print();
    Vector vec4 = mat6.solve(vec3);
    std::cout << "Solving the system of equations" << std::endl;
    vec4.print();
    return 0;
}