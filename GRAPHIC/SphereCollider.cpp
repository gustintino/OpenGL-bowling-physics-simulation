#include "SphereCollider.h"
#include "GameObject.h"

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
	//collisionInfo.collisionNormal = glm::normalize(collisionInfo.collisionNormal);
	collisionInfo.object1->mesh->ChangePosition(collisionInfo.penetrationDepth / 2 * (-collisionInfo.collisionNormal), glm::vec3(0, 0, 0), collisionInfo.object1->mesh->scale);
	collisionInfo.object2->mesh->ChangePosition(collisionInfo.penetrationDepth / 2 * (collisionInfo.collisionNormal), glm::vec3(0,0,0), collisionInfo.object2->mesh->scale);

	float impulseTop;
	float impulseBottom;

	impulseTop = glm::dot(-(collisionInfo.object1->rigid->linearVelocity - collisionInfo.object2->rigid->linearVelocity), collisionInfo.collisionNormal * (1 + 0.5f));
	impulseBottom = glm::dot(collisionInfo.collisionNormal, collisionInfo.collisionNormal * (1 / collisionInfo.object1->rigid->mass + 1 / collisionInfo.object2->rigid->mass));
	float impulse = impulseTop / impulseBottom;

	glm::vec3 v1 = collisionInfo.object1->rigid->linearVelocity + impulse / collisionInfo.object1->rigid->mass * collisionInfo.collisionNormal;
	glm::vec3 v2 = collisionInfo.object2->rigid->linearVelocity - impulse / collisionInfo.object2->rigid->mass * collisionInfo.collisionNormal;

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
