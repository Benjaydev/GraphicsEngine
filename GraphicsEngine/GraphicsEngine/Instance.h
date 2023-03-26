#pragma once
#include "Scene.h"

class Instance
{
public:
	Instance(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader);
	Instance(Mesh* _mesh, aie::ShaderProgram* _shader);
	
	void Draw(Scene* scene, bool rebind = true);

	glm::mat4 MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
	void SetTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

	float SqrMagnitude(glm::vec3 pos1, glm::vec3 pos2);

	aie::ShaderProgram* GetShader() { return shader; }

	glm::vec3 GetPosition() {
		return glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
	}

protected:
	glm::mat4 transform;
	Mesh* mesh;
	aie::ShaderProgram* shader;

	
};
