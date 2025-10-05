#include <Shader.hpp>

Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode)
{
	// Convert from std::string to const char*
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	int success{};
	char infoLog[1024]{};

	// Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Specify source code for shader before compiling it
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
		std::cerr << "Failed to compile vertex shader!" << std::endl;
		std::cerr << "InfoLog: " << infoLog << std::endl;
	}

	// Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Specify source code for shader before compiling it
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
		std::cerr << "Failed to compile fragment shader!" << std::endl;
		std::cerr << "InfoLog: " << infoLog << std::endl;
	}


	// Take both shaders and combine them to form a Shader Program, assign to private member variable
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 1024, nullptr, infoLog);
		std::cerr << "Failed to link shaders!" << std::endl;
		std::cerr << "InfoLog: " << infoLog << std::endl;
	}

	// We have linked shaders to the program, can delete shaders now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
	glUseProgram(shaderProgramID);
}

// ------------------------------
// Uniform Setter Implementations
// ------------------------------

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

std::string Shader::LoadShaderFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "ERROR: Could not open shader file: " << filePath << std::endl;
		return "";
	}

	std::stringstream ss;
	ss << file.rdbuf(); // read entire file into stringstream
	file.close();

	return ss.str(); // convert stringstream to string
}
