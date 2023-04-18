#pragma once

#include "glm/glm.hpp"
#include "Collider.h"

class PlaneCollider;
class AABBCollider;

struct collisionInformation;

class SphereCollider : public Collider
{
public:
	float radius;
	std::vector <collisionInformation> info;
	

	SphereCollider();
	SphereCollider(float _radius, Mesh* _mesh);

	bool CheckForCollision(SphereCollider* collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(PlaneCollider* collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo) override;

	void CollisionResponse(CollisionInformation& collisionInfo) override;

	void Draw(glm::vec3 _position, glm::vec3 _rotation);
};

