#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(glm::vec3 _forceToApply)
{
	forceToApply = _forceToApply;
}

ForceGenerator::~ForceGenerator()
{
}

void ForceGenerator::Apply()
{
	for (int i = 0; i < objects.size(); i++)
	{
		//objects[i]->particle.ResetTotalForce();
		objects[i]->rigid->AddForce(forceToApply, glm::vec3(0,1,0));
		//objects[i]->particle.Update(0.016f);
	}
}


void ForceGenerator::AddObject(GameObject* _gameObject)
{
	objects.push_back(_gameObject);
}
