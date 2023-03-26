#pragma once
#include <glm/glm.hpp>

class Light {
public:
	Light();
	Light(glm::vec3 _position, glm::vec3 _colour, float _intensity);
	glm::vec3 direction;
	glm::vec3 colour;
	float intensity;
};
