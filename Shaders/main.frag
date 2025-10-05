#version 450 core
in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    // Example: simple gradient
    FragColor = vec4(TexCoords, 0.5, 1.0);
}
