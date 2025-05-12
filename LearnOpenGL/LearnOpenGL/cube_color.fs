#version 330 core

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

in vec2 FragmentTextureCoordinate;
in vec3 FragmentNormal;
in vec3 FragmentPosition;
in vec3 FragmentLightPosition;
in vec3 FragmentGouraudLighting;

out vec4 FragmentColor;

uniform sampler2D texture_00;
uniform Material material;
uniform Light light;
uniform float mixerValue;
uniform vec3 viewPosition;
uniform vec3 lightColor;

void main()
{
    //FragColor = mix(texture(texture_00, TexCoord), texture(texture_00, TexCoord) * (objectColor * lightColor), mixerValue);
    // Calculate the lighting in the fragment shader (Phong).
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

    vec3 phong_lighting = lightColor * (ambient + diffuse + specular);

    // Output final fragment/pixel color.
    FragmentColor = vec4(phong_lighting, 1.0f);
    //FragmentColor = vec4(FragmentGouraudLighting, 1.0f);
}