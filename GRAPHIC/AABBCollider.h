#pragma once

#include "Collider.h"

class AABBCollider : public Collider
{
public:
	glm::vec3 extents;
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 closestPoint;
	glm::vec3 centreOffset;
	bool repeatOnce;
	glm::vec3 newScale;
	std::vector < glm::vec3> verts;
	glm::vec3 oldRotation;

	glm::vec3 oldExtent;
	glm::vec3 newExtent;

	AABBCollider();
	~AABBCollider();
	AABBCollider(Mesh* _mesh);

	bool CheckForCollision(SphereCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(PlaneCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo) override;
	bool CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo) override;

	void CollisionResponse(CollisionInformation& collisionInfo) override;

	virtual void Draw(glm::vec3 _position, glm::vec3 _rotation);
};

