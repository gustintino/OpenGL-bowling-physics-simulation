#version 330 core
        
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 fragmentShaderColor;

out vec3 worldSpaceNormal;
out vec4 lightDirection;
out vec4 viewerVector;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

vec4 b;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);

    vec4 worldSpacePoint = modelMatrix * vec4(vertexPosition, 1.0f);
    worldSpaceNormal = mat3(transpose(inverse(modelMatrix))) * normal;  //N
    lightDirection = vec4(lightPosition, 1.0f) - worldSpacePoint;       //L

    viewerVector = vec4(cameraPosition, 1.0) - worldSpacePoint;         //V

    fragmentShaderColor = color;

    //gourad

    ////diffuse
    //vec4 worldSpacePoint = modelMatrix * vec4(vertexPosition, 1.0f);
    //vec3 worldSpaceNormal = normalize(mat3(transpose(inverse(modelMatrix))) * normal);    // N
    //vec4 lightDirection = normalize(vec4(lightPosition, 1.0f) - worldSpacePoint);         // L
    //
    //
    ////specular
    //vec4 reflection = reflect(-lightDirection, vec4(worldSpaceNormal, 1.0f));             // R
    //vec4 viewerVector = normalize(vec4(cameraPosition, 1.0) - worldSpacePoint);           // V
    //
    ////b = reflection * viewerVector;
    ////vec4 a = pow(b, vec4(shinines, 1.0f));
    //
    //fragmentShaderColor = (color * vec4(ambientIntensity, 1.0))
    //    + color * ((max(dot(lightDirection, vec4(worldSpaceNormal, 1.0)), 0.0)) * vec4(diffuseIntensity, 1))
    //      + (vec4(specularColor, 1.0) * pow(max(dot(reflection, viewerVector), 0.0), shinines) * vec4(specularIntensity, 1));
}
        