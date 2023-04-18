#pragma once

#include "glm/glm.hpp"
#include "Mesh.h"
#include "values.h"


class SphereCollider;
class PlaneCollider;
class AABBCollider;
class GameObject;

struct CollisionInformation
{
	GameObject* object1;
	GameObject* object2;
	glm::vec3 collisionNormal;
	float penetrationDepth;
};

class Collider
{
public:

	glm::vec3 position;
	Mesh* mesh;
	float CoefficientOfRestitution;

	Collider();
	~Collider();
	Collider(glm::vec3 _position, Mesh* _mesh);

	virtual bool CheckForCollision(SphereCollider* _collider, CollisionInformation& collisionInfo) = 0;
	virtual bool CheckForCollision(PlaneCollider* _collider, CollisionInformation& collisionInfo) = 0;
	virtual bool CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo) = 0;
	virtual bool CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo) = 0;

	virtual void CollisionResponse(CollisionInformation& collisionInfo) = 0;

	virtual void Draw(glm::vec3 _position, glm::vec3 _rotation) = 0;
};


