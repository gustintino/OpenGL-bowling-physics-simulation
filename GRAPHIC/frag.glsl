#version 330 core

layout(location = 0) out vec4 fragmentColour;

in vec4 fragmentShaderColor;

in vec3 worldSpaceNormal;
in vec4 lightDirection;
in vec4 viewerVector;

uniform vec3 ambientIntensity;
uniform vec3 diffuseIntensity;
uniform vec3 specularIntensity;
uniform vec3 specularColor;
uniform float shinines;

void main()
{
    vec3 N = normalize(worldSpaceNormal);
    vec4 L = normalize(lightDirection);

    vec4 V = normalize(viewerVector);
    vec4 reflection = reflect(-L, vec4(N, 1.0f));

    fragmentColour = fragmentShaderColor * vec4(ambientIntensity, 1.0f)
                   + fragmentShaderColor * ((max(dot(L, vec4(N, 1.0)), 0.0)) * vec4(diffuseIntensity, 1))
                   + vec4(specularColor, 1.0) * pow(max(dot(reflection, V), 0.0), shinines) * vec4(specularIntensity, 1);

}
        