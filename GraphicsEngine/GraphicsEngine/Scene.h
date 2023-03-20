#pragma once
#include "glad.h"  
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include <list>

class Instance;

class Scene
{
public:
	Scene(Camera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3
		_ambientLight, GLFWwindow* _window);
	Scene(glm::vec2 _windowSize, GLFWwindow* _window);

	~Scene();
	void AddInstance(Instance* instance);

	void Draw();
	void Update(float deltaTime);

	Camera* camera;
	glm::vec2 windowSize;
	Light light;
	glm::vec3 ambientLight;
	std::list<Instance*> instances;
	bool spinLight = true;

	GLFWwindow* window;

};