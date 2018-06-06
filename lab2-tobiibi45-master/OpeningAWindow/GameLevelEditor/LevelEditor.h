#pragma once
#include "Game.h"
#include "GameScene.h"
#include <ModelManager.h>
#include "LevelEditorScene.h"
#include  <LevelCameraEngine.h>
#include "SceneManager.h"
#include <map>
#define M_PI 3.14159265359

class LevelEditor : public Game
{
	enum class SceneChange { firstScene, SecondScene, ThirdScene };
public: 
	LevelEditor();
	~LevelEditor();

	void update(float dt) override;
	void render() override;

	bool loadLevelJSON(std::string levelJSONFile);
	bool saveLevelJSON(std::string file_name);
	bool add_models(string name, string type);
	bool delete_models(int id);
	bool add_input(GameObject* object);
	bool remove_input(int id);

	bool load_scene_2;
	bool load_scene_1;
	bool levEditLoaded;
	bool render_win;

	LevelEditorScene* m_currentscene;
	ModelManager mm;
	SceneManager sm;
	SceneChange scene_change;
	TransformComponent* trans;
	LevelCamera* LevelEditorCamera;
	MemoryManager * m_memManager;

	std::vector<GameObject*> v_go;

	std::vector<std::string> string_;
	std::vector<std::string> type_;
};
