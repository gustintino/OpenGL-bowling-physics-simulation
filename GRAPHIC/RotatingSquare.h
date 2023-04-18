#pragma once

#include "RigidBody3D.h"
#include "GameObject.h"
#include "glm/glm.hpp"

class RotatingSquare : public GameObject
{
public:
	float speed;
	RigidBody3D rigidBody;

	RotatingSquare();
	RotatingSquare(Mesh* _mesh, float _mass, float _inertia, float _speed, glm::vec3 _position);
	~RotatingSquare();

	void Update(float deltaTime);
	void Draw();

};

