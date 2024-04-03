#include "shapes.h"

Shape::~Shape()
{
}

void Shape::applyMatrix(mat3x3 m) 
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
            (*vertices[i]) = m * vec3((*vertices[i]), 1);
        }
    }
}
GLfloat *Shape::toVertexArray()
{
    int n = numVertices();
    GLfloat *result = new GLfloat[numVertices()];
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
        for (int i = 0; i < numVertices() / 2; i++)
        {
            result[count++] = (*vertices[i])[0];
            result[count++] = (*vertices[i])[1];
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
        for (int i = 0; i < numVertices() / 2; i++)
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
Triangle::Triangle(vec2 p1, vec2 p2, vec2 p3, vec3 color)
{
    numShapes = 0;
    shapes = new Shape*[0];
    vertices = new vec2*[3];
    colors = new vec3[3];
    vertices[0] = new vec2(p1);
    vertices[1] = new vec2(p2);
    vertices[2] = new vec2(p3);
    for(int i = 0; i < 3; i++)
    {
        colors[i] = color;
    }
}

int Triangle::numVertices()
{
    return 6;
}

int Triangle::numColors()
{
    return 9;
}

int Triangle::numPoints()
{
    return 3;
}

// GLfloat* Triangle::toVertexArray()
// {
//     GLfloat *out = new GLfloat[6];
//     int count = 0;
//     for(int i = 0; i < 3; i++)
//     {
//         out[count++] = (*vertices[i])[0];
//         out[count++] = (*vertices[i])[1];
//     }
//     return out;
// }
// GLfloat* Triangle::toColorArray()
// {
//     GLfloat *out = new GLfloat[9];
//     int count = 0;
//     for(int i = 0; i < 3; i++)
//     {
//         out[count++] = (*vertices[i])[0];
//         out[count++] = (*vertices[i])[1];
//         out[count++] = (*vertices[i])[2];
//     }
//     return out;
// }

Rectangle::Rectangle(vec2 tl, vec2 tr, vec2 bl, vec2 br, vec3 color)
{
    numShapes = 2;
    shapes = new Shape*[numShapes];
    shapes[0] = new Triangle(tl, tr, bl, color);
    shapes[1] = new Triangle(tr, bl, br, color);
    int n = this->numPoints();
    vertices = new vec2*[n];
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
Circle::Circle(vec2 center, GLfloat radius, vec3 color, int numSegments)
{
    numShapes = numSegments;
    shapes = new Shape*[numShapes];
    vertices = new vec2*[numSegments];
    colors = new vec3[numSegments];
    this->center = center;
    for (int i = 0; i < numSegments; i++)
    {
        GLfloat angle = 2 * M_PI * i / numSegments;
        vertices[i] = new vec2(center[0] + radius * cos(angle), center[1] + radius * sin(angle));
        colors[i] = color;
    }
    for (int i = 0; i < numSegments; i++)
    {
        // Create a triangle for each segment
        shapes[i] = new Triangle(center, *vertices[i], *vertices[(i+1)%numSegments], color);
    }
}
Car::Car()
{
    numShapes = 10;
    shapes = new Shape *[numShapes];

    // Body of the car
    shapes[0] = new Rectangle(
        vec2(-0.8, -0.2),
        vec2(0.8, -0.2),
        vec2(-0.8, -0.6),
        vec2(0.8, -0.6),
        vec3(1, 0, 0)); // Red color

    // Left wheel of the car
    shapes[1] = new Circle(
        vec2(-0.5, -0.6), // Center
        0.2, // Radius
        vec3(0, 0, 0), // Black color
        50); // Number of segments

    // Right wheel of the car
    shapes[2] = new Circle(
        vec2(0.5, -0.6), // Center
        0.2, // Radius
        vec3(0, 0, 0), // Black color
        50); // Number of segments

    // Roof of the car
    shapes[3] = new Rectangle(
        vec2(-0.4, 0.0),
        vec2(0.4, 0.0),
        vec2(-0.4, -0.2),
        vec2(0.4, -0.2),
        vec3(0.5, 0.5, 0.5)); // Grey color

    //front roof corner
    shapes[4] = new Triangle(
        vec2(-0.4, 0.0),
        vec2(-0.8, -0.2),
        vec2(-0.4, -0.2),
        vec3(0.5, 0.5, 0.5)); // Grey color

    //front corner window
    shapes[5] = new Triangle(
        vec2(-0.4 + 0.02, 0.0 - 0.02), // Top left
        vec2(-0.8 + 0.08, -0.2 + 0.02), // Bottom left
        vec2(-0.4 + 0.02, -0.2 + 0.02), // Bottom right
        vec3(0, 1, 1)); // Cyan color

    // Front window of the car
    shapes[6] = new Rectangle(
        vec2(-0.4 + 0.02, 0.0 - 0.02), // Top left
        vec2(0.0 - 0.02, 0.0 - 0.02), // Top right
        vec2(-0.4 + 0.02, -0.2 + 0.02), // Bottom left
        vec2(0.0 - 0.02, -0.2 + 0.02), // Bottom right
        vec3(0, 1, 1)); // Cyan color

    //back window of the car
    shapes[7] = new Rectangle(
        vec2(0.0 + 0.02, 0.0 - 0.02), // Top left
        vec2(0.4 - 0.02, 0.0 - 0.02), // Top right
        vec2(0.0 + 0.02, -0.2 + 0.02), // Bottom left
        vec2(0.4 - 0.02, -0.2 + 0.02), // Bottom right
        vec3(0, 1, 1)); // Cyan color
    // Left rim of the car
    shapes[8] = new Circle(
        vec2(-0.5, -0.6), // Center
        0.1, // Radius
        vec3(1, 1, 1), // White color
        10); // Number of segments

    // Right rim of the car
    shapes[9] = new Circle(
        vec2(0.5, -0.6), // Center
        0.1, // Radius
        vec3(1, 1, 1), // White color
        10); // Number of segments
        
    //front light
    shapes[10] = new Rectangle(
        vec2(-0.8, -0.2),
        vec2(-0.7, -0.2),
        vec2(-0.8, -0.3),
        vec2(-0.7, -0.3),
        vec3(1, 1, 0)); // Yellow color
    
    wheels[0] = static_cast<Circle *>(shapes[1]);
    wheels[1] = static_cast<Circle *>(shapes[2]);
    wheels[2] = static_cast<Circle *>(shapes[8]);
    wheels[3] = static_cast<Circle *>(shapes[9]);
}
void Car::applyMatrix(mat3x3 matrix)
{
    Shape::applyMatrix(matrix);

    // Update the center of each wheel
    for (int i = 0; i < 4; i++) {
        vec3 newCenter = matrix * vec3(wheels[i]->center, 1);
        wheels[i]->center = vec2(newCenter.x, newCenter.y);
    }
}
Road::Road()
{
    numShapes = 11;
    shapes = new Shape *[numShapes];

    // Road
    shapes[0] = new Rectangle(
        vec2(-1.0, 0.5), // Top left
        vec2(1.0, 0.5), // Top right
        vec2(-1.0, -1.0), // Bottom left
        vec2(1.0, -1.0), // Bottom right
        vec3(0.1,0.1,0.1)); // grey color

    // Stripes
    for (int i = 1; i < numShapes; i++)
    {
        float x = -1.0 + i * 0.2;
        shapes[i] = new Rectangle(
            vec2(x, -0.05), // Top left
            vec2(x + 0.1, -0.05), // Top right
            vec2(x, -0.15), // Bottom left
            vec2(x + 0.1, -0.15), // Bottom right
            vec3(1, 1, 1)); // White color
    }
}
