#include "Model.h"
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <iostream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(string filepath, MemoryManager* memManager)
{
	m_memoryManager = memManager;
	loadModel(filepath);
	
}

void Model::loadModel(string filepath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = filepath.substr(0, filepath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}




void Model::processNode(aiNode* node, const aiScene* scene)
{
	// get the meshes of the node and add them to our vector
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int sceneMeshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[sceneMeshIndex];
		v_meshes.push_back(processMesh(mesh, scene));
	}
	// recursively process the nodes of any children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	/*vector<Vertex> vertices;
	vector<unsigned int> indices;*/
	vector<Texture> textures;
	//// create enough space for the vertices and indices
	//vertices.resize(mesh->mNumVertices);
	//indices.resize(mesh->mNumFaces * 3); // imported as triangles
	
	vertices = (Vertex *)m_memoryManager->allocateBlock("vertices", sizeof(Vertex) * mesh->mNumVertices);
	unsigned int* indices = (unsigned int *)m_memoryManager->allocateBlock("indices", sizeof(unsigned int) * mesh->mNumFaces * 3);


	// for each vertex of the mesh copy the data to out own mesh format
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// all meshes should have vertices and normals
		vertices[i].position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// check if the mesh has texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertices[i].textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices[i].textureCoords = glm::vec2(0.0f, 0.0f);
		}

	}

	// save all the vertex indices in the indices vector
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		// retrieve all indices of the face and store them in the indices vector
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices[3 * i + j] = mesh->mFaces[i].mIndices[j];
	}

	// get material textures (if there are any)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");

		// put all the textures together in a single vector
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	Texture * pTextures = (Texture *)m_memoryManager->allocateBlock("textures", sizeof(Texture) * textures.size());
	Texture * pWalker = pTextures;
	for (int i = 0; i < textures.size(); i++)
	{
		(*pWalker).filepath = textures[i].filepath;
		(*pWalker).id = textures[i].id;
		(*pWalker).type = textures[i].type;
		pWalker++;
	}

	// return the mesh data
	//return Mesh(vertices, indices, textures);

	return Mesh(vertices, mesh->mNumVertices, indices, mesh->mNumFaces * 3, pTextures, textures.size());
}

Vertex*& Model::get_Vertex()
{
	return vertices;
}

void Model::render(const unsigned int shaderProgram, const glm::mat4& model_Matrix)
{
	for (auto mesh : v_meshes)
	{
		mesh.render(shaderProgram);
		//draw_box(&mesh, shaderProgram, model_Matrix); //sttempt to create a bounding box arounf the player
	}
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool b_loadedTexture = false;
		for (auto texture : v_textures)
		{
			if (std::strcmp(texture.filepath.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(texture);
				b_loadedTexture = true;
				break;
			}
		}
		if (!b_loadedTexture)
		{
			// setup a new texture
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.filepath = str;
			textures.push_back(texture);
			v_textures.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}


// static function to load a texture using lightweight stb_image library
unsigned int Model::TextureFromFile(const char* filepath, const string& directory, bool gamma)
{
	string filename = string(filepath);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (textureData)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);
	}
	else
	{
		std::cout << "Texture failed to load from: " << filepath << std::endl;
		stbi_image_free(textureData);
	}

	return textureID;
}

void Model::draw_box(Mesh* mesh, GLuint m_defaultShaderProgram, const glm::mat4& model_Matrix)
{
	if (mesh->numVertices == 0)
		return;

	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
		0.5, -0.5, -0.5, 1.0,
		0.5,  0.5, -0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,
		0.5, -0.5,  0.5, 1.0,
		0.5,  0.5,  0.5, 1.0,
		-0.5,  0.5,  0.5, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (this->vertices != nullptr)
	{
		GLfloat
			min_x, max_x,
			min_y, max_y,
			min_z, max_z;
		min_x = max_x = this->vertices[0].position.x;
		min_y = max_y = this->vertices[0].position.y;
		min_z = max_z = this->vertices[0].position.z;
		for (int i = 0; i < mesh->numVertices; i++) {
			if (this->vertices[i].position.x < min_x) min_x = this->vertices[i].position.x;
			if (this->vertices[i].position.x > max_x) max_x = this->vertices[i].position.x;
			if (this->vertices[i].position.y < min_y) min_y = this->vertices[i].position.y;
			if (this->vertices[i].position.y > max_y) max_y = this->vertices[i].position.y;
			if (this->vertices[i].position.z < min_z) min_z = this->vertices[i].position.z;
			if (this->vertices[i].position.z > max_z) max_z = this->vertices[i].position.z;
		}

		glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
		glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
		glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

		/* Apply object's transformation matrix */
		glm::mat4 m = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 10000.0f);

		glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model_Matrix));

		auto attribute_v_coord = 1;
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
		glEnableVertexAttribArray(attribute_v_coord);
		glVertexAttribPointer(
			attribute_v_coord,  // attribute
			4,                  // number of elements per vertex, here (x,y,z,w)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(attribute_v_coord);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &vbo_vertices);
		glDeleteBuffers(1, &ibo_elements);
	}

}
