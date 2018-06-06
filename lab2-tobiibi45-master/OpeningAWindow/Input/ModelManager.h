#pragma once
#include "Model.h"
#include <map>

class ModelManager
{
public:

	/**
	 * \brief
	 */

	ModelManager();
	~ModelManager();

	bool load_model(const std::string file_path, MemoryManager* memMan);
	Model* get_model(const std::string model_name);

	const std::string load_path = "assets/models/";
	std::map<string, Model*> loaded_models;
	std::map<string, Model*>::iterator it;
};

