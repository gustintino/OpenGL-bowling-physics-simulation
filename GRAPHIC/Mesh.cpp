#include "Mesh.h"

Mesh::Mesh()
{
	modelLocation = 0;
}

Mesh::~Mesh()
{
}

void Mesh::ChangePosition(glm::vec3 _translation, glm::vec3 _rotation,  glm::vec3 _scale)
{
	position = _translation;

	model = glm::translate(model, _translation);

	model = glm::rotate(model, _rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, _rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, _rotation.z, glm::vec3(0, 0, 1));

	model = glm::scale(model, _scale);
}
