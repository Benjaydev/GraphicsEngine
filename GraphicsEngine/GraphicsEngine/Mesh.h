#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
#include "Texture.h"
#include <list>
#include <vector>
#include <unordered_map>

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
	void CalculateTangents(Vertex* vertices, unsigned int vertexCount, unsigned int indices[], unsigned int indiciesCount);
	void ApplyMaterial(aie::ShaderProgram* shader);
	void LoadMaterial(const char* filename);
	void LoadDiffuseTexture(const char* filename);
	void LoadSpecularTexture(const char* filename);
	void LoadBumpTexture(const char* filename);
	glm::vec3 Ka = glm::vec3(1); // ambient colour of the surface 
	glm::vec3 Kd = glm::vec3(1); // diffuse colour of the surface 
	glm::vec3 Ks = glm::vec3(1); // specular colour of the surface 
	float specularPower = 1; // tightness of specular highlights 

	aie::Texture* mapKd = new aie::Texture(); // diffuse texture map
	aie::Texture* mapKs = new aie::Texture(); // specular texture map 
	aie::Texture* mapBump = new aie::Texture(); // normal map 




protected:
	static std::list<aie::Texture*> cachedDiffuseTextures;
	static std::list<aie::Texture*> cachedSpecularTextures;
	static std::list<aie::Texture*> cachedBumpTextures;
	aie::Texture* LoadTexture(const char* filename, std::list<aie::Texture*>* cache);

	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};

