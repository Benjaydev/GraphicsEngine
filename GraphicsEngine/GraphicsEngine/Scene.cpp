#pragma once
#include "Scene.h"
#include "Instance.h"
#include <iostream>
#include <iterator>
Scene::Scene(Camera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight, GLFWwindow* _window)
{
	camera = _camera;
	windowSize = _windowSize;
	sunLight = _light;
	ambientLight = _ambientLight;

	window = _window;
}

Scene::Scene(glm::vec2 _windowSize, GLFWwindow* _window)
{
	camera = new Camera();
	windowSize = _windowSize;
	sunLight = Light();

	sunLight.direction = glm::normalize(vec3(-1));
	sunLight.colour = { 1, 1, 1 };
	ambientLight = { 0.25f, 0.25f, 0.25f };

	window = _window;
}

bool Scene::Start()
{
	if (!renderTarget.initialise(4, windowSize.x, windowSize.y)) {
		printf("Render Target Error!\n");
		return false;
	}
}




Scene::~Scene()
{
	// Search all batches
	for (std::map<aie::ShaderProgram*, std::list<Instance*>>::iterator it = instances.begin(); it != instances.end(); it++)
	{
		// Get batch list
		std::list<Instance*> instList = instances[it->first];
		for (auto inIt = instList.begin(); inIt != instList.end(); inIt++) {
			// Delete instance
			delete* inIt;
		}		

	}
	instances.clear();
}

void Scene::AddInstance(Instance* instance)
{
	// This instance will be drawing a render target, 
	// so it must be drawn after all other objects in the scene in order to have a fully complete texture
	if (instance->drawTextureFromRenderTarget) {
		postRenderTargets.push_back(instance);
		return;
	}
	// Shader has not been batched yet
	aie::ShaderProgram* shader = instance->GetShader();
	if (instances.find(shader) == instances.end()) {
		instances.insert( std::make_pair( shader, std::list<Instance*>() ) );
	}
	instances[shader].push_back(instance);


}

void Scene::Draw()
{
	aie::Gizmos::clear();

	aie::Gizmos::addTransform(glm::mat4(1));

    glm::mat4 pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) * camera->GetViewMatrix();


    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; ++i) {
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10),
            vec3(-10 + i, 0, -10),
            i == 10 ? white : black);

		aie::Gizmos::addLine(vec3(10, 0, -10 + i),
            vec3(-10, 0, -10 + i),
            i == 10 ? white : black);
    }



	
	renderTarget.hasBinded = false;
	// Search all batches
	for (std::map<aie::ShaderProgram*, std::list<Instance*>>::reverse_iterator it = instances.rbegin(); it != instances.rend(); it++)
	{
		// Only rebind shaders on the first of each batch
		bool rebind = true;

		std::list<Instance*> instList = instances[it->first];
		for (auto inIt = instList.begin(); inIt != instList.end(); inIt++) {
			Instance* instance = *inIt;
			instance->Draw(this, rebind);
			rebind = true;
		}

	}

	for(std::list<Instance*>::iterator it = postRenderTargets.begin(); it != postRenderTargets.end(); it++)
	{
		(*it)->Draw(this, true);
	}


	for (int i = 0; i < MAX_LIGHTS && i < pointLights.size(); i++)
	{
		pointLightPositions[i] = pointLights[i].direction;
		pointLightColours[i] = pointLights[i].colour * pointLights[i].intensity;

		if (showPointLightsDebug) {
			aie::Gizmos::addSphere(pointLights[i].direction, pointLights[i].intensity / 10, 10, 10, vec4(pointLights[i].colour.r, pointLights[i].colour.g, pointLights[i].colour.b, 0.25));
		}


	}

	aie::Gizmos::draw(pv);
}

void Scene::Update(float deltaTime)
{
	camera->Update(deltaTime, window);

	// query time since application started 
	float time = glfwGetTime();

	if (spinLight) {
		// rotate light 
		sunLight.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	}
}

