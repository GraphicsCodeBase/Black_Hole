#include <Camera.hpp>

Camera::Camera(glm::vec3 target, float radius)
    : target(target), radius(radius), azimuth(0.0f), elevation(0.3f), fov(45.0f)
{
}

glm::vec3 Camera::getPosition() const
{
    float clampedElevation = glm::clamp(elevation, minElevation, maxElevation);

    float x = radius * cos(clampedElevation) * cos(azimuth);
    float y = radius * sin(clampedElevation);
    float z = radius * cos(clampedElevation) * sin(azimuth);

    return target + glm::vec3(x, y, z);
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(getPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

glm::mat4 Camera::getViewProjectionMatrix(float aspectRatio) const
{
    return getProjectionMatrix(aspectRatio) * getViewMatrix();
}

void Camera::processMouseDrag(float deltaX, float deltaY)
{
    azimuth -= deltaX * 0.005f;  // Sensitivity
    elevation += deltaY * 0.005f;
    elevation = glm::clamp(elevation, minElevation, maxElevation);
}

void Camera::processMouseScroll(float delta)
{
    radius -= delta * 10.0f;
    radius = glm::clamp(radius, minRadius, maxRadius);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    float speed = 50.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        radius -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        radius += speed;

    radius = glm::clamp(radius, minRadius, maxRadius);
}