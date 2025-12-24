#include <glm/glm.hpp>
#include <Graphics.hpp>


Graphics::Graphics(int width, int height)
	: width(width), height(height), computeTexture(0), quadMesh(nullptr)
{
	createTexture();

	// Create fullscreen quad mesh
	auto quadVertices = Mesh::generateQuadVertices();
	quadMesh = new Mesh(
		quadVertices.data(),
		quadVertices.size() * sizeof(float),
		6,
		{ {0, 2}, {1, 2} },  // position (vec2) and texcoord (vec2)
		4 * sizeof(float)
	);

	std::cout << "Graphics initialized: " << width << "x" << height << std::endl;
}

//destructor
Graphics::~Graphics()
{
	// Cleanup texture
	if (computeTexture != 0) {
		glDeleteTextures(1, &computeTexture);
	}

	// Cleanup mesh
	if (quadMesh != nullptr) {
		delete quadMesh;
	}
}

int Graphics::getWidth() const
{
	return width;
}

int Graphics::getHeight() const
{
	return height;
}

void Graphics::createTexture()
{
	// Delete old texture if it exists
	if (computeTexture != 0) {
		glDeleteTextures(1, &computeTexture);
	}

	// Generate new texture
	glGenTextures(1, &computeTexture);
	glBindTexture(GL_TEXTURE_2D, computeTexture);

	// Allocate texture storage
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Setup texture parameters
	setupTextureParameters();

	std::cout << "Compute texture created: " << width << "x" << height << std::endl;
}

void Graphics::setupTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Graphics::resize(int newWidth, int newHeight)
{
	//check if theres no change in the size.
	if(newWidth == width && newHeight == height)
	{
		return; //return if true
	}

	//if not 
	//update the width and height
	width = newWidth;
	height = newHeight;
	createTexture(); //recreate the texture with new size
	bindForCompute();
}

GLuint Graphics::getTexture() const
{
	return computeTexture;
}

void Graphics::bindForCompute()
{
	// Bind texture as image unit 0 for compute shader (write-only)
	glBindImageTexture(0, computeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void Graphics::renderQuad(Shader& quadShader)
{
	// Use the shader
	quadShader.Use();

	// Bind texture for sampling
	glBindTexture(GL_TEXTURE_2D, computeTexture);
	quadShader.SetInt("screenTexture", 0);  // Texture unit 0

	// Draw the fullscreen quad
	quadMesh->draw();
}

void Graphics::getWorkGroups(int& outX, int& outY) const
{
	// Compute shader uses 16x16 work groups
	// Round up division
	outX = (width + 15) / 16;
	outY = (height + 15) / 16;
}