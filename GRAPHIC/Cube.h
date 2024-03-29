#pragma once

#include "GL/gl3w.h"
//#include "values.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Cube : public Mesh
{
public:

	Cube();
	~Cube();
	Cube(glm::vec4 _color, glm::vec3 _scale);

	void Draw(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) override;
	//void ChangePosition(glm::vec3 offset) override;
};

