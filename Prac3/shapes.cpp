#include "shapes.h"

Shape::~Shape()
{
}

void Shape::applyMatrix(Matrix *m) 
{
    if (numShapes > 0)
    {
        for (int i = 0; i < numShapes; i++)
        {
            shapes[i]->applyMatrix(m);
        }
    }
    else
    {
        for (int i = 0; i < numPoints(); i++)
        {
            vec3 temp = vec3(0, 0, 0);
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    temp[j] += (*m)[j][k] * (*vertices[i])[k];
                }
            }
            *vertices[i] = temp;
        }
    }
}
GLfloat *Shape::toVertexArray()
{
    int n = numVertices();
    GLfloat *result = new GLfloat[n];
    if (numShapes > 0)
    {
        int count = 0;
        for (int i = 0; i < numShapes; i++)
        {
            GLfloat *temp = shapes[i]->toVertexArray();
            for (int j = 0; j < shapes[i]->numVertices(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = (*vertices[i])[0];
            result[count++] = (*vertices[i])[1];
            result[count++] = (*vertices[i])[2];
        }
    }

    return result;
}
GLfloat *Shape::toColorArray()
{
    GLfloat *result = new GLfloat[numColors()];
    if (numShapes > 0)
    {
        int count = 0;
        for (int i = 0; i < numShapes; i++)
        {
            GLfloat *temp = shapes[i]->toColorArray();
            for (int j = 0; j < shapes[i]->numColors(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = colors[i][0];
            result[count++] = colors[i][1];
            result[count++] = colors[i][2];
        }
    }
    return result;
}

int Shape::accumulateShapes(int (Shape::*func)())
{
    int num = 0;
    for (int i = 0; i < numShapes; i++)
    {
        num += (shapes[i]->*func)();
    }
    return num;
}

int Shape::numPoints() 
{
    return accumulateShapes(&Shape::numPoints);
}

int Shape::numVertices()
{
    return accumulateShapes(&Shape::numVertices);
}

int Shape::numColors()
{
    return accumulateShapes(&Shape::numColors);
}

int Triangle::numVertices()
{
    return 9;
}

int Triangle::numColors()
{
    return 9;
}

int Triangle::numPoints()
{
    return 3;
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 color)
{
    numShapes = 0;
    shapes = new Shape*[0];
    vertices = new vec3*[3];
    colors = new vec3[3];
    vertices[0] = new vec3(p1);
    vertices[1] = new vec3(p2);
    vertices[2] = new vec3(p3);
    for(int i = 0; i < 3; i++)
    {
        colors[i] = color;
    }
}

Rectangle::Rectangle(vec3 tl, vec3 tr, vec3 bl, vec3 br, vec3 color)
{
    numShapes = 2;
    shapes = new Shape*[numShapes];
    shapes[0] = new Triangle(tr, tl, bl, color);
    shapes[1] = new Triangle(bl, br, tr, color);
    int n = this->numPoints();
    vertices = new vec3*[n];
    colors = new vec3[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
    int count = 0;
    for (int i = 0; i < numShapes; i++)
    {
        for (int j = 0; j < shapes[i]->numPoints(); j++)
        {
            vertices[count++] = shapes[i]->vertices[j];
        }
    }
}
Circle::Circle(vec3 center, GLfloat radius, vec3 color, int numPoints)
{
    numShapes = 1;
    shapes = new Shape*[numShapes];
    vertices = new vec3*[numPoints];
    colors = new vec3[numPoints];
    for (int i = 0; i < numPoints; i++)
    {
        GLfloat angle = 2 * M_PI * i / numPoints;
        vertices[i] = new vec3(center + vec3(radius * cos(angle), 0, radius * sin(angle)));
        colors[i] = color;
    }
    shapes[0] = new Triangle(center, *vertices[0], *vertices[1], color);
}
RectangularPrism::RectangularPrism(vec3 center, double height, double width, double length, vec3 color)
{

    vec3 topPlane = center + vec3(0, height / 2, 0);
    vec3 bottomPlane = center - vec3(0, height / 2, 0);
    vec3 leftPlane = center - vec3(width / 2, 0, 0);
    vec3 rightPlane = center + vec3(width / 2, 0, 0);
    vec3 nearPlane = center - vec3(0, 0, length / 2);
    vec3 farPlane = center + vec3(0, 0, length / 2);

    vec3 nearLeftTop = nearPlane + leftPlane + topPlane;
    vec3 nearRightTop = nearPlane + rightPlane + topPlane;
    vec3 nearLeftBottom = nearPlane + leftPlane + bottomPlane;
    vec3 nearRightBottom = nearPlane + rightPlane + bottomPlane;

    vec3 farLeftTop = farPlane + leftPlane + topPlane;
    vec3 farRightTop = farPlane + rightPlane + topPlane;
    vec3 farLeftBottom = farPlane + leftPlane + bottomPlane;
    vec3 farRightBottom = farPlane + rightPlane + bottomPlane;

    Rectangle *front = new Rectangle(nearLeftTop, nearRightTop, nearLeftBottom, nearRightBottom, color*0.1f);
    Rectangle *back = new Rectangle(farRightTop, farLeftTop, farRightBottom, farLeftBottom, color*0.2f);
    Rectangle *left = new Rectangle(farLeftTop, nearLeftTop, farLeftBottom, nearLeftBottom, color*0.3f);
    Rectangle *right = new Rectangle(nearRightTop, farRightTop, nearRightBottom, farRightBottom, color*0.4f);
    Rectangle *bottom = new Rectangle(nearLeftBottom, nearRightBottom, farLeftBottom, farRightBottom, color*0.5f);
    Rectangle *top = new Rectangle(nearLeftTop, nearRightTop, farLeftTop, farRightTop, color*0.6f);

    numShapes = 6;
    shapes = new Shape *[numShapes]
    {
        front, back, left, right, bottom, top
    };
}
Cylinder::Cylinder(vec3 center, double height, double radius, vec3 color, int numPoints)
{
    vec3 topCenter = center + vec3(0, height / 2, 0);
    vec3 bottomCenter = center - vec3(0, height / 2, 0);
    Circle *top = new Circle(topCenter, radius, color, numPoints);
    Circle *bottom = new Circle(bottomCenter, radius, color, numPoints);

    numShapes = numPoints + 2; // One shape for each rectangle, plus the top and bottom circles.
    shapes = new Shape *[numShapes];
    shapes[0] = top;
    shapes[1] = bottom;

    for (int i = 0; i < numPoints; i++)
    {
        vec3 tl = *(top->vertices[i]); // Top left vertex.
        vec3 tr = *(top->vertices[(i + 1) % numPoints]); // Top right vertex.
        vec3 bl = *(bottom->vertices[i]); // Bottom left vertex.
        vec3 br = *(bottom->vertices[(i + 1) % numPoints]); // Bottom right vertex.

        shapes[i + 2] = new Rectangle(tl, tr, bl, br, color);
    }
}
Cone::Cone(vec3 center, double height, double radius, vec3 color)
{
    vec3 bottomCenter = center - vec3(0, height / 2, 0);
    Circle *bottom = new Circle(bottomCenter, radius, color, 100);
    Rectangle *side = new Rectangle(center, bottomCenter, center + vec3(radius, 0, 0), bottomCenter + vec3(radius, 0, 0), color);
    numShapes = 2;
    shapes = new Shape *[numShapes]
    {
        bottom, side
    };
}
Plane::Plane(vec3 center)
{
}