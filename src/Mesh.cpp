#include <Mesh.hpp>
//mesh constructor
 // General constructor: pass stride (in bytes) and attribute layout info
Mesh::Mesh(float* vertices, size_t size, GLsizei count,
    const std::vector<std::pair<GLuint, GLint>>& attributes, GLsizei stride)
{
    vertexCount = count;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    for (size_t i = 0; i < attributes.size(); ++i)
    {
        GLuint index = attributes[i].first;   // attribute location
        GLint sizeAttr = attributes[i].second; // number of floats
        GLintptr offset = 0;
        for (size_t j = 0; j < i; ++j)
            offset += attributes[j].second * sizeof(float);

        glVertexAttribPointer(index, sizeAttr, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(index);
    }

    glBindVertexArray(0);
}

//draw function.
void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Mesh::draw_Circle()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    glBindVertexArray(0);
}

std::vector<float> Mesh::generateCircleVertices(float radius, int segments)
{
    std::vector<float> vertices;

    // Center of circle
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Points around the circle
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    return vertices;
}

void Mesh::setColor(glm::vec4 color_in)
{
    color = color_in;
}

glm::vec4 Mesh::getColor()
{
    return color;
}

