#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

class RigidBody3D
{
public:
	//linear
	glm::vec3 position;
	glm::vec3 linearVelocity;
	glm::vec3 linearAcceleration;
	glm::vec3 totalLinearForce;
	glm::vec3 lastLinearAcceleration;
	float mass;

	//rotational
	glm::vec3 orientation;
	glm::vec3 angularVelocity;
	glm::vec3 angularAcceleration;
	glm::vec3 lastAngularAcceleration;
	glm::vec3 torque;
	//float momentOfInertia;
	glm::mat3 momentOfInertia;
	
	glm::mat4 identity;

	RigidBody3D();
	RigidBody3D(float _mass, glm::vec3 _position, float _moi, glm::vec3 orientation);
	~RigidBody3D();

	void Update(float deltaTime);
	void ResetTotalForces();
	void AddForce(glm::vec3 forceStrength, glm::vec3 forcePosition);


};

