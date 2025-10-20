#version 450 core
in vec2 TexCoords;
out vec4 FragColor;

uniform vec4 u_Color; // the color from your C++ code

void main()
{
    // Example: simple gradient
    FragColor = u_Color;
}
