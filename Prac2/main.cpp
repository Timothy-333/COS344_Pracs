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

using namespace glm;
using namespace std;

bool wireframe = false;
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void drawShape(Shape* shape, GLuint vertexbuffer, GLuint colorbuffer, GLuint programID)
{
    if (Car* car = dynamic_cast<Car*>(shape)) 
    {
        float rotateAmount = 0.05f; // Adjust as needed
        mat3x3 rotationMatrix = mat3x3(1.0f);
        rotationMatrix[0][0] = cos(rotateAmount);
        rotationMatrix[0][1] = -sin(rotateAmount);
        rotationMatrix[1][0] = sin(rotateAmount);
        rotationMatrix[1][1] = cos(rotateAmount);

        for (int i = 0; i < 4; i++) {
            // Translate to origin
            mat3x3 translateToOrigin = mat3x3(1.0f);
            translateToOrigin[2][0] = -car->wheels[i]->center.x;
            translateToOrigin[2][1] = -car->wheels[i]->center.y;

            // Translate back
            mat3x3 translateBack = mat3x3(1.0f);
            translateBack[2][0] = car->wheels[i]->center.x;
            translateBack[2][1] = car->wheels[i]->center.y;

            // Apply the transformations
            car->wheels[i]->applyMatrix(translateToOrigin);
            car->wheels[i]->applyMatrix(rotationMatrix);
            car->wheels[i]->applyMatrix(translateBack);
        }
    }
    GLfloat* vertices;
    int numVertices = shape->numVertices();
    // if (wireframe)
    // {
    //     vertices = shape->toVertexArrayWireframe();
    //     numVertices += shape->numShapes * 2;
    // }
    // else
    // {
        vertices = shape->toVertexArray();
    // }
    
    GLfloat* colors = shape->toColorArray();

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[numVertices]), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[shape->numColors()]), colors, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float moveAmount = 0.05f;
    float scaleAmountUp = 0.98f;
    float scaleAmountDown = 1.02f;
    Car* car = static_cast<Car*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        mat3x3 transformationMatrix = mat3x3(1.0f);
        switch (key) {
            case GLFW_KEY_A: // Move up
                transformationMatrix[2][1] = moveAmount;
                transformationMatrix[0][0] = transformationMatrix[1][1] = scaleAmountUp;
                break;
            case GLFW_KEY_S: // Move left
                transformationMatrix[2][0] = -moveAmount;
                break;
            case GLFW_KEY_D: // Move down
                transformationMatrix[2][1] = -moveAmount;
                transformationMatrix[0][0] = transformationMatrix[1][1] = scaleAmountDown;
                break;
            case GLFW_KEY_W: // Move right
                transformationMatrix[2][0] = moveAmount;
                break;
            case GLFW_KEY_ENTER: // Toggle wireframe
                wireframe = !wireframe;
            default:
                return;
        }
        car->applyMatrix(transformationMatrix);

        // Check if the car is within bounds
        if (!car->isWithinBounds())
        {
            // If the car is out of bounds, undo the transformation
            transformationMatrix[2][0] = -transformationMatrix[2][0];
            transformationMatrix[2][1] = -transformationMatrix[2][1];

            // Undo the scaling
            if (key == GLFW_KEY_A) {
                transformationMatrix[0][0] = transformationMatrix[1][1] = 1 / scaleAmountUp;
            } else if (key == GLFW_KEY_D) {
                transformationMatrix[0][0] = transformationMatrix[1][1] = 1 / scaleAmountDown;
            }

            car->applyMatrix(transformationMatrix);
        }
    }
}
int main()
{
    GLFWwindow *window;
    try
    {
        window = setUp();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }
    catch (const char *e)
    {
        cout << e << endl;
        throw;
    }

    glClearColor(0.0f, 0.6f, 0.0f, 0.0f); // Set the background color to green

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer; 
    glGenBuffers(1, &colorbuffer);

    Shape* car = new Car();
    Shape* road = new Road();
    House* house = new House(0.0f, 2.1f, 0.3f);
    House* house2 = new House(-2.0f, 2.1f, 0.3f);
    House* house3 = new House(2.0f, 2.1f, 0.3f);
    

    glfwSetWindowUserPointer(window, car);
    glfwSetKeyCallback(window, key_callback);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        // Draw the car
        drawShape(road, vertexbuffer, colorbuffer, programID);
        drawShape(house, vertexbuffer, colorbuffer, programID);
        drawShape(house2, vertexbuffer, colorbuffer, programID);
        drawShape(house3, vertexbuffer, colorbuffer, programID);
        drawShape(car, vertexbuffer, colorbuffer, programID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glfwTerminate();
    return 0;
}
