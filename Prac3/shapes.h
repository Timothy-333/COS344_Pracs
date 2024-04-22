#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Matrix.h"

using namespace glm;
using namespace std;

struct Shape
{
    public:
        vec3** vertices;
        vec3* colors;
        vec3 center;
        Shape** shapes;
        int numShapes;
        double **transformationMatrix = new double*[4];
        // SquareMatrix* rotationalMatrix;

        virtual ~Shape();
        Shape();
        virtual void applyMatrix(Matrix*);
        virtual GLfloat* toVertexArray();
        virtual GLfloat* toColorArray();
        int accumulateShapes(int (Shape::*func)());
        virtual int numPoints();
        virtual int numVertices();
        virtual int numColors();
        virtual void rotate(double, int);
        virtual void translate(vec3);
        virtual void resetMatrix();
        virtual void accelerate(double);
        virtual void fly();
};

struct Triangle: public Shape
{
    public:
        Triangle(vec3, vec3, vec3, vec3 = vec3(1, 0, 0));
        int numVertices() override;
        int numColors() override;
        int numPoints() override;
};
struct Circle: public Shape
{
    public:
        vec3 center;
        GLfloat radius;
        Circle(vec3, GLfloat, vec3, int = 100);
};
struct Rectangle: public Shape
{
    public:
        Rectangle(vec3, vec3, vec3, vec3, vec3 = vec3(1, 0, 0));
};

struct RectangularPrism: public Shape
{
    public:
        RectangularPrism(vec3, double, double, double, vec3 = vec3(1, 0, 0));
};
struct Cylinder: public Shape
{
    public:
        Cylinder(vec3, double, double, vec3 = vec3(1, 0, 0), int = 8);
};
struct Cone: public Shape
{
    public:
        Cone(vec3, double, double, vec3 = vec3(1, 0, 0), int = 8);
};
struct TriangularPrism: public Shape
{
    public:
        TriangularPrism(vec3, vec3, vec3, double, vec3 = vec3(1, 0, 0));
};
struct Tail: public Shape
{
    public:
        Tail(vec3, vec3 = vec3(1, 0, 0));
};
struct Wheel: public Shape
{
    public:
        Wheel(vec3, double, vec3 = vec3(1, 0, 0), int = 8);
};
struct EngineNose: public Shape
{
    public:
        EngineNose(vec3, double, double, vec3 = vec3(1, 0, 0));
};
struct Engine: public Shape
{
    public:
        Engine(vec3, vec3 = vec3(1, 0, 0));
};
struct Plane: public Shape
{
    public:
        Engine** engines = new Engine*[4];
        double speed;
        Plane(vec3, vec3);
        void accelerate(double);
        void fly();
};

#endif