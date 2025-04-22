#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform sampler2D texture_00;
uniform float mixerValue;

void main()
{
    //FragColor = mix(texture(texture_00, TexCoord), texture(texture_00, TexCoord) * (objectColor * lightColor), mixerValue);
    FragColor = vec4(objectColor * lightColor, 1.0);
}