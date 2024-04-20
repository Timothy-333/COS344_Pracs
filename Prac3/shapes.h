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
        Shape** shapes;
        int numShapes;

        virtual ~Shape();

        virtual void applyMatrix(Matrix*);
        virtual GLfloat* toVertexArray();
        virtual GLfloat* toColorArray();
        int accumulateShapes(int (Shape::*func)());
        virtual int numPoints();
        virtual int numVertices();
        virtual int numColors();
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
        Cylinder(vec3, double, double, vec3 = vec3(1, 0, 0), int = 100);
};

struct Cone: public Shape
{
    public:
        Cone(vec3, double, double, vec3 = vec3(1, 0, 0));
};
struct Plane: public Shape
{
    public:
        Plane(vec3);
};

#endif