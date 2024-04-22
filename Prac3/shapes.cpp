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
            // Convert to homogeneous coordinates
            Matrix homogenousVertex = Matrix(4, 1);
            for (int j = 0; j < 3; j++)
            {
                homogenousVertex[j][0] = (*vertices[i])[j];
            }
            homogenousVertex[3][0] = 1.0;

            // Perform the matrix multiplication
            Matrix transformedVertex = (*m) * homogenousVertex;

            // Convert back to 3D coordinates
            for (int j = 0; j < 3; j++)
            {
                (*vertices[i])[j] = transformedVertex[j][0] / transformedVertex[3][0];
            }
        }
    }
}
void Shape::accelerate(double amount)
{
    return;
}
void Shape::fly()
{
    return;
}
void Shape::resetMatrix()
{
    if (transformationMatrix == NULL)
    {
        transformationMatrix = new double *[4];
    }
    for (int i = 0; i < 4; i++)
    {
        if (transformationMatrix[i] == NULL)
        {
            transformationMatrix[i] = new double[4];
        }
        for (int j = 0; j < 4; j++)
        {
            transformationMatrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}
void Shape::rotate(double amount, int direction)
{
    double angle = amount;
    if (angle == 0)
    {
        return;
    }

    // Translate the shape to the origin
    resetMatrix();
    transformationMatrix[0][3] = -center.x;
    transformationMatrix[1][3] = -center.y;
    transformationMatrix[2][3] = -center.z;
    Matrix* matrix = new Matrix(4, 4, transformationMatrix);
    applyMatrix(matrix);

    // Rotate the shape
    resetMatrix();
    switch (direction)
    {
    case 0: // Rotate about x-axis in a clockwise direction.
        transformationMatrix[1][1] = cos(-angle);
        transformationMatrix[1][2] = sin(-angle);
        transformationMatrix[2][1] = -sin(-angle);
        transformationMatrix[2][2] = cos(-angle);
        break;
    case 1: // Rotate about y-axis in a clockwise direction.
        transformationMatrix[0][0] = cos(-angle);
        transformationMatrix[0][2] = -sin(-angle);
        transformationMatrix[2][0] = sin(-angle);
        transformationMatrix[2][2] = cos(-angle);
        break;
    case 2: // Rotate about z-axis in a clockwise direction.
        transformationMatrix[0][0] = cos(-angle);
        transformationMatrix[0][1] = sin(-angle);
        transformationMatrix[1][0] = -sin(-angle);
        transformationMatrix[1][1] = cos(-angle);
        break;
    default:
        return;
    }
    matrix = new Matrix(4, 4, transformationMatrix);
    applyMatrix(matrix);

    // Translate the shape back to its original position
    resetMatrix();
    transformationMatrix[0][3] = center.x;
    transformationMatrix[1][3] = center.y;
    transformationMatrix[2][3] = center.z;
    matrix = new Matrix(4, 4, transformationMatrix);
    applyMatrix(matrix);
}
void Shape::translate(vec3 amount)
{
    resetMatrix();
    transformationMatrix[0][3] = amount.x;
    transformationMatrix[1][3] = amount.y;
    transformationMatrix[2][3] = amount.z;
    Matrix* matrix = new Matrix(4, 4, transformationMatrix);
    applyMatrix(matrix);
}
void Plane::accelerate(double amount)
{
    speed += amount;
    if (speed < 0)
    {
        speed = 0;
    }
}
void Plane::fly()
{
    for (int i = 0; i < 4; i++)
    {
        engines[i]->shapes[0]->rotate(speed, 1);
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
Circle::Circle(vec3 center, GLfloat radius, vec3 color, int numSegments)
{
    this->center = center;
    numShapes = numSegments;
    shapes = new Shape*[numShapes];
    vertices = new vec3*[numSegments];
    colors = new vec3[numSegments];
    this->radius = radius;
    this->center = center;
    for (int i = 0; i < numSegments; i++)
    {
        GLfloat angle = 2 * M_PI * i / numSegments;
        vertices[i] = new vec3(center + vec3(radius * cos(angle), 0, radius * sin(angle)));
        colors[i] = color;
    }
    for (int i = 0; i < numSegments; i++)
    {
        // Create a triangle for each segment
        shapes[i] = new Triangle(center, *vertices[i], *vertices[(i+1)%numSegments], color);
    }
}
RectangularPrism::RectangularPrism(vec3 center, double height, double width, double length, vec3 color)
{
    this->center = center;
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
    this->center = center;
    vec3 topCenter = center + vec3(0, height / 2, 0);
    vec3 bottomCenter = center - vec3(0, height / 2, 0);
    Circle *top = new Circle(topCenter, radius, color * 0.5f, numPoints);
    Circle *bottom = new Circle(bottomCenter, radius, color, numPoints);

    numShapes = numPoints + 2; // One shape for each rectangle, plus the top and bottom circles.
    shapes = new Shape *[numShapes]
    {
        bottom, top
    };

    for (int i = 0; i < numPoints; i++)
    {
        vec3 tl = *(top->vertices[i]); // Top left vertex.
        vec3 tr = *(top->vertices[(i + 1) % numPoints]); // Top right vertex.
        vec3 bl = *(bottom->vertices[i]); // Bottom left vertex.
        vec3 br = *(bottom->vertices[(i + 1) % numPoints]); // Bottom right vertex.

        shapes[i + 2] = new Rectangle(tl, bl, tr, br, color * 0.7f);
    }
}
Cone::Cone(vec3 center, double height, double radius, vec3 color, int numPoints)
{
    this->center = center;
    vec3 bottomCenter = center - vec3(0, height / 2, 0);
    Circle *bottom = new Circle(bottomCenter, radius, color, numPoints);

    numShapes = numPoints + 1; // One shape for each triangle, plus the bottom circle.
    shapes = new Shape *[numShapes];
    shapes[0] = bottom;

    for (int i = 0; i < numPoints; i++)
    {
        vec3 p1 = *(bottom->vertices[i]);
        vec3 p2 = *(bottom->vertices[(i + 1) % numPoints]);
        vec3 p3 = center;
        shapes[i + 1] = new Triangle(p1, p2, p3, color * 0.5f);
    }
}
TriangularPrism::TriangularPrism(vec3 topCorner, vec3 bottomLeftCorner, vec3 bottomRigthCorner, double height, vec3 color)
{
    Triangle *front = new Triangle(topCorner, bottomLeftCorner, bottomRigthCorner, color);
    Triangle *back = new Triangle(topCorner + vec3(0, 0, -height), bottomLeftCorner + vec3(0, 0, -height), bottomRigthCorner + vec3(0, 0, -height), color * 0.8f);

    Rectangle *left = new Rectangle(topCorner, bottomLeftCorner, topCorner + vec3(0, 0, -height), bottomLeftCorner + vec3(0, 0, -height), color * 0.6f);
    Rectangle *right = new Rectangle(topCorner, bottomRigthCorner, topCorner + vec3(0, 0, -height), bottomRigthCorner + vec3(0, 0, -height), color * 0.4f);
    Rectangle *bottom = new Rectangle(bottomLeftCorner, bottomRigthCorner, bottomLeftCorner + vec3(0, 0, -height), bottomRigthCorner + vec3(0, 0, -height), color * 0.2f);
    
    numShapes = 5;
    shapes = new Shape *[numShapes]
    {
        front, back, left, right, bottom
    };
}
Tail::Tail(vec3 start, vec3 color)
{
    TriangularPrism* base = new TriangularPrism(start + vec3(0.1, -0.35, 0.1), start + vec3(-0.1, 0, 0.1), start + vec3(0.1,0,0.1), 0.2, color);
    
    double wingHeight = 0.2, wingWidth = 0.05, wingLength = 0.35;

    RectangularPrism* leftWing = new RectangularPrism(start + vec3(0.03, 0.25, 0.08),wingHeight, wingWidth, wingLength, color);
    RectangularPrism* rightWing = new RectangularPrism(start + vec3(0.03, 0.25, -0.08), wingHeight, wingWidth, wingLength, color);
    RectangularPrism* topWing = new RectangularPrism(start + vec3(0.09, 0.25, 0), wingHeight, wingLength, wingWidth, color);
    
    RectangularPrism* rudder = new RectangularPrism(start + vec3(0.085, 0.21, 0), wingHeight - 0.15, wingLength - 0.02, wingWidth - 0.02, vec3(0, 0.5, 1));
    
    RectangularPrism* leftFin = new RectangularPrism(start + vec3(0.03, 0.21, 0.08), wingHeight - 0.15, wingWidth - 0.02, wingLength - 0.02, vec3(0, 0.5, 1));
    RectangularPrism* rightFin = new RectangularPrism(start + vec3(0.03, 0.21, -0.08), wingHeight - 0.15, wingWidth - 0.02, wingLength - 0.02, vec3(0, 0.5, 1));

    numShapes = 7;
    shapes = new Shape*[numShapes] { base, leftWing, rightWing, topWing, rudder, leftFin, rightFin };
}
Wheel::Wheel(vec3 center, double radius, vec3 color, int numVertices)
{
    this->center = center;
    double shaftHeight = 0.15, shaftRadius = 0.03;

    vec3 shaftCenter = center + vec3(0, shaftHeight / 2, 0);
    Cylinder* shaft = new Cylinder(shaftCenter, shaftHeight, shaftRadius, vec3(0.3,0.3,0.3), numVertices);
    Cylinder* shaftCover = new Cylinder(shaftCenter + vec3(0,0.05,0), shaftHeight - 0.05, shaftRadius + 0.02, color * 0.5f, numVertices);

    vec3 leftWheelCenter = center - vec3(shaftRadius/2 + radius/2, 0, 0);
    vec3 rightWheelCenter = center + vec3(shaftRadius/2 + radius/2, 0, 0);
    Cylinder* leftWheel = new Cylinder(leftWheelCenter, 0.03, radius, vec3(0,0,0), numVertices);
    leftWheel->rotate(M_PI/2, 2);
    Cylinder* rightWheel = new Cylinder(rightWheelCenter, 0.03, radius, vec3(0,0,0), numVertices);
    rightWheel->rotate(M_PI/2, 2);
    
    numShapes = 4; 
    shapes = new Shape*[numShapes] { shaft, shaftCover, leftWheel, rightWheel };
}
EngineNose::EngineNose(vec3 center, double radius, double height, vec3 color)
{
    this->center =  center + vec3(0, height - 0.08, 0);
    Cone* engineNose = new Cone(this->center, height - 0.1, radius, vec3(0.3, 0.3, 0.3));
    numShapes = 4;
    shapes = new Shape*[numShapes] {engineNose };
    for (int i = 0; i < 3; i++)
    {
        double angle = i * (2 * M_PI / 3);
        vec3 bladeCenter = center + vec3(0, height - 0.12, 0);
        Cylinder* blade = new Cylinder(bladeCenter, 0.3, 0.01, vec3(0.3, 0.3, 0.3));
        blade->rotate(M_PI/2, 2);
        blade->rotate(angle, 1);
        shapes[i + 1] = blade;
    }
}
Engine::Engine(vec3 center, vec3 color)
{
    this->center = center;
    double radius = 0.06, height = 0.3;
    Cylinder* engineBody = new Cylinder(center, height, radius, color);
    EngineNose* nose = new EngineNose(center, radius, height, color);
    numShapes = 2;
    shapes = new Shape*[numShapes] {nose, engineBody };
}
Plane::Plane(vec3 center, vec3 color)
{
    center = vec3(0, 0, 0);
    // Define dimensions for the plane parts
    double bodyRadius = 0.15, bodyLength = 1.0;
    double noseHeight = 0.6, noseRadius = bodyRadius;
    double wingHeight = 0.08, wingWidth = 0.3, wingLength = 0.7;
    int numVertices = 12;

    Cylinder* body = new Cylinder(center, bodyLength, bodyRadius, color, numVertices);

    vec3 noseCenter = center + vec3(0 , bodyLength/2 + noseHeight/2, 0);
    Cone* nose = new Cone(noseCenter, noseHeight, noseRadius, color * 0.7f, numVertices);
    Cone* tailCone = new Cone(center - vec3(0, bodyLength/2 + 0.1, 0), -0.2, noseRadius, color, numVertices);
    
    vec3 leftWingCenter = center - vec3(-bodyRadius/5, 0, bodyRadius/1.1);
    vec3 rightWingCenter = center + vec3(bodyRadius/5, 0, bodyRadius/1.1);
    RectangularPrism* leftWing = new RectangularPrism(leftWingCenter, wingWidth, wingHeight, wingLength, color);
    RectangularPrism* rightWing = new RectangularPrism(rightWingCenter, wingWidth, wingHeight,  wingLength, color);

    // Create the tail (triangular prism)
    Tail* tail = new Tail(vec3(0,-bodyLength/2,0), color);

    Wheel* leftWheel = new Wheel(center + vec3(-0.23, 0, -0.09), 0.06, color, numVertices);
    leftWheel->rotate(-M_PI/2, 2);
    leftWheel->rotate(M_PI/2, 0);
    Wheel* leftWheel2 = new Wheel(center + vec3(-0.23, -0.12, -0.09), 0.06, color, numVertices);
    leftWheel2->rotate(-M_PI/2, 2);
    leftWheel2->rotate(M_PI/2, 0);
    Wheel* rightWheel = new Wheel(center + vec3(-0.23, 0, 0.09), 0.06, color, numVertices);
    rightWheel->rotate(-M_PI/2, 2);
    rightWheel->rotate(M_PI/2, 0);
    Wheel* rightWheel2 = new Wheel(center + vec3(-0.23, -0.12, 0.09), 0.06, color, numVertices);
    rightWheel2->rotate(-M_PI/2, 2);
    rightWheel2->rotate(M_PI/2, 0);
    Wheel* frontWheel = new Wheel(center + vec3(-0.23, bodyLength/2.5, 0), 0.06, color, numVertices);
    frontWheel->rotate(-M_PI/2, 2);
    frontWheel->rotate(M_PI/2, 0);

    Engine *engine1 = new Engine(center + vec3(0.01, 0.13, 0.6), color);
    Engine *engine2 = new Engine(center + vec3(0.01, 0.13, -0.6), color);
    Engine *engine3 = new Engine(center + vec3(0.01, 0.13, 0.3), color);
    Engine *engine4 = new Engine(center + vec3(0.01, 0.13, -0.3), color);

    engines[0] = engine1;
    engines[1] = engine2;
    engines[2] = engine3;
    engines[3] = engine4;

    Cylinder* cockpit = new Cylinder(center + vec3(0.03, 0.53, 0), 0.14, 0.12, color, numVertices);
    RectangularPrism* window1 = new RectangularPrism(center + vec3(0.035, 0.195, 0.01), 0.05, 0.05, 0.05, vec3(0.2, 0.2, 1));
    RectangularPrism* window2 = new RectangularPrism(center + vec3(0.035, 0.195, -0.01), 0.05, 0.05, 0.05, vec3(0.2, 0.2, 1));
    RectangularPrism* window3 = new RectangularPrism(center + vec3(0.035, 0.18, 0.03), 0.05, 0.05, 0.05, vec3(0.2, 0.2, 1));
    RectangularPrism* window4 = new RectangularPrism(center + vec3(0.035, 0.18, -0.03), 0.05, 0.05, 0.05, vec3(0.2, 0.2, 1));
     
    numShapes = 20;
    shapes = new Shape*[numShapes] { body, nose, leftWing, rightWing, tailCone, tail, leftWheel, leftWheel2, rightWheel, rightWheel2, frontWheel, engine1, engine2, engine3, engine4, cockpit, window1, window2, window3, window4 };
}