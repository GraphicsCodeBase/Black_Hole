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

// Camera and mouse tracking
Camera camera(glm::vec3(400.0f, 300.0f, 0.0f), 300.0f);
bool mousePressed = false;
double lastX = 400.0, lastY = 300.0;
bool firstMouse = true;

//======================== 
//========================
// Callback to adjust viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    // Set viewport and callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    std::cout << "=== BLACK HOLE INFO ===\n";
    std::cout << "Position: (" << x << ", " << y << ")\n";
    std::cout << "Schwarzschild Radius: " << blackHole.schwarzschildRadius << " pixels\n";
    std::cout << "Photon sphere: " << blackHole.schwarzschildRadius * 1.5f << " pixels\n\n";

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
        // Clear screen to pastel blue
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // pastel blue RGBA
        glClear(GL_COLOR_BUFFER_BIT);

		//bind compute shader
        computeShader.Use();

        // Set uniforms for compute shader
        computeShader.SetVec2("u_blackHolePos", glm::vec2(x, y));
        computeShader.SetFloat("u_mass", static_cast<float>(mass));
        computeShader.SetFloat("u_Rs", static_cast<float>(blackHole.schwarzschildRadius));
        computeShader.SetVec2("u_screenSize", glm::vec2(screenWidth, screenHeight));

        glBindImageTexture(0, graphics.getTexture(),0,GL_FALSE,0, GL_WRITE_ONLY, GL_RGBA8);

		int workGroupsX, workGroupsY;
        graphics.getWorkGroups(workGroupsX, workGroupsY);
		//this will dispatch the compute shader with enough work groups to cover the whole texture.
        glDispatchCompute(workGroupsX, workGroupsY, 1);
		//this will tell opengl to wait until the compute shader is done writing to the texture.
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        graphics.renderQuad(quadShader);

        float deltaTime = 0.008f;  // ~60 FPS
        static int debugCounter = 0;
        debugCounter++;
        //for (int i = 0; i < lightRays.size(); i++)
        //{
        //    auto& ray = lightRays[i];

        //    // DEBUG: Print before step
        //    if (debugCounter == 1 && i == 0)  // First frame, first ray
        //    {
        //        std::cout << "BEFORE step - Ray 0:\n";
        //        std::cout << "  Position: (" << ray.position.x << ", " << ray.position.y << ")\n";
        //        std::cout << "  r=" << ray.r << ", theta=" << ray.theta << "\n";
        //        std::cout << "  dr_dlambda=" << ray.dr_dlambda << ", dtheta_dlambda=" << ray.dtheta_dlambda << "\n";
        //        std::cout << "  active=" << ray.active << "\n";
        //    }

        //    if (ray.active)
        //    {
        //        ray.step(deltaTime, blackHole);
        //    }

        //    // DEBUG: Print after step
        //    if (debugCounter == 1 && i == 0)  // First frame, first ray
        //    {
        //        std::cout << "\nAFTER step - Ray 0:\n";
        //        std::cout << "  Position: (" << ray.position.x << ", " << ray.position.y << ")\n";
        //        std::cout << "  r=" << ray.r << ", theta=" << ray.theta << "\n";
        //        std::cout << "  Trail size: " << ray.trail.size() << "\n\n";
        //    }
        //}

        ////bind shader
        //mainShader.Use();
        //// Build your transformation matrix (translate, rotate, scale)
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(x, y, 0.0f));
        //model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

        //mainShader.SetMat4("u_Model", model);
        //mainShader.SetMat4("u_View", glm::mat4(1.0f));
        //mainShader.SetMat4("u_Projection", projection);
        //mainShader.SetVec4("u_Color", circleMesh.getColor());
        //circleMesh.draw_Circle();

        //// RENDER ALL LIGHT RAY TRAILS
        //for (const auto& ray : lightRays)
        //{
        //    if (ray.trail.size() > 1)  // Only render if there's a trail
        //    {
        //        renderTrail(ray.trail, mainShader, projection);
        //    }
        //}

        //triangle.draw();
        //quad.draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
