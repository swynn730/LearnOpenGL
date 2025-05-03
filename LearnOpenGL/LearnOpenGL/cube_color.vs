#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexureCoordinate;

out vec3 vNormal;
out vec2 vTextureCoordinate;
out vec3 FragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float textureCoordinateScale;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    FragmentPosition = vec3(model * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vTextureCoordinate = textureCoordinateScale * aTexureCoordinate;
}