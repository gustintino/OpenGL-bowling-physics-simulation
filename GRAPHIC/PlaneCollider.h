#pragma once

#include "glm/glm.hpp"
#include "Collider.h"

class PlaneCollider : public Collider
{
public:
	glm::vec3 normal;

	PlaneCollider();
	~PlaneCollider();
	PlaneCollider(Mesh* _mesh, glm::vec3 _normal);
	bool CheckForCollision(SphereCollider* collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(PlaneCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo) override;

	void CollisionResponse(CollisionInformation& collisionInfo) override;

	void Draw(glm::vec3 _position, glm::vec3 _rotation);
};

