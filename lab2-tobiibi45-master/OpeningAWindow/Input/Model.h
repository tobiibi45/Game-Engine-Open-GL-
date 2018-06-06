#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "memoryManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	Model() {}
	Model(string filepath, MemoryManager* memManager);

	void render(const unsigned int shaderProgram, const glm::mat4& model_Matrix);

	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);
	Vertex*& get_Vertex();

private:

	std::vector<Mesh> v_meshes;
	string directory;
	vector<Texture> v_textures;
	Vertex * vertices;

	void draw_box(Mesh* mesh, GLuint m_defaultShaderProgram, const glm::mat4& model_Matrix);

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	MemoryManager * m_memoryManager;
};
