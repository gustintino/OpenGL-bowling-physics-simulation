#include "AABBCollider.h"
#include "ObjectFromFile.h"

#include "SphereCollider.h"
#include "glm/gtx/rotate_vector.hpp"

AABBCollider::AABBCollider()
{
	extents = glm::vec3(0, 0, 0);
}

AABBCollider::~AABBCollider()
{
}

AABBCollider::AABBCollider(Mesh* _mesh)
{
	extents = glm::vec3(0, 0, 0);
	closestPoint = glm::vec3(0, 0, 0);

	position = glm::vec3(0,0,0);
	mesh = _mesh;

	ObjectFromFile* off = (ObjectFromFile*)mesh;

	for (int i = 0; i < off->vertices.size(); i++)
	{
		verts.emplace_back(off->vertices[i].position * off->scale);
	}

	repeatOnce = true;
	
}

bool AABBCollider::CheckForCollision(SphereCollider* _collider, CollisionInformation& collisionInfo)
{

	//x
	if (_collider->position.x >= max.x)
	{
		closestPoint.x = max.x;
	}
	else if (_collider->position.x <= min.x)
	{
		closestPoint.x = min.x;
	}
	else
	{
		closestPoint.x = _collider->position.x;
	}

	//y
	if (_collider->position.y >= max.y)
	{
		closestPoint.y = max.y;
	}
	else if (_collider->position.y <= min.y)
	{
		closestPoint.y = min.y;
	}
	else
	{
		closestPoint.y = _collider->position.y;
	}

	//z
	if (_collider->position.z >= max.z)
	{
		closestPoint.z = max.z;
	}
	else if (_collider->position.z <= min.z)
	{
		closestPoint.z = min.z;
	}
	else
	{
		closestPoint.z = _collider->position.z;
	}

	

	float a = glm::distance(closestPoint, _collider->position);
	if (a <= _collider->radius)
	{

		return true;
	}
	else
		return false;
}

bool AABBCollider::CheckForCollision(PlaneCollider* _collider, CollisionInformation& collisionInfo)
{
	return false;
}

bool AABBCollider::CheckForCollision(AABBCollider* _collider, CollisionInformation& collisionInfo)
{
	return false;
}

bool AABBCollider::CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo)
{
	return _collider->CheckForCollision(this, collisionInfo);
}

void AABBCollider::CollisionResponse(CollisionInformation& collisionInfo)
{
}

void AABBCollider::Draw(glm::vec3 _position, glm::vec3 _rotation)
{
	position = _position;

	//wip, not working
	max = verts[0];
	min = verts[0];

	for (int i = 0; i < verts.size(); i++)
	{
		if (_rotation != glm::vec3(0, 0, 0) && _rotation != oldRotation)
		{
			verts[i] = glm::rotateX(verts[i], _rotation.x);
			verts[i] = glm::rotateY(verts[i], _rotation.y);
			verts[i] = glm::rotateZ(verts[i], _rotation.z);

			oldRotation = _rotation;
		}

		if (verts[i].x > max.x)
			max.x = verts[i].x;
		if (verts[i].x < min.x)
			min.x = verts[i].x;

		if (verts[i].y > max.y)
			max.y = verts[i].y;
		if (verts[i].y < min.y)
			min.y = verts[i].y;

		if (verts[i].z > max.z)
			max.z = verts[i].z;
		if (verts[i].z < min.z)
			min.z = verts[i].z;

		
	}
	
	min += position;
	max += position;

	glm::vec3 newpos((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2);
	oldExtent = (max - min) / 2.0f;

	glm::vec3 newscale(max.x / min.x, max.y / min.y, max.z / min.z);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mesh->Draw(newpos, glm::vec3(0,0,0), mesh->scale);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
