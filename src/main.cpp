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

    ////make a simple triangle in 2D 
    //float vertices[] = {
    //    // positions        // colors
    //     0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top (red)
    //    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // left (green)
    //     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // right (blue)
    //};
    ////make a mesh
    //Mesh triangle(vertices, sizeof(vertices));
    //make shaders
    float quadVertices[] = {
        // positions    // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    Mesh quad(
        quadVertices,
        sizeof(quadVertices),
        6, // 6 vertices
        { {0,2},{1,2} }, // attributes: location 0=pos(2), location 1=UV(2)
        4 * sizeof(float) // stride
    );

    std::string vertCode = Shader::LoadShaderFromFile(vertShader);
    std::string fragCode = Shader::LoadShaderFromFile(fragShader);
    Shader mainShader(vertCode, fragCode);

   
    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen to pastel blue
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // pastel blue RGBA
        glClear(GL_COLOR_BUFFER_BIT);

        //bind shader
        mainShader.Use();
    
        //triangle.draw();
        quad.draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
