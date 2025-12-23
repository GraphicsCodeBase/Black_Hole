#include <BlackHole.hpp>
#include <cmath>
BlackHole::BlackHole(glm::vec2 pos, double m)
{
    //pass in params.
    position = pos;
    mass = m;

    //calculate 
    schwarzschildRadius = (2 * G * mass) / (C * C);
}

//we would use RK4 to step the light ray forward in time.
void LightRay::step(float deltaTime, const BlackHole& blackHole)
{
    // Save initial state
    RayState initial = getRayState(*this);

    // ===== k1: Evaluate at current position =====
    calculateAccelerations(*this, blackHole);
    RayState k1 = RayState{
        dr_dlambda,           // derivative of r
        dtheta_dlambda,       // derivative of theta
        d2r_dlambda2,         // derivative of dr_dlambda
        d2theta_dlambda2      // derivative of dtheta_dlambda
    };

    // ===== k2: Evaluate at midpoint using k1 =====
    setRayState(*this, initial + k1 * (deltaTime / 2.0f));
    calculateAccelerations(*this, blackHole);
    RayState k2 = RayState{
        dr_dlambda,
        dtheta_dlambda,
        d2r_dlambda2,
        d2theta_dlambda2
    };

    // ===== k3: Evaluate at midpoint using k2 =====
    setRayState(*this, initial + k2 * (deltaTime / 2.0f));
    calculateAccelerations(*this, blackHole);
    RayState k3 = RayState{
        dr_dlambda,
        dtheta_dlambda,
        d2r_dlambda2,
        d2theta_dlambda2
    };

    // ===== k4: Evaluate at endpoint using k3 =====
    setRayState(*this, initial + k3 * deltaTime);
    calculateAccelerations(*this, blackHole);
    RayState k4 = RayState{
        dr_dlambda,
        dtheta_dlambda,
        d2r_dlambda2,
        d2theta_dlambda2
    };

    // ===== Combine with RK4 weights =====
    RayState finalState = initial + (k1 + k2 * 2.0f + k3 * 2.0f + k4) * (deltaTime / 6.0f);
    setRayState(*this, finalState);

    // Update Cartesian position for rendering
    glm::vec2 cartesian = polarToCartesian(r, theta, blackHole.position);
    position = cartesian;

    // Add to trail
    trail.push_back(position);

    // EVENT HORIZON CHECK (ADD THIS)
    if (r <= blackHole.schwarzschildRadius)
    {
        active = false;  // Ray hit the event horizon - deactivate it
    }
}

void LightRay::initialize(glm::vec2 startPos, glm::vec2 startVel, const BlackHole& blackHole)
{
    // Set Cartesian coordinates
    position = startPos;
    velocity = startVel;

    // Convert position to polar
    glm::vec2 polar = cartesianToPolar(startPos, blackHole.position);
    r = polar.x;
    theta = polar.y;

    // Convert velocity to polar
    // We need to project velocity onto radial and tangential directions
    glm::vec2 toRay = startPos - blackHole.position;  // Vector from BH to ray
    float distance = std::sqrt(toRay.x * toRay.x + toRay.y * toRay.y);

    if (distance > 0.0f)
    {
        glm::vec2 radialDir = toRay / distance;  // Unit vector pointing away from BH
        glm::vec2 tangentialDir = glm::vec2(-radialDir.y, radialDir.x);  // Perpendicular

        // Project velocity onto these directions
        dr_dlambda = glm::dot(startVel, radialDir);           // Radial velocity
        dtheta_dlambda = glm::dot(startVel, tangentialDir) / r;  // Angular velocity
    }
    else
    {
        dr_dlambda = 0.0f;
        dtheta_dlambda = 0.0f;
    }

    // Initialize accelerations to zero (will be calculated in first step)
    d2r_dlambda2 = 0.0f;
    d2theta_dlambda2 = 0.0f;

    // Ray starts active
    active = true;

    // Clear trail and add starting position
    trail.clear();
    trail.push_back(position);
}

glm::vec2 cartesianToPolar(glm::vec2 pos, glm::vec2 blackHole)
{
	//calculate offset from the black hole.
    float dx = pos.x - blackHole.x;
	float dy = pos.y - blackHole.y;

    //calculate r from the using Pythagorean theorem
	float r = std::sqrt(dx * dx + dy * dy);

    //calculate theta 
	float theta = std::atan2(dy, dx); //atan2 handles quadrant correctly.

	return glm::vec2(r, theta);
}

glm::vec2 polarToCartesian(float r, float theta, glm::vec2 blackHole)
{
    // Step 1: Convert polar to Cartesian offset using trig
    float x = r * std::cos(theta);
    float y = r * std::sin(theta);

    // Step 2: Add black hole position to get world coordinates
    return glm::vec2(blackHole.x + x, blackHole.y + y);
}

void calculateAccelerations(LightRay& ray, const BlackHole& bh)
{
    //extract the current state of the ray.
    float r = ray.r;
    float dr = ray.dr_dlambda;
    float dtheta = ray.dtheta_dlambda;
    float Rs = bh.schwarzschildRadius;

    // Equation 1: Angular acceleration
    // Formula: d²θ/dλ² = -(2/r) × (dr/dλ) × (dθ/dλ)
    // Translate to C++:
    ray.d2theta_dlambda2 = -(2.0f / r) * dr * dtheta;

    // Equation 2: Radial acceleration
    // Formula: d²r/dλ² = (c² × Rs)/(2 × r²) + r × (dθ/dλ)²
    // Translate to C++:
    ray.d2r_dlambda2 = -(C * C * Rs) / (2.0f * r * r) + r * dtheta * dtheta;
}

RayState getRayState(const LightRay& ray)
{
    return RayState{ ray.r, ray.theta, ray.dr_dlambda, ray.dtheta_dlambda };
}

void setRayState(LightRay& ray, const RayState& state)
{
    ray.r = state.r;
    ray.theta = state.theta;
    ray.dr_dlambda = state.dr_dlambda;
    ray.dtheta_dlambda = state.dtheta_dlambda;
}