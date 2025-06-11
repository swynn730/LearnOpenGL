#version 330 core

struct Material 
{
    sampler2D diffuse;
    vec3 tint;
    sampler2D specular;
    float shininess;
    sampler2D emissive;
    float emissiveStrength;
    sampler2D mask;
};

struct Light 
{
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
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

    vec3 maskSampler = vec3(texture(material.mask, FragmentTextureCoordinate));

    // Calculate ambient lighting.
    vec3 ambient = mix(vec3(1.0f), material.tint, maskSampler.r) * vec3(texture(material.diffuse, FragmentTextureCoordinate)) * light.ambientStrength;

    // Calculate the diffuse lighting.
    vec3 lightDirection = normalize(FragmentLightPosition - FragmentPosition);
    vec3 diffuse = mix(vec3(1.0f), material.tint, maskSampler.r) * vec3(texture(material.diffuse, FragmentTextureCoordinate)) * max(dot(normal, lightDirection), 0.0f) * light.diffuseStrength;

    // Calculate the specular lighting.
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFalloff = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.specular, FragmentTextureCoordinate)) * specularFalloff * light.specularStrength;

    // Calculate the emissive lighting.
    vec3 emissive = vec3(texture(material.emissive, FragmentTextureCoordinate)) * material.emissiveStrength * maskSampler.r;

    vec3 phong_lighting = (lightColor * (ambient + diffuse + specular)) + emissive;

    // Output final fragment/pixel color.
    FragmentColor = vec4(phong_lighting, 1.0f);
}