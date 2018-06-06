#include "LevelEditorScene.h"

LevelEditorScene::LevelEditorScene()
{
	
}

LevelEditorScene::~LevelEditorScene()
{

}

void LevelEditorScene::update(float dt)
{
	// update the camera (probably don't need to do this each frame)
	for (int i = 0; i < v_gameObjects.size(); i++)
	{
		v_gameObjects[i]->OnUpdate(dt);
	}
}

void LevelEditorScene::render(IEngineCore* m_engineInterfacePtr)
{
	m_engineInterfacePtr->renderColouredBackground(0.196078, 0.6, 0.8);


	//m_engineInterfacePtr->setCamera(m_camera);
	m_engineInterfacePtr->setCameraLevel(camera);

	// draw the models
	for (auto gameobjects : v_gameObjects)
	{

		m_engineInterfacePtr->drawModel(gameobjects->getComponent<ModelComponent>()->themodel
			, gameobjects->getComponent<TransformComponent>()->getModelMatrix());
	}
}

void LevelEditorScene::cleanup()
{

}

void LevelEditorScene::delete_object(int id)
{
	v_gameObjects.erase(v_gameObjects.begin() + id);
}
