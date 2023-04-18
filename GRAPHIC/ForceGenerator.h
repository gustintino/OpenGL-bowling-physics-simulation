#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "GameObject.h"

class ForceGenerator
{
public:
	glm::vec3 forceToApply;
	std::vector<GameObject*> objects;

	ForceGenerator(glm::vec3 _forceToApply);
	~ForceGenerator();
	void Apply();
	void AddObject(GameObject* _gameObject);
};

