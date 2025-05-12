#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexureCoordinate;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};

out vec2 FragmentTextureCoordinate;
out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec3 FragmentLightPosition;
out vec3 FragmentGouraudLighting;

uniform Material material;
uniform Light light;
uniform float textureCoordinateScale;
uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 calculate_gouraud_lighting() 
{
    // Calculate the lighting in the vertex shader (Gouraud).
    vec3 normal = normalize(FragmentNormal);

    // Calculate ambient lighting.
    vec3 ambient = material.ambient * light.ambientStrength;

    // Calculate the diffuse lighting.
    vec3 lightDirection = normalize(FragmentLightPosition - FragmentPosition);
    vec3 diffuse = material.diffuse * max(dot(normal, lightDirection), 0.0f) * light.diffuseStrength;

    // Calculate the specular lighting.
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFalloff = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
    vec3 specular = material.specular * specularFalloff * light.specularStrength;
    return vec3(lightColor * (ambient + diffuse + specular));
}

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    FragmentPosition = vec3(view * model * vec4(aPosition, 1.0f));
    FragmentNormal = mat3(transpose(inverse(view * model))) * aNormal;
    FragmentTextureCoordinate = textureCoordinateScale * aTexureCoordinate;
    FragmentLightPosition = vec3(view * vec4(lightPosition, 1.0f));
    FragmentGouraudLighting = calculate_gouraud_lighting();
}