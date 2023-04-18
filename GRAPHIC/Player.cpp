#include "Player.h"
#include "GLFW/glfw3.h"


Player::Player(Mesh* _mesh, float _mass, glm::vec3 _position, float _speed, Collider* _collider) :
	GameObject(_mesh, _mass, _position, 0.0f, glm::vec3(0,0,0), _collider)
{
	speed = _speed;
}

Player::~Player()
{

}

void Player::Update(float deltaTime) // add input function
{
	

	if (GameObject::keyMap[GLFW_KEY_W] == true)
	{
		rigid->AddForce(glm::vec3(0.0f, 0.2f, 0.0f) * speed, glm::vec3(0, 0, 0));
	}
	if (GameObject::keyMap[GLFW_KEY_S] == true)
	{
		rigid->AddForce(glm::vec3(0.0f, -0.2f, 0.0f) * speed, glm::vec3(0, 0, 0));
	}
	if (GameObject::keyMap[GLFW_KEY_A] == true)
	{
		rigid->AddForce(glm::vec3(0.2f, 0.0f, 0.0f) * speed, glm::vec3(0, 0, 0));
	}
	if (GameObject::keyMap[GLFW_KEY_D] == true)
	{
		rigid->AddForce(glm::vec3(-0.2f, 0.0f, 0.0f) * speed, glm::vec3(0, 0, 0));
	}

	rigid->Update(deltaTime);
	rigid->ResetTotalForces();
	rigid->linearVelocity *= pow(0.1, deltaTime); // affects gravity
}

