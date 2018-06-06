#pragma once
#include <Scene.h>
#include "LevelEditorScene.h"
#include <map>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool load_scene(const std::string name);
	bool find_scene(const std::string name);
	LevelEditorScene* gets_scene(const std::string name);

private:
	std::map<std::string, LevelEditorScene*> loaded_scene;
	std::map<std::string, LevelEditorScene*>::iterator it;
};

