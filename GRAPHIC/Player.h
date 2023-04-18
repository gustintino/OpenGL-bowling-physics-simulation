#pragma once

#include "GameObject.h"
#include "Collider.h"

class Player : public GameObject
{
public:
	float speed;

	Player(Mesh* _mesh, float _mass, glm::vec3 _position, float _speed, Collider* _collider);
	~Player();
	void Update(float deltaTime);
};