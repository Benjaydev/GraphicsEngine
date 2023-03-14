#pragma once
#include "Camera.h"
#include "Application.h"
#include <GLFW/glfw3.h> 
#include <iostream>

mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);
	vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	return glm::lookAt(position, position + forward, vec3(0, 1, 0));
}

mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.f);
}

void Camera::Update(float deltaTime, GLFWwindow* window)
{

	float thetaR = glm::radians(theta);
	float phiR = glm::radians(phi);

	//calculate the forwards and right axes and up axis for the camera 
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float moveSpeed = 1;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		moveSpeed *= 3;
	}

	// use WASD, ZX keys to move camera around 
	if (glfwGetKey(window, GLFW_KEY_W)) {
		position += forward * deltaTime * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		position += -forward * deltaTime * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		position += right * deltaTime * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		position += -right * deltaTime * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_X)) {
		position += up * deltaTime * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Z)) {
		position += -up * deltaTime * moveSpeed;
	}


	glm::vec2 mouseDelta = Application::GetInstance()->GetMouseDelta();

	const float turnSpeed = 0.1f;

	// if the right button is down, increment theta and phi 
	if (glfwGetMouseButton(window, 1)) {
		theta += turnSpeed * mouseDelta.x;
		phi -= turnSpeed * mouseDelta.y;
	}
}

