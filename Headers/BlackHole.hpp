#include <iostream>
#include <glm/glm.hpp>
#include <vector>
//physics constants
const double G = 6.67430e-11;   // Gravitational constant
const double C = 299792458.0;   // Speed of light

struct BlackHole
{
	glm::vec2 position;
	double mass;
	double schwarzschildRadius;

	BlackHole(glm::vec2 pos, double m );
};

//this would hold the state of a single light ray.
struct LightRay
{
	glm::vec2 position;
	glm::vec2 velocity;

	std::vector<glm::vec2> trail;

	void step(float deltaTime);
};