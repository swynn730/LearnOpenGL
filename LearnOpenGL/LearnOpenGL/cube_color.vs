#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexureCoordinate;

out vec2 FragmentTextureCoordinate;
out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec3 FragmentLightPosition;

uniform float textureCoordinateScale;
uniform vec3 lightPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    FragmentPosition = vec3(view * model * vec4(aPosition, 1.0f));
    FragmentNormal = mat3(transpose(inverse(view * model))) * aNormal;
    FragmentTextureCoordinate = textureCoordinateScale * aTexureCoordinate;
    FragmentLightPosition = vec3(view * vec4(lightPosition, 1.0f));
}