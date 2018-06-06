#pragma once
#include "Scene.h"
#include <IEngineCore.h>
#include <CameraComponent.h>
#include <GameObject.h>

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void update(float dt) override;
	void render(IEngineCore* m_engineInterfacePtr) override;

	void cleanup() override;

	CameraComponent* m_camera;

	std::vector<GameObject*> v_gameObjects;

	void GameScene::delete_object(int id);

	bool checkCollision(GameObject* first, GameObject* second, int id);
	void doCollisionsWalls();
	void doCollisionsFloor();
	void doCollisions(string name_, int id);

	bool found_player;

};

