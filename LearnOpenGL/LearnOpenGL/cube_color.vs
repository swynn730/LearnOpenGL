#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexureCoordinate;

out vec3 vNormal;
out vec2 vTextureCoordinate;
out vec3 FragmentPosition;
out vec3 vLightPosition;
out vec3 gouraud_lighting;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float textureCoordinateScale;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

vec3 calculate_gouraud_lighting() 
{
    // Calculate the lighting in the vertex shader (Gouraud).
    float ambientStrength = 0.15f;
    float diffuseStrength = 1.25f;
    float specularStrength = 0.60f;

    vec3 normal = normalize(vNormal);

    // Calculate ambient lighting.
    vec3 ambient = ambientStrength * lightColor;

    // Calculate the diffuse lighting.
    vec3 lightDirection = normalize(vLightPosition - FragmentPosition);
    vec3 diffuse = diffuseStrength * max(dot(normal, lightDirection), 0.0f) * lightColor;

    // Calculate the specular lighting.
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFalloff = pow(max(dot(viewDirection, reflectDirection), 0.0f), 256);
    vec3 specular = specularStrength * specularFalloff * lightColor; 
    return vec3(ambient + diffuse + specular);
}

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    FragmentPosition = vec3(view * model * vec4(aPosition, 1.0f));
    vNormal = mat3(transpose(inverse(view * model))) * aNormal;
    vTextureCoordinate = textureCoordinateScale * aTexureCoordinate;
    vLightPosition = vec3(view * vec4(lightPosition, 1.0f));
    gouraud_lighting = calculate_gouraud_lighting();
}