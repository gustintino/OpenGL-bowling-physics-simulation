#include "Particle.h"

Particle::Particle()
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);

	mass = 0.0f;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Particle::Particle(float _mass, glm::vec3 _position)
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);

	mass = 1 / _mass;
	position = _position;
}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	acceleration = totalLinearForce * mass;

	velocity = velocity + acceleration * deltaTime;
	position = position + velocity * deltaTime;
	//velocity *= pow(0.0f, deltaTime);
}

void Particle::ResetTotalForce()
{
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Particle::AddForce(glm::vec3 force)
{
	totalLinearForce += force;
}
