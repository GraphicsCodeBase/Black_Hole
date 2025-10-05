#version 450 core

layout (location = 0) in vec3 aPos;     // vertex position
layout (location = 1) in vec3 aColor;   // vertex color

out vec3 vColor;  // pass to fragment shader

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    vColor = aColor;
}
