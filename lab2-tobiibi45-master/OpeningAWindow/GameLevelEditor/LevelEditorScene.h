#pragma once
#include "Scene.h"
#include <IEngineCore.h>
#include <GameObject.h>
#include <LevelCameraEngine.h>

class LevelEditorScene : public Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void update(float dt) override;
	void render(IEngineCore* m_engineInterfacePtr) override;

	void cleanup() override;

	LevelCamera* camera;
	std::vector<GameObject*> v_gameObjects;
	void delete_object(int id);


};

