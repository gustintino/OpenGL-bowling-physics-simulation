#include "Collider.h"

#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "AABBCollider.h"

Collider::Collider()
{
	mesh = nullptr;
	position = glm::vec3(0, 0, 0);
}

Collider::~Collider()
{
}

Collider::Collider(glm::vec3 _position, Mesh* _mesh)
{
	position = _position;
	mesh = _mesh;
}

