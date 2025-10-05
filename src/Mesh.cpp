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

