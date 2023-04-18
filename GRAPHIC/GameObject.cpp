#include "GameObject.h"

#include "GLFW/glfw3.h"

std::map<int, bool> GameObject::keyMap;

GameObject::GameObject()
{
	mesh = nullptr;
	rigid = nullptr;
	collider = nullptr;
}

GameObject::GameObject(Mesh* _mesh, float _mass, glm::vec3 _position, float _moi, glm::vec3 _orientation, Collider* _collider)
{
	mesh = _mesh;
	rigid = new RigidBody3D(_mass, _position, _moi, _orientation);
	collider = _collider;
}

GameObject::~GameObject()
{

}

void GameObject::Draw()
{
	mesh->Draw(rigid->position, rigid->orientation, mesh->scale);

	if(collider != nullptr)
		collider->Draw(rigid->position, rigid->orientation);
}

void GameObject::Update(float deltaTime)
{
	
	//rigid->AddForce(glm::vec3(0.0f, 0.0f, 0.0f));
	rigid->Update(deltaTime);
	rigid->ResetTotalForces();
			
	rigid->linearVelocity *= pow(0.3, deltaTime);


	//particle.position += glm::vec3(0, .1, 0) * deltaTime;
	//mesh->ChangePosition(glm::vec3(0.0f, 0.1f, 0.0f) * deltaTime);
}
