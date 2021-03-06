#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
//out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float time;

void main()
{
    float roty = 0.4 * vertex.y * sin(time);
    mat3 twist = mat3(		vec3(cos(roty), 0, -sin(roty)),
    				vec3(0, 1, 0),
    				vec3(sin(roty), 0, cos(roty)));
    
    frontColor = vec4(color,1.0);
    vec3 V = twist * vertex;
    gl_Position = modelViewProjectionMatrix * vec4(V, 1.0);
}
