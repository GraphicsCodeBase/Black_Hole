#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
	unsigned int VAO, VBO;
	//constructor
	Mesh(float* vertices, size_t size);

	void draw();

};

