#include <iostream>
#include <glm/glm.hpp>
#include <vector>
//physics constants
const double G = 1.0;           // Gravitational constant (pixel units)
const double C = 100.0;         // Speed of light (pixels/second)

struct BlackHole
{
	glm::vec2 position;
	double mass;
	double schwarzschildRadius;

	BlackHole(glm::vec2 pos, double m );
};
struct RayState
{
	float r;				//distance from black hole center 
	float theta;			//angle from the black hole center.(radians)
	float dr_dlambda;		//radial velocity. (moving toward/away)
	float dtheta_dlambda;    //angular velocity. (rotating around)

	//allowing for adding states together
	//For adding states together (for RK4 math)
	RayState operator+(const RayState& other) const
	{
		return RayState{
			r + other.r,
			theta + other.theta,
			dr_dlambda + other.dr_dlambda,
			dtheta_dlambda + other.dtheta_dlambda
		};
	}

	// Allow multiplying state by a scalar (for RK4 math)
	RayState operator*(float scalar) const
	{
		return RayState{
			r * scalar,
			theta * scalar,
			dr_dlambda * scalar,
			dtheta_dlambda* scalar
		};
	}

};
//this would hold the state of a single light ray.
struct LightRay
{
	glm::vec2 position;
	glm::vec2 velocity;

	std::vector<glm::vec2> trail;

	//physics fields
	float r;				//distance from black hole center 
	float theta;			//angle from the black hole center.(radians)
	float dr_dlambda;		//radial velocity. (moving toward/away)
	float dtheta_dlambda;    //angular velocity. (rotating around)

	// Accelerations (from geodesic equations)
	float d2r_dlambda2;         // Radial acceleration
	float d2theta_dlambda2;     // Angular acceleration
	// Control
	bool active;                // Becomes false at event horizon

	void step(float deltaTime, const BlackHole& blackHole);
	void initialize(glm::vec2 startPos, glm::vec2 startVel, const BlackHole& blackHole);
};

//coordinate conversion helpers 
glm::vec2 cartesianToPolar(glm::vec2 pos, glm::vec2 blackHole);
glm::vec2 polarToCartesian(float r, float theta, glm::vec2 blackHole);

//Geodesic calculations
void calculateAccelerations(LightRay& ray, const BlackHole& bh);

// RK4 helper functions
RayState getRayState(const LightRay& ray);
void setRayState(LightRay& ray, const RayState& state);
