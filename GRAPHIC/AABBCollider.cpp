#include "AABBCollider.h"
#include "ObjectFromFile.h"

#include "GameObject.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
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
		collisionInfo.collisionNormal = _collider->position - this->position;
		collisionInfo.penetrationDepth = _collider->radius - a;
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
	if (!(min.x > _collider->max.x || _collider->min.x > max.x))
	{
		if (!(min.y > _collider->max.y || _collider->min.y > max.y))
		{
			if (!(min.z > _collider->max.z || _collider->min.z > max.z))
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


				//x
				if (position.x >= _collider->max.x)
				{
					_collider->closestPoint.x = _collider->max.x;
				}
				else if (position.x <= _collider->min.x)
				{
					_collider->closestPoint.x = _collider->min.x;
				}
				else
				{
					_collider->closestPoint.x = position.x;
				}

				//y
				if (position.y >= _collider->max.y)
				{
					_collider->closestPoint.y = _collider->max.y;
				}
				else if (position.y <= _collider->min.y)
				{
					_collider->closestPoint.y = _collider->min.y;
				}
				else
				{
					_collider->closestPoint.y = position.y;
				}

				//z
				if (position.z >= _collider->max.z)
				{
					_collider->closestPoint.z = _collider->max.z;
				}
				else if (position.z <= _collider->min.z)
				{
					_collider->closestPoint.z = _collider->min.z;
				}
				else
				{
					_collider->closestPoint.z = position.z;
				}



				collisionInfo.penetrationDepth = glm::distance(closestPoint, _collider->closestPoint) / 10;
				collisionInfo.collisionNormal = _collider->position - position;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool AABBCollider::CheckForCollision(Collider* _collider, CollisionInformation& collisionInfo)
{
	return _collider->CheckForCollision(this, collisionInfo);
}

void AABBCollider::CollisionResponse(CollisionInformation& collisionInfo)
{
	if (glm::pow(collisionInfo.object1->rigid->mass, -1.0f) < 99999.0f)
		collisionInfo.object1->rigid->position += (collisionInfo.penetrationDepth / 2 * (collisionInfo.collisionNormal));
	
	if (glm::pow(collisionInfo.object2->rigid->mass, -1.0f) < 99999.0f)
		collisionInfo.object2->rigid->position += (collisionInfo.penetrationDepth / 2 * (-collisionInfo.collisionNormal));

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

void AABBCollider::Draw(glm::vec3 _position, glm::vec3 _rotation)
{
	position = glm::vec3(_position.x, 0, _position.z);

	//wip, not working
	max = verts[0];
	min = verts[0];

	for (int i = 0; i < verts.size(); i++)
	{
		//if (_rotation != glm::vec3(0, 0, 0) && _rotation != oldRotation)
		//{
		//	verts[i] = glm::rotateX(verts[i], _rotation.x);
		//	verts[i] = glm::rotateY(verts[i], _rotation.y);
		//	verts[i] = glm::rotateZ(verts[i], _rotation.z);
		//
		//	oldRotation = _rotation;
		//}

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

	//glm::vec3 newpos((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2);
	//oldExtent = (max - min) / 2.0f;
	//
	//glm::vec3 newscale(max.x / min.x, max.y / min.y, max.z / min.z);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mesh->Draw(position, glm::vec3(0, 0, 0), mesh->scale);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
