#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class Mesh
{
	public:
	unsigned int VAO, VBO;
	GLsizei vertexCount;
	//constructor
	Mesh(float* vertices, size_t size, GLsizei count,
		const std::vector<std::pair<GLuint, GLint>>& attributes, GLsizei stride);

	void draw();
	void draw_Circle();

	static std::vector<float> generateCircleVertices(float radius, int segments);

	static std::vector<float> generateQuadVertices();

	void setColor(glm::vec4 color_in);
	glm::vec4 getColor();

	private:
		glm::vec4 color;
};

