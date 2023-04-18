#include "RotatingSquare.h"

RotatingSquare::RotatingSquare() : GameObject()
{
	speed = 0.0f;
	//rigidbody = null
}

RotatingSquare::RotatingSquare(Mesh* _mesh, float _mass, float _inertia, float _speed, glm::vec3 _position) : 
	GameObject(_mesh, _mass, _position, 0.0f, glm::vec3(0,0,0), nullptr)
{
	speed = _speed;


	rigidBody = RigidBody3D(_mass, _position, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));

}

RotatingSquare::~RotatingSquare()
{
}

void RotatingSquare::Update(float deltaTime)
{
	rigidBody.AddForce(glm::vec3(-0.1f, 0.1f, 0.0f), glm::vec3(2.0f, 3.0f, 1.0f));

	rigidBody.Update(deltaTime);
	rigidBody.ResetTotalForces();

	rigidBody.angularVelocity *= pow(0.7f, deltaTime);
}

void RotatingSquare::Draw()
{
	mesh->Draw(rigidBody.position, rigidBody.orientation, glm::vec3(1,1,1));
}
