#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>                // core types (vec3, mat4, etc.)
#include <glm/gtc/matrix_transform.hpp> // for translate, rotate, scale, perspective, etc
#include <iostream>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <BlackHole.hpp>
#include <Camera.hpp>
#include <Graphics.hpp>
std::string vertShader = "../../../Shaders/main.vert";
std::string fragShader = "../../../Shaders/main.frag";
std::string QuadfragShader = "../../../Shaders/quad.frag";
std::string QuadvertShader = "../../../Shaders/quad.vert";
std::string CompShader = "../../../Shaders/geodesic.comp";
std::string GridvertShader = "../../../Shaders/grid.vert";
std::string GridfragShader = "../../../Shaders/grid.frag";

bool mousePressed = false;
double lastX = 400.0, lastY = 300.0;
bool firstMouse = true;

// Global camera pointer for callbacks
Camera* g_camera = nullptr;

//========================
//========================
// Callback to adjust viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Mouse button callback - track when mouse is pressed/released
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mousePressed = true;
            firstMouse = true;  // Reset on new drag
        }
        else if (action == GLFW_RELEASE)
        {
            mousePressed = false;
        }
    }
}

// Mouse movement callback - orbit camera when dragging
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mousePressed || !g_camera)
        return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    // Calculate mouse movement delta
    float deltaX = static_cast<float>(xpos - lastX);
    float deltaY = static_cast<float>(lastY - ypos);  // Reversed: y goes from bottom to top

    lastX = xpos;
    lastY = ypos;

    // Update camera based on mouse drag
    g_camera->processMouseDrag(deltaX, deltaY);
}

// Mouse scroll callback - zoom in/out
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_camera)
    {
        g_camera->processMouseScroll(static_cast<float>(yoffset));
    }
}

void renderTrail(const std::vector<glm::vec2>& trail, Shader& shader, const glm::mat4& projection)
{
    if (trail.size() < 2) return;  // Need at least 2 points for a line

    // Create vertices from trail points
    std::vector<float> vertices;
    for (const auto& point : trail)
    {
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(0.0f);  // z = 0 (2D)
    }

    // Create VAO, VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set uniforms and draw
    shader.Use();
    shader.SetMat4("u_Projection", projection);
    shader.SetMat4("u_View", glm::mat4(1.0f));
    shader.SetMat4("u_Model", glm::mat4(1.0f));  // Identity matrix
    shader.SetVec4("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));  // Yellow trail

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, trail.size());

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

    // Set viewport and callbacks
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    
    //auto circleVertices = Mesh::generateCircleVertices(1.0f, 64);

    //Mesh circleMesh(
    //    circleVertices.data(),
    //    circleVertices.size() * sizeof(float),
    //    static_cast<GLsizei>(circleVertices.size() / 3),
    //    { {0, 3} },  // only one attribute: vec3 position
    //    3 * sizeof(float)
    //);
    //circleMesh.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));//set to red for now.

    //std::string vertCode = Shader::LoadShaderFromFile(vertShader);
    //std::string fragCode = Shader::LoadShaderFromFile(fragShader);
    //Shader mainShader(vertCode, fragCode);

	std::string quadVertCode = Shader::LoadShaderFromFile(QuadvertShader);
	std::string quadFragCode = Shader::LoadShaderFromFile(QuadfragShader);
	Shader quadShader(quadVertCode, quadFragCode);

    //convert the comp shader into a string
	std::string computeCode = Shader::LoadShaderFromFile(CompShader);
	Shader computeShader(computeCode, true);

    // Load grid shader for spacetime visualization
    std::string gridVertCode = Shader::LoadShaderFromFile(GridvertShader);
    std::string gridFragCode = Shader::LoadShaderFromFile(GridfragShader);
    Shader gridShader(gridVertCode, gridFragCode);
    std::cout << "Grid shader loaded successfully\n";

    //generate a quad to will up the window.
	Graphics graphics(static_cast<int>(screenWidth), static_cast<int>(screenHeight));
    graphics.bindForCompute();//making sure that the current computer shader is active.

    //float x = 0.7f;     // move 0.5 units to the right
    //float y = -0.3f;    // move 0.3 units down
    //float radius = 0.5f; // scale the circle (default is 1.0)
    glm::mat4 projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    // After (using pixel coordinates):
    float x = 400.0f;
    float y = 300.0f;
    float radius = 40.0f;

    float desiredRs = 40.0f;
    double mass = (desiredRs * C * C) / (2.0 * G);
    BlackHole blackHole(glm::vec2(x, y), mass);
    // Camera and mouse tracking
    // Camera orbits around black hole at (x, 0, y) on the ground plane
    Camera camera(glm::vec3(x, 0.0f, y), 650.0f);
    camera.elevation = 1.3f;   // Look down from above to see horizontal disk
    camera.azimuth = 0.8f;     // Diagonal view like reference

    // Set global camera pointer for callbacks
    g_camera = &camera;

    std::cout << "=== BLACK HOLE INFO ===\n";
    std::cout << "Position: (" << x << ", " << y << ")\n";
    std::cout << "Schwarzschild Radius: " << blackHole.schwarzschildRadius << " pixels\n";
    std::cout << "Photon sphere: " << blackHole.schwarzschildRadius * 1.5f << " pixels\n";
    std::cout << "\n=== CAMERA INFO ===\n";
    std::cout << "Camera Position: (" << camera.getPosition().x << ", "
              << camera.getPosition().y << ", " << camera.getPosition().z << ")\n";
    std::cout << "Camera FOV: " << camera.fov << " degrees\n";
    std::cout << "\n=== CONTROLS ===\n";
    std::cout << "LEFT CLICK + DRAG: Orbit camera around black hole\n";
    std::cout << "SCROLL WHEEL: Zoom in/out\n";
    std::cout << "W/S KEYS: Zoom in/out\n";
    std::cout << "ESC: Close window\n\n";

    // Create spacetime grid mesh
    float gridSize = 1500.0f;  // LARGER grid to show more spacetime
    int gridDivisions = 50;     // 50x50 grid (finer detail)
    auto gridVertices = Mesh::generateGridVertices(gridSize, gridDivisions);
    Mesh gridMesh(
        gridVertices.data(),
        gridVertices.size() * sizeof(float),
        static_cast<GLsizei>(gridVertices.size() / 3),
        { {0, 3} },  // Only position attribute (vec3)
        3 * sizeof(float)
    );

    std::cout << "Grid created: " << gridDivisions << "x" << gridDivisions
              << " (" << gridVertices.size() / 3 << " vertices)\n";

    //std::vector<LightRay> lightRays;

    //// === LIGHT SOURCE: Spray of rays from one point ===
    //glm::vec2 sourcePosition(100.0f, 300.0f);  // Single point on the left
    //int numRays = 30;  // Number of rays in the fan
    //float spreadAngle = 60.0f;  // Total spread in degrees (±30°)

    //for (int i = 0; i < numRays; i++)
    //{
    //    LightRay ray;

    //    // Calculate angle for this ray
    //    float angleOffset = -spreadAngle / 2.0f + (spreadAngle / (numRays - 1)) * i;
    //    float angleRadians = glm::radians(angleOffset);  // Convert to radians

    //    // Base velocity pointing right (toward black hole)
    //    float speed = 70.0f;
    //    float baseAngle = 0.0f;  // 0° = pointing right

    //    // Rotate velocity by angleOffset
    //    float finalAngle = baseAngle + angleRadians;
    //    glm::vec2 velocity(
    //        speed * std::cos(finalAngle),
    //        speed * std::sin(finalAngle)
    //    );

    //    ray.initialize(sourcePosition, velocity, blackHole);
    //    lightRays.push_back(ray);
    //}

    //std::cout << "Created " << numRays << " rays from ("
    //    << sourcePosition.x << ", " << sourcePosition.y << ")\n";
    //std::cout << "Spread: ±" << spreadAngle / 2.0f << " degrees\n";

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process keyboard input (W/S to zoom)
        float deltaTime = 0.016f;  // ~60 FPS
        camera.processKeyboard(window, deltaTime);

        // Clear screen to PURE BLACK background (like reference)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Pure black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind compute shader
        computeShader.Use();

        // Set uniforms for compute shader
        computeShader.SetVec2("u_blackHolePos", glm::vec2(x, y));
        computeShader.SetFloat("u_mass", static_cast<float>(mass));
        computeShader.SetFloat("u_Rs", static_cast<float>(blackHole.schwarzschildRadius));
        computeShader.SetVec2("u_screenSize", glm::vec2(screenWidth, screenHeight));

        glm::vec3 camPos = camera.getPosition();
        computeShader.SetVec3("u_cameraPos", camPos);
        computeShader.SetFloat("u_cameraFOV", camera.fov);

        glBindImageTexture(0, graphics.getTexture(),0,GL_FALSE,0, GL_WRITE_ONLY, GL_RGBA8);

		int workGroupsX, workGroupsY;
        graphics.getWorkGroups(workGroupsX, workGroupsY);
		//this will dispatch the compute shader with enough work groups to cover the whole texture.
        glDispatchCompute(workGroupsX, workGroupsY, 1);
		//this will tell opengl to wait until the compute shader is done writing to the texture.
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        graphics.renderQuad(quadShader);

        // === Render spacetime grid with warping ===
        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        gridShader.Use();

        // Set transformation matrices
        glm::mat4 gridModel = glm::mat4(1.0f);
        gridModel = glm::translate(gridModel, glm::vec3(x, 0.0f, y));  // Center grid at black hole on ground plane
        gridModel = glm::rotate(gridModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate to stand upright

        gridShader.SetMat4("u_Model", gridModel);
        gridShader.SetMat4("u_View", camera.getViewMatrix());
        gridShader.SetMat4("u_Projection", camera.getProjectionMatrix(screenWidth / screenHeight));

        // Set grid-specific uniforms
        gridShader.SetVec2("u_blackHolePos", glm::vec2(x, y));
        gridShader.SetFloat("u_Rs", static_cast<float>(blackHole.schwarzschildRadius));
        gridShader.SetFloat("u_warpStrength", 400.0f);  // MUCH stronger warp for deep funnel!

        // Draw grid as lines
        gridMesh.draw_Lines();

        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
