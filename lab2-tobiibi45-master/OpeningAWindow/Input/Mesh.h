#pragma once

#include <glad/glad.h> 

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <vector>
using namespace std;

class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

struct Texture {
	unsigned int id;
	string type;
	aiString filepath;
};

class Mesh
{
public:
	Vertex *vertices;
	unsigned int* indices;
	Texture* textures;
	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int numTextures;
	unsigned int VAO;

	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, Texture* textures, unsigned int numTextures);

	// render the mesh with a given shader program
	void render(const unsigned int shaderProgram);

private:
	// buffer objects
	unsigned int VBO, EBO;

	// initialises all the buffer arrays
	void setupMesh();

};
