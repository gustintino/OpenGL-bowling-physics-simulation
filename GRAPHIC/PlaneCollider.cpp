#include "PlaneCollider.h"

#include "SphereCollider.h"
#include "AABBCollider.h"

PlaneCollider::PlaneCollider()
{
	mesh = nullptr;
	normal = glm::vec3(0, 1, 0);
	position = glm::vec3(0, 0, 0);
}

PlaneCollider::~PlaneCollider()
{
}

PlaneCollider::PlaneCollider(Mesh* _mesh, glm::vec3 _normal)
{
	mesh = _mesh;
	normal = _normal;
	position = glm::vec3(0, 0, 0);
}

bool PlaneCollider::CheckForCollision(SphereCollider* collider, CollisionInformation& collisionInfo)
{
	glm::vec3 vector = collider->position - position;
	float distance = glm::dot(vector, normal);
	if (distance <= collider->radius)
	{
		collisionInfo.collisionNormal = normal;
		collisionInfo.penetrationDepth = collider->radius - distance;
		return true;
	}
	else
		return false;
}

bool PlaneCollider::CheckForCollision(PlaneCollider* _collider, CollisionInformation& collisionInfo)
{
	return false;
}

bool PlaneCollider::CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo)
{
	return false;
}

bool PlaneCollider::CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo)
{
	return _collider->CheckForCollision(this, collisionInfo);
}

void PlaneCollider::CollisionResponse(CollisionInformation& collisionInfo)
{
}

void PlaneCollider::Draw(glm::vec3 _position, glm::vec3 _rotation)
{
	position = _position;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//mesh->Draw(_position, _rotation, glm::vec3(1, 1, 1));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
