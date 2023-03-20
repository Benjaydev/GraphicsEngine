#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
#include "Texture.h"
#include <list>
#include <vector>
namespace aie { class ShaderProgram; }

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
		glm::vec4 tangent;
	};

	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	void InitialiseQuad();

	void InitialiseBox();
	void InitialiseCylinder();
	void InitialisePyramid();
	void InitialiseSphere();
	void InitialiseCone();
	void InitialiseGrid();
	void InitialiseFromFile(const char* filename);

	virtual void Draw();

	void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);
	void ApplyMaterial(aie::ShaderProgram* shader);
	void LoadMaterial(const char* filename);
	glm::vec3 Ka; // ambient colour of the surface 
	glm::vec3 Kd; // diffuse colour of the surface 
	glm::vec3 Ks; // specular colour of the surface 
	float specularPower; // tightness of specular highlights 

	aie::Texture mapKd; // diffuse texture map
	aie::Texture mapKs; // specular texture map 
	aie::Texture mapBump; // normal map 

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};

