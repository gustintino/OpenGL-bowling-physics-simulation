#include "RigidBody3D.h"

RigidBody3D::RigidBody3D()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	linearAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = 0.0f;
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);

	
	orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	torque = glm::vec3(0.0f, 0.0f, 0.0f);
	momentOfInertia = glm::mat4(1.0f);
}

RigidBody3D::RigidBody3D(float _mass, glm::vec3 _position, float _moi, glm::vec3 _orientation)
{
	linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	linearAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);

	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	torque = glm::vec3(0.0f, 0.0f, 0.0f);

	mass = 1 / _mass; //optimization
	position = _position;

	//momentOfInertia = 1 / _moi; //optimization
	//momentOfInertia = 1 / (1.0f / 12.0f * _mass * 8.0f);

	//glm::vec3 c1((1.0f / 12.0f * _mass * 8.0f), 0.0f, 0.0f);
	//glm::vec3 c2(0.0f, (1.0f / 12.0f * _mass * 8.0f), 0.0f);
	//glm::vec3 c3(0.0f, 0.0f, (1.0f / 12.0f * _mass * 8.0f));	//cube

	glm::vec3 c1((2.0f / 5.0f) * mass, 0, 0);
	glm::vec3 c2(0, (2.0f / 5.0f) * mass, 0);
	glm::vec3 c3(0, 0, (2.0f / 5.0f) * mass );

	momentOfInertia = glm::mat3(c1, c2, c3);
	momentOfInertia = glm::inverse(momentOfInertia);


	orientation = _orientation;
	identity = glm::mat4(1.0f);
}

RigidBody3D::~RigidBody3D()
{
}

void RigidBody3D::Update(float deltaTime)
{

	//explicit euler
	//angularAcceleration = torque * momentOfInertia; // moi = 1 / moi;
	//angularVelocity = angularVelocity + angularAcceleration * deltaTime;
	//orientation = orientation + angularVelocity * deltaTime;
	//identity = glm::rotate(identity, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//identity = glm::rotate(identity, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//identity = glm::rotate(identity, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//totalLinearForce = glm::vec3(identity * glm::vec4(totalLinearForce, 1.0f));
	//linearAcceleration = totalLinearForce * mass; // mass = 1 / mass;
	//linearVelocity = linearVelocity + linearAcceleration * deltaTime;
	//position = position + linearVelocity * deltaTime;

	//verlet
	lastLinearAcceleration = linearAcceleration;
	position += linearVelocity * deltaTime + (0.5f * lastLinearAcceleration * (float)glm::pow(deltaTime, 2));
	linearAcceleration = totalLinearForce * mass;
	glm::vec3 averageLinearAccel = (lastLinearAcceleration + linearAcceleration) / 2.0f;
	linearVelocity += averageLinearAccel * deltaTime;

	identity = glm::rotate(identity, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	identity = glm::rotate(identity, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	identity = glm::rotate(identity, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	totalLinearForce = glm::vec3(identity * glm::vec4(totalLinearForce, 1.0f));

	lastAngularAcceleration = angularAcceleration;
	orientation += angularVelocity * deltaTime + (0.5f * lastAngularAcceleration * (float)glm::pow(deltaTime, 2));
	angularAcceleration = torque * momentOfInertia;
	glm::vec3 averageRotationalAccel = (lastAngularAcceleration + angularAcceleration) / 2.0f;
	angularVelocity += averageRotationalAccel * deltaTime;
}

void RigidBody3D::ResetTotalForces()
{
	totalLinearForce = glm::vec3(0.0f, 0.0f, 0.0f);
	torque = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody3D::AddForce(glm::vec3 forceStrength, glm::vec3 forcePosition)
{
	totalLinearForce += forceStrength;
	torque += glm::cross(forcePosition, forceStrength);
}
