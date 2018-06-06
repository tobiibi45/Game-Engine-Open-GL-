#include "ModelManager.h"
#include "iostream"
#include "Model.h"


ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{
	for (auto models : loaded_models)
	{
		delete models.second;
	}
}

bool ModelManager::load_model(const std::string file_path, MemoryManager* memMan)
{
	it = loaded_models.find(file_path);
	if (it != loaded_models.end())
	{
		std::cout << "Model already Loaded " << file_path << '\n';
		return false;
	}
	// Load model because not found
	loaded_models[file_path] = new Model(load_path + file_path, memMan);
	std::cout << "Loaded Model " << file_path << ":" << load_path + file_path << '\n';

	return true;
}

Model* ModelManager::get_model(const std::string model_name)
{
	try
	{
		if (loaded_models.find(model_name) != loaded_models.end())
		{
			std::cout << "Found Model  " << model_name << '\n';
		
		}
	}
	catch (const char e)
	{
		std::cout << " exception occured, Model with name  " << model_name <<" is not loaded " << endl << e;
	}

	return loaded_models.at(model_name);
}