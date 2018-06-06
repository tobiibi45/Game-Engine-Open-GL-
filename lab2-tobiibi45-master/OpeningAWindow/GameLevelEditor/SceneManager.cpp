#include "SceneManager.h"
#include "LevelEditorScene.h"


SceneManager::SceneManager()
{

}


SceneManager::~SceneManager()
{
	for (auto scene : loaded_scene)
	{
		delete scene.second;
	}
}

bool SceneManager::load_scene(const std::string name)
{
	it = loaded_scene.find(name);
	if (it != loaded_scene.end())
	{
		std::cout << "Scene already Loaded " << name << '\n';
		return false;
	}
	// Load model because not found
	loaded_scene[name] = new LevelEditorScene();
	std::cout << "Loaded Scene " << name << '\n';

	return true;
}

bool SceneManager::find_scene(const std::string name)
{
	it = loaded_scene.find(name);
	if (it != loaded_scene.end())
	{
		std::cout << "Scene Found and Loaded: " << name << '\n';
		return true;
	}
	return false;
}

LevelEditorScene* SceneManager::gets_scene(const std::string name)
{
	try
	{
		if (loaded_scene.find(name) != loaded_scene.end())
		{
			std::cout << "Found Scene  " << name << '\n';

		}
	}
	catch (const char e)
	{
		std::cout << " exception occured, Scene with name  " << name << " can't be found " << endl << e;
	}

	return loaded_scene.at(name);
}

