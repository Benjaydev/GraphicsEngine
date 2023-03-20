#pragma once
#include "Scene.h"

class Instance
{
public:
	Instance(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader);
	
	void Draw(Scene* scene);

	glm::mat4 MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
protected:
	glm::mat4 transform;
	Mesh* mesh;
	aie::ShaderProgram* shader;

	
};
