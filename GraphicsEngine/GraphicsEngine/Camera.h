#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct GLFWwindow{};

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	mat4 GetViewMatrix();
	mat4 GetProjectionMatrix(float w, float h);
	vec3 GetPosition() { return position; }
	void Update(float deltaTime, GLFWwindow* window);

private:
	float theta = 0;
	float phi = 0;
	vec3 position = vec3(-10, 2, 0);
};

