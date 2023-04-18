#pragma once

#include "glm/glm.hpp"


class Particle
{
public:
	glm::vec3 totalLinearForce;
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	Particle();
	Particle(float _mass, glm::vec3 _position);
	~Particle();

	void Update(float deltaTime);
	void ResetTotalForce();
	void AddForce(glm::vec3 force);
};

