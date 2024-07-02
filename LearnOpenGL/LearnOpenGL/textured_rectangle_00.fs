#version 330 core
out vec4 FragColor;
  
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture_00;
uniform sampler2D texture_01;
uniform float mixerValue;

void main()
{
    FragColor = mix(texture(texture_00, TexCoord), texture(texture_01, 1.0f*vec2(-TexCoord.x, TexCoord.y)), mixerValue);
}