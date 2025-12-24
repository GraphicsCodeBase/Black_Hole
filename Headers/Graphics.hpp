#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <iostream>
//this folder holds the functions to render the quad onto the screen.
//it will render a quad the size of the screen.
class Graphics
{
public:
	//constructor
	Graphics(int width, int height);

	// Destructor: cleanup resources
	~Graphics();

	int getWidth() const;
	int getHeight() const;

	// Resize the compute texture (for dynamic resolution)
	void resize(int newWidth, int newHeight);

	GLuint getTexture() const;

	// Bind texture as image for compute shader (call before dispatch)
	void bindForCompute();

	// Render the fullscreen quad with the texture
	void renderQuad(Shader& quadShader);

	// Get work group counts for compute dispatch
	void getWorkGroups(int& outX, int& outY) const;

private:
	GLuint computeTexture;
	int width;
	int height;
	Mesh* quadMesh; // Pointer to manage lifetime

	void createTexture();
	void setupTextureParameters();
};