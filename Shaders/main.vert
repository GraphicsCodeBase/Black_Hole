#version 450 core

layout(location = 0) in vec2 aPos;      // NDC position
layout(location = 1) in vec2 aTexCoords; // UV

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


void main()
{
    TexCoords = aTexCoords;
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);;
}
