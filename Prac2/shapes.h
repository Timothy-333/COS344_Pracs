#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

struct Shape
{
    public:
        vec2** vertices;
        vec3* colors;
        Shape** shapes;
        int numShapes;

        virtual ~Shape();

        virtual void applyMatrix(mat3x3);
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
        Triangle(vec2, vec2, vec2, vec3);
        int numVertices() override;
        int numColors() override;
        int numPoints() override;
};

struct Rectangle: public Shape
{
    public:
        Rectangle(vec2, vec2, vec2, vec2, vec3);
};
struct Circle: public Shape
{
    public:
        vec2 center;
        Circle(vec2, GLfloat, vec3, int);
};
struct Car: public Shape
{
    public:
        Circle* wheels[4];
        void applyMatrix(mat3x3);
        Car();
};
struct Road: public Shape
{
    public:
        Road();
};

#endif