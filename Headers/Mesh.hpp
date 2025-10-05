#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class Mesh
{
	public:
	unsigned int VAO, VBO;
	GLsizei vertexCount;
	//constructor
	Mesh(float* vertices, size_t size, GLsizei count,
		const std::vector<std::pair<GLuint, GLint>>& attributes, GLsizei stride);

	void draw();
};

