#version 330 core

in vec3 FragmentPosition;
in vec3 vNormal;
in vec2 vTextureCoordinate;
in vec3 vLightPosition;
in vec3 gouraud_lighting;

out vec4 FragmentColor;

uniform sampler2D texture_00;
uniform float mixerValue;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main()
{
    //FragColor = mix(texture(texture_00, TexCoord), texture(texture_00, TexCoord) * (objectColor * lightColor), mixerValue);
    // Calculate the lighting in the fragment shader (Phong).
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

    vec3 phong_lighting = (ambient + diffuse + specular);

    // Output final fragment/pixel color.
    FragmentColor = vec4(objectColor * phong_lighting, 1.0f);
    //FragmentColor = vec4(objectColor * gouraud_lighting, 1.0f);
}