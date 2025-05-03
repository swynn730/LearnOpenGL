#version 330 core

in vec3 FragmentPosition;
in vec3 vNormal;
in vec2 vTextureCoordinate;

out vec4 FragmentColor;

uniform sampler2D texture_00;
uniform float mixerValue;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
    //FragColor = mix(texture(texture_00, TexCoord), texture(texture_00, TexCoord) * (objectColor * lightColor), mixerValue);
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 normal = normalize(vNormal);

    // Calculate ambient lighting.
    vec3 ambient = ambientStrength * lightColor;

    // Calculate the diffuse lighting.
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 diffuse = max(dot(normal, lightDirection), 0.0) * lightColor;

    // Calculate the specular lighting.
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularFalloff = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * specularFalloff * lightColor;

    // Output final fragment/pixel color.
    FragmentColor = vec4(objectColor * (ambient + diffuse + specular), 1.0);
}