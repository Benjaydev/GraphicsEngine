#include "Light.h"

Light::Light()
{
	direction = glm::vec3(0);
	colour = glm::vec3(1);
	intensity = 1;
}

Light::Light(glm::vec3 _position, glm::vec3 _colour, float _intensity)
{
	direction = _position;
	colour = _colour;
	intensity = _intensity;
}
