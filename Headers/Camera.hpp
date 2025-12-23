#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    // Orbital camera parameters
    glm::vec3 target;      // Look-at point (black hole center)
    float radius;          // Distance from target
    float azimuth;         // Horizontal angle (radians)
    float elevation;       // Vertical angle (radians)
    float fov;             // Field of view

    // View constraints
    float minRadius = 50.0f;
    float maxRadius = 500.0f;
    float minElevation = -1.4f;  // ~-80 degrees
    float maxElevation = 1.4f;   // ~+80 degrees

    Camera(glm::vec3 target = glm::vec3(0.0f), float radius = 200.0f);

    // Get camera position in world space
    glm::vec3 getPosition() const;

    // Get view and projection matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    glm::mat4 getViewProjectionMatrix(float aspectRatio) const;

    // Input handling
    void processMouseDrag(float deltaX, float deltaY);
    void processMouseScroll(float delta);
    void processKeyboard(GLFWwindow* window, float deltaTime);
};