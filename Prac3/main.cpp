#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "shapes.h"
#include "Matrix.h"

using namespace glm;
using namespace std;

bool wireframe = false;
Shape *plane = new Cylinder(vec3(0, 0, 0), 0.5, 1, vec3(1, 0, 0));
// Shape *plane = new RectangularPrism(vec3(0, 0, 0), 1, 1, 1, vec3(1, 0, 0));
double **transformationMatrix;
const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1000, 1000, "Experiment", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
}
void resetMatrix(double** matrix)
{
    if (matrix == NULL)
    {
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        if (matrix[i] == NULL)
        {
            matrix[i] = new double[3];
        }
        for (int j = 0; j < 3; j++)
        {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    double angle = 0.1;
    if (action == GLFW_PRESS || action == GLFW_REPEAT && transformationMatrix != NULL)
    {
        switch (key)
        {
            case GLFW_KEY_W: // Rotate about x-axis in an anti-clockwise direction.
                transformationMatrix[1][1] = cos(angle);
                transformationMatrix[1][2] = sin(angle);
                transformationMatrix[2][1] = -sin(angle);
                transformationMatrix[2][2] = cos(angle);
                break;
            case GLFW_KEY_S: // Rotate about x-axis in a clockwise direction.
                transformationMatrix[1][1] = cos(-angle);
                transformationMatrix[1][2] = sin(-angle);
                transformationMatrix[2][1] = -sin(-angle);
                transformationMatrix[2][2] = cos(-angle);
                break;
            case GLFW_KEY_A: // Rotate about y-axis in an anti-clockwise direction.
                transformationMatrix[0][0] = cos(angle);
                transformationMatrix[0][2] = -sin(angle);
                transformationMatrix[2][0] = sin(angle);
                transformationMatrix[2][2] = cos(angle);
                break;
            case GLFW_KEY_D: // Rotate about y-axis in a clockwise direction.
                transformationMatrix[0][0] = cos(-angle);
                transformationMatrix[0][2] = -sin(-angle);
                transformationMatrix[2][0] = sin(-angle);
                transformationMatrix[2][2] = cos(-angle);
                break;
            case GLFW_KEY_E: // Rotate about z-axis in an anti-clockwise direction.
                transformationMatrix[0][0] = cos(angle);
                transformationMatrix[0][1] = sin(angle);
                transformationMatrix[1][0] = -sin(angle);
                transformationMatrix[1][1] = cos(angle);
                break;
            case GLFW_KEY_Q: // Rotate about z-axis in a clockwise direction.
                transformationMatrix[0][0] = cos(-angle);
                transformationMatrix[0][1] = sin(-angle);
                transformationMatrix[1][0] = -sin(-angle);
                transformationMatrix[1][1] = cos(-angle);
                break;
            case GLFW_KEY_ENTER: // Toggle wireframe
                wireframe = !wireframe;
            default:
                return;
        }
        
        Matrix *m = new Matrix(3,3,transformationMatrix);
        plane->applyMatrix(m);
        resetMatrix(transformationMatrix);
    }
}
void drawShape(Shape* shape, GLuint vertexbuffer, GLuint colorbuffer, GLuint programID)
{
    GLfloat* vertices;
    int numVertices = shape->numVertices();
    vertices = shape->toVertexArray();
    
    GLfloat* colors = shape->toColorArray();

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[numVertices]), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[shape->numColors()]), colors, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    if (wireframe)
    {
        glDrawArrays(GL_LINES, 0, numVertices);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
int main()
{
    transformationMatrix = new double*[3];
    resetMatrix(transformationMatrix);
    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << e << endl;
        throw;
    }

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // light blue background

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEAREST);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer; 
    glGenBuffers(1, &colorbuffer);

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        //
        drawShape(plane, vertexbuffer, colorbuffer, programID);
        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete plane;
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glfwTerminate();
    return 0;
}
