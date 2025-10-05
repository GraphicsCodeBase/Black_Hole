#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Mesh.hpp>
#include <Shader.hpp>
std::string vertShader = "../../../Shaders/main.vert";
std::string fragShader = "../../../Shaders/main.frag";

// Callback to adjust viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set GLFW to use OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac compatibility
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "BLACK_HOLE_SIM", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set viewport and callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //make a simple triangle in 2D 
    float vertices[] = {
        // positions        // colors
         0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top (red)
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // left (green)
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // right (blue)
    };
    //make a mesh
    Mesh triangle(vertices, sizeof(vertices));
    //make shaders
    std::string vertCode = Shader::LoadShaderFromFile(vertShader);
    std::string fragCode = Shader::LoadShaderFromFile(fragShader);
    Shader mainShader(vertCode, fragCode);

    // Model matrix — identity, triangle stays in place
    glm::mat4 model = glm::mat4(1.0f);

    // View matrix — camera at z=3 looking at origin
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Ortho projection — cube from -1 to 1 in XY
    glm::mat4 projection = glm::ortho(
        -1.0f, 1.0f,   // left, right
        -1.0f, 1.0f,   // bottom, top
        0.1f, 100.0f   // near, far
    );

   
    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen to pastel blue
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // pastel blue RGBA
        glClear(GL_COLOR_BUFFER_BIT);

        //bind shader
        mainShader.Use();
        //pass in uniforms
        mainShader.SetMat4("u_Model", model);
        mainShader.SetMat4("u_View", view);
        mainShader.SetMat4("u_Projection", projection);
        triangle.draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
