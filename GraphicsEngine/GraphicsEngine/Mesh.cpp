#pragma once
#include "Mesh.h"
#include "glad.h"  
#include <assimp/scene.h> 
#include <assimp/cimport.h> 
#include <vector> 
#include <iostream>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	assert(vao == 0);

	// Generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the vertex array, mesh wrapper
	glBindVertexArray(vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Fill the vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	// Enable the first element as position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Bind indices if there are any 
	if (indexCount != 0) {
		glGenBuffers(1, &ibo);

		// Bind vertex buffer 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// Fill vertex buffer 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else {
		triCount = vertexCount / 3;
	}

	// unbind buffers 
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::InitialiseQuad()
{
	assert(vao == 0);

	// Generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the vertex array, mesh wrapper
	glBindVertexArray(vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	// Define 6 vertices for 2 triangles 
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// Fill the vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable the first element as position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Unbind the buffers 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Quad has 2 triangles 
	triCount = 2;
}

void Mesh::InitialiseBox()
{
	Mesh::Vertex vertices[8];
	vertices[0].position = { 0.5f, 0.5f, 0.5f, 1 };
	vertices[1].position = { -0.5f, 0.5f, 0.5f, 1 };
	vertices[2].position = { -0.5f, -0.5f, 0.5f, 1 };
	vertices[3].position = { 0.5f, -0.5f, 0.5f, 1 };

	vertices[4].position = { 0.5f, -0.5f, -0.5f, 1 };
	vertices[5].position = { 0.5f, 0.5f, -0.5f, 1 };
	vertices[6].position = { -0.5f, 0.5f, -0.5f, 1 };
	vertices[7].position = { -0.5f, -0.5f, -0.5f, 1 };

	// Front, right, top, left, bottom, back
	unsigned int indices[36] = { 0, 1, 2, 2, 0, 3,  5, 0, 3, 3, 5, 4,  1, 0, 5, 5, 1, 6,  1, 6, 7, 7, 1, 2,  3, 2, 7, 7, 3, 4,  6, 5, 4, 4, 6, 7 };
	Initialise(8, vertices, 36, indices);
}

void Mesh::InitialiseCylinder()
{
}

void Mesh::InitialisePyramid()
{
}

void Mesh::InitialiseSphere()
{
}

void Mesh::InitialiseCone()
{
}

void Mesh::InitialiseGrid()
{
}

void Mesh::InitialiseFromFile(const char* filename)
{
	// Read vertices from the model 
	const aiScene* scene = aiImportFile(filename, 0);

	// Just use the first mesh found

	aiMesh* mesh = scene->mMeshes[0];

	// Extract indicies from the first mesh 
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;
	for (int i = 0; i < numFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);

		// generate a second triangle for quads 
		if (mesh->mFaces[i].mNumIndices == 4)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[3]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	// Extract vertex data 
	int numV = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numV];
	for (int i = 0; i < numV; i++)
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		// TODO, normals and UVs 
	}
	Initialise(numV, vertices, indices.size(), indices.data());

	delete[] vertices;

}

void Mesh::Draw()
{
	glBindVertexArray(vao);
	// Check whether using indices or just vertices
	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}
