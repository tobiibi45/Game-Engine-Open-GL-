#include "GameScene.h"


GameScene::GameScene()
{
	found_player = false;
}

void GameScene::update(float dt)
{
	// update the camera (probably don't need to do this each frame)
	for (auto & v_gameObject : v_gameObjects)
	{
		v_gameObject->OnUpdate(dt);
	}

	doCollisionsFloor();
	doCollisionsWalls();
	doCollisions("key1", 3);
	doCollisions("key2", 3);
	doCollisions("key3", 3);
	doCollisions("cemetary_gate", 4);
}


void GameScene::render(IEngineCore * m_engineInterfacePtr)
{
	// e.g. pass object details to the engine to render the next frame
	// e.g. pass object details to the engine to render the next frame
	// also clears the screen ready for the next frame
	m_engineInterfacePtr->renderColouredBackground(0.196078, 0.6, 0.8);

	
	m_engineInterfacePtr->setCamera(m_camera);
	

	// draw the models
	for (auto gameobjects : v_gameObjects)
	{

		m_engineInterfacePtr->drawModel(gameobjects->getComponent<ModelComponent>()->themodel 
			,gameobjects->getComponent<TransformComponent>()->getModelMatrix());
	}

}

void GameScene::cleanup()
{

}

GameScene::~GameScene()
{

}

void GameScene::delete_object(int id)
{
	v_gameObjects.erase(v_gameObjects.begin() + id);
}

bool GameScene::checkCollision(GameObject* first, GameObject* second, int id)
{
	if (id == 1) // collision with floor
	{
		if (first != nullptr && second != nullptr)
		{
			first->getComponent<TransformComponent>()->m_position.y -= 9.8f; // gravity
			if (
				first->getComponent<TransformComponent>()->m_position.y + 50 < second->getComponent<TransformComponent>()->m_position.y + second->getComponent<TransformComponent>()->m_scale.y &&
				first->getComponent<TransformComponent>()->m_position.y + 50 > second->getComponent<TransformComponent>()->m_position.y - first->getComponent<TransformComponent>()->m_scale.y
				)
			{
				//std::cout << "COLLISION NOT DETECTED " << second->getName() << "  and  " << first->getName() << std::endl;
				return true;
			}
			//	std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << std::endl;
			first->getComponent<TransformComponent>()->m_position.y = 0.f;
			return false;
		}
	}
	if (id == 2) // collision with wall
	{

		if (first != nullptr && second != nullptr)
		{
			auto dx = (first->getComponent<TransformComponent>()->m_position.x - second->getComponent<TransformComponent>()->m_position.x);
			auto dz = (first->getComponent<TransformComponent>()->m_position.z - second->getComponent<TransformComponent>()->m_position.z);

			auto dist = ((dx * dx) + (dz * dz));

			if (dist <= first->getComponent<TransformComponent>()->m_scale.x + first->getComponent<TransformComponent>()->m_position.x + first->getComponent<TransformComponent>()->m_scale.z + first->getComponent<TransformComponent>()->m_position.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				first->getComponent<TransformComponent>()->m_position.x = 59.f;
				first->getComponent<TransformComponent>()->m_position.z = -5200.f;
				return true;
			}
			//std::cout << "Bushes Xposs:  " << second->getComponent<TransformComponent>()->m_position.x << endl;
			//std::cout << "COLLISION NOT DETECTED " << s ond->getName() << "  and  " << first->getName() << std::endl;
			return false;
		}
	}

	if (id == 3) // collision with keys
	{

		if (first != nullptr && second != nullptr)
		{
			auto dx = (first->getComponent<TransformComponent>()->m_position.x - second->getComponent<TransformComponent>()->m_position.x);
			auto dz = (first->getComponent<TransformComponent>()->m_position.z - second->getComponent<TransformComponent>()->m_position.z);

			auto dist = ((dx * dx) + (dz * dz));

			if (dist <= first->getComponent<TransformComponent>()->m_scale.x + first->getComponent<TransformComponent>()->m_position.x + first->getComponent<TransformComponent>()->m_scale.z + first->getComponent<TransformComponent>()->m_position.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				//delete key
				for (auto i = 0; i < v_gameObjects.size(); i++)
				{
					string _name_ = v_gameObjects[i]->getName();
					if (_name_ == second->getName())
					{
						audioEngine->play2D(Sparkle);
						delete_object(i);
						first->add_keys(1);
						std::cout << "Key Found: " << second->getName() << endl;
					}
				}
				return true;
			}
			return false;
		}
	}

	if (id == 4) // collision with keys
	{

		if (first != nullptr && second != nullptr)
		{

			if (first->getComponent<TransformComponent>()->m_position.z < second->getComponent<TransformComponent>()->m_position.z + second->getComponent<TransformComponent>()->m_scale.z &&
				first->getComponent<TransformComponent>()->m_position.z > second->getComponent<TransformComponent>()->m_position.z - first->getComponent<TransformComponent>()->m_scale.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				//first->getComponent<TransformComponent>()->m_position.x = 4780.f;
				//first->getComponent<TransformComponent>()->m_position.z = -7640.f;
				return true;
			}

			if (first->get_key() >= 3)
			{
				for (auto i = 0; i < v_gameObjects.size(); i++)
				{
					string _name_ = v_gameObjects[i]->getName();
					if (_name_ == "cemetary_gate")
					{
						delete_object(i);
					}
				}
			}
			return false;
		}
	}
	return false;
}

void GameScene::doCollisionsWalls()
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto & v_gameObject : v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		std::string name2 = v_gameObject->getName();
		if (name == "Boo_" && !found_player)
		{
			first = v_gameObject;
			found_player = true;
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
	}
	for (auto & v_gameObject : v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		if (name == "bush")
		{
			second = v_gameObject;
			checkCollision(first, second, 2);
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
	}
}

void GameScene::doCollisionsFloor()
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto & v_gameObject : v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		if (name == "Boo_")
		{
			first = v_gameObject;
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
		else if (name == "floor")
		{
			second = v_gameObject;
			//std::cout << "NAME 2:  " << second->getName() << std::endl;
		}
		checkCollision(first, second, 1);
	}
}

void GameScene::doCollisions(string name_, int id)
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto i = 0; i < v_gameObjects.size(); i++)
	{
		std::string name = v_gameObjects[i]->getName();
		if (name == "Boo_")
		{
			first = v_gameObjects[i];
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
		else if (name == name_)
		{
			second = v_gameObjects[i];
		}
		checkCollision(first, second, id);
	}

}
