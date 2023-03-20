#pragma once
#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight, GLFWwindow* _window)
{
	camera = _camera;
	windowSize = _windowSize;
	light = _light;
	ambientLight = _ambientLight;

	window = _window;
}

Scene::Scene(glm::vec2 _windowSize, GLFWwindow* _window)
{
	camera = new Camera();
	windowSize = _windowSize;
	light = Light();

	light.direction = glm::normalize(vec3(-1));
	light.colour = { 1, 1, 1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	window = _window;
}
Scene::~Scene()
{
	for (auto it = instances.begin(); it != instances.end(); it++)
		delete* it;
}

void Scene::AddInstance(Instance* instance)
{
	instances.push_back(instance);
}

void Scene::Draw()
{
	for (auto it = instances.begin(); it != instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::Update(float deltaTime)
{
	camera->Update(deltaTime, window);


	// query time since application started 
	float time = glfwGetTime();

	if (spinLight) {
		// rotate light 
		light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	}
}
