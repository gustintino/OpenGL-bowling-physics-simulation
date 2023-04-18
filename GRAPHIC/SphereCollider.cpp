#include "SphereCollider.h"
#include "GameObject.h"

#include "AABBCollider.h"
#include "PlaneCollider.h"

struct collisionInformation
{
	
};

SphereCollider::SphereCollider()
{
	radius = 0.0f;
	mesh = nullptr;
}

SphereCollider::SphereCollider(float _radius, Mesh* _mesh)
{
	radius = _radius;
	mesh = _mesh;
}

bool SphereCollider::CheckForCollision(SphereCollider* collider, CollisionInformation& collisionInfo)
{
	if (glm::distance(position, collider->position) <= (radius + collider->radius))
	{
		collisionInfo.collisionNormal = collider->position - this->position;
		collisionInfo.penetrationDepth = (this->radius + collider->radius) - glm::distance(position, collider->position);
		return true;
	}
	else
		return false;
}

bool SphereCollider::CheckForCollision(PlaneCollider* collider, CollisionInformation& collisionInfo)
{
	glm::vec3 vector = collider->position - position;
	float distance = glm::dot(vector, collider->normal);
	if (distance <= radius)
	{
		collisionInfo.collisionNormal = collider->normal;
		collisionInfo.penetrationDepth = radius - distance;
		return true;
	}
	else
		return false;
}

bool SphereCollider::CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo)
{
	return false;
}

bool SphereCollider::CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo)
{
	return _collider->CheckForCollision(this, collisionInfo);
}

void SphereCollider::CollisionResponse(CollisionInformation& collisionInfo)
{
	if(glm::pow(collisionInfo.object1->rigid->mass, -1.0f) < 99999.0f)
		collisionInfo.object1->rigid->position += (collisionInfo.penetrationDepth / 2 * (collisionInfo.collisionNormal));

	if (glm::pow(collisionInfo.object2->rigid->mass, -1.0f) < 99999.0f)
		collisionInfo.object2->rigid->position += (collisionInfo.penetrationDepth / 2 * (-collisionInfo.collisionNormal)) ;

	float impulseTop;
	float impulseBottom;

	impulseTop = glm::dot(-(collisionInfo.object1->rigid->linearVelocity - collisionInfo.object2->rigid->linearVelocity), collisionInfo.collisionNormal * (1 + 0.5f));
	impulseBottom = glm::dot(collisionInfo.collisionNormal, collisionInfo.collisionNormal * (collisionInfo.object1->rigid->mass + collisionInfo.object2->rigid->mass));
	float impulse = impulseTop / impulseBottom;

	glm::vec3 v1 = collisionInfo.object1->rigid->linearVelocity + impulse * collisionInfo.object1->rigid->mass * collisionInfo.collisionNormal;
	glm::vec3 v2 = collisionInfo.object2->rigid->linearVelocity - impulse * collisionInfo.object2->rigid->mass * collisionInfo.collisionNormal;

	collisionInfo.object1->rigid->linearVelocity = v1;
	collisionInfo.object2->rigid->linearVelocity = v2;
}

void SphereCollider::Draw(glm::vec3 _position, glm::vec3 _rotation)
{
	position = _position;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mesh->Draw(_position, _rotation, mesh->scale);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
