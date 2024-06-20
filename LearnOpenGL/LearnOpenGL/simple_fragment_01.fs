#version 330 core
in vec3 vertexColor;
in vec3 vertexPosition;
out vec4 FragColor;

uniform float ourColor;

void main()
{
    //FragColor = vec4(0.0, ourColor, 0.0, 1.0f);
    FragColor = vec4(vertexPosition, 1.0f);
}