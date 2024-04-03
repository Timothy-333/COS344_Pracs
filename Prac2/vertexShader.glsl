#version 330 core

// Input vertex data
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec3 vertexColor;

// Output data to the fragment shader
out vec3 fragmentColor;

void main()
{
    // Set the position of the vertex
    gl_Position = vec4(vertexPosition, 0.0, 1.0);

    // Pass the color to the fragment shader
    fragmentColor = vertexColor;
}