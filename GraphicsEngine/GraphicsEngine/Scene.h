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
#include <unordered_map>
#include <utility>
#include <vector>
#define MAX_LIGHTS 4

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


	int GetNumLights() { return (int)pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &pointLightPositions[0]; }
	glm::vec3* GetPointLightColours() { return &pointLightColours[0]; }
	std::vector<Light>& GetPointLights() { return pointLights; }



	Camera* camera;
	glm::vec2 windowSize;
	Light sunLight;
	std::vector<Light> pointLights;
	glm::vec3 pointLightPositions[MAX_LIGHTS];
	glm::vec3 pointLightColours[MAX_LIGHTS];
	glm::vec3 ambientLight;
	std::unordered_map<aie::ShaderProgram*, std::list<Instance*>> instances;
	bool spinLight = true;
	bool showPointLightsDebug = true;

	GLFWwindow* window;


protected:

};