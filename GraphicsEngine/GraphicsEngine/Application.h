#pragma once
#include "glad.h"  
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <imgui.h>
#include "Texture.h"
#include "Instance.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

using aie::Gizmos;
using aie::ShaderProgram;

class Application
{
public:


	virtual bool Startup();
	virtual void Update();
	virtual void Draw();
	virtual void Shutdown();

	bool applicationIsActive = false;

	static Application* GetInstance() { return instance; };

	// Set up mouse input 
	static void SetMousePosition(GLFWwindow* window, double x, double y);
	vec2 GetMousePosition() { return mousePosition; }
	vec2 GetMouseDelta(){ return mousePosition - lastMousePosition; }

	static Application* instance;

protected:
	GLFWwindow* window;

	const int windowWidth = 1280;
	const int windowHeight = 720;

	ShaderProgram shader;
	ShaderProgram phongShader;
	ShaderProgram normalMapShader;

	Scene* scene;



	vec2 mousePosition;
	vec2 lastMousePosition;

	float deltaTime = 0;
	int fps = 0;

private:
	std::chrono::steady_clock::time_point lastTime;

};

