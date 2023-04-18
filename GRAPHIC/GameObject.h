#pragma once
#include <map>

#include "Mesh.h"
#include "RigidBody3D.h"
#include "Collider.h"

class GameObject
{
public:
	Mesh* mesh;
	RigidBody3D* rigid;
	Collider* collider;

	static std::map<int, bool> keyMap;

	GameObject();
	GameObject(Mesh* _mesh, float _mass, glm::vec3 _position, float _moi, glm::vec3 _orientation, Collider* _collider);
	~GameObject();

	virtual void Draw();
	virtual void Update(float deltaTime);
};

