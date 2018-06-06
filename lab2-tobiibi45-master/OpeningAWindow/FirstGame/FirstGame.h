#pragma once
#include "Game.h"
#include "GameScene.h"
#include <ModelManager.h>
#include "PlayerCharacter.h"

class FirstGame : public Game
{

public: 
	FirstGame();

	void update(float dt) override;
	void render() override;
	bool add_models(string name, string type);
	bool delete_models(int id);
	bool loadLevelJSON(std::string levelJSONFile);

	GameScene* m_currentscene;
	ModelManager mm;
	TransformComponent* trans;
	MemoryManager * m_memManager;
};