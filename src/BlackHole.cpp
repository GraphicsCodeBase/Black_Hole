#include <BlackHole.hpp>

BlackHole::BlackHole(glm::vec2 pos, double m)
{
    //pass in params.
    position = pos;
    mass = m;

    //calculate 
    schwarzschildRadius = (2 * G * mass) / (C * C);
}

void LightRay::step(float deltaTime)
{
    // Update position based on velocity
    position += velocity * deltaTime;
    // Add current position to the trail
    trail.push_back(position);
}
