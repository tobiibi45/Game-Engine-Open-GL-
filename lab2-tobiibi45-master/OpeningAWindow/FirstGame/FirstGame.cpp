
#include "FirstGame.h"
#include <fstream>
#include <sstream>
#include <json.h>
#include "PlayerCharacter.h"
#include <GameObject.h>
#include "MyGameObjects.h"
#include <InputHandler.h>

FirstGame::FirstGame()
{
	audioEngine->play2D(BackgroundSong, true);

	m_currentscene = nullptr;
	m_engineInterfacePtr = nullptr;
	trans = nullptr;

	m_memManager = new MemoryManager(100000000, 64);

	loadLevelJSON("assets/levels/level2.json");
}

void FirstGame::update(float dt)
{
	
	// perform all the logic for your game world
	if (m_currentscene)
		m_currentscene->update(dt);

}

void FirstGame::render()
{
	// perform all the logic for your game world
	if (m_currentscene)
		m_currentscene->render(m_engineInterfacePtr);
}

bool FirstGame::loadLevelJSON(std::string levelJSONFile)
{
	

	if (m_currentscene)
	{
		m_currentscene->cleanup();
		delete m_currentscene;
	}

	m_currentscene = new GameScene();
	m_inputHandler = new InputHandler();
	m_currentscene->m_camera = new CameraComponent(glm::vec3(60.0f, 800.0f, 6200.0f));
	

	std::ifstream jsonData;
	Json::Value root;

	jsonData.open(levelJSONFile.c_str());

	Json::Reader reader;
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: " << levelJSONFile << reader.getFormattedErrorMessages();
		return false;
	}
	const Json::Value gameObjects = root["GameObjects"];

	for (int i = 0; i < gameObjects.size(); i++)
	{

		string name = gameObjects[i]["name"].asString();

		float w, x, y, z;
		const Json::Value orientNode = gameObjects[i]["orientation"];
		// check if the node exists
		if (orientNode.type() != Json::nullValue)
		{
			w = orientNode[0].asFloat();
			x = orientNode[1].asFloat();
			y = orientNode[2].asFloat();
			z = orientNode[3].asFloat();
		}
		else
		{
			w = 1; x = 0; y = 0; z = 0;
		}

		glm::quat orient(w, x, y, z);

		// has to have a position node
		const Json::Value posNode = gameObjects[i]["position"];
		x = posNode[0].asFloat();
		y = posNode[1].asFloat();
		z = posNode[2].asFloat();

		glm::vec3 pos(x, y, z);

		//scale
		const Json::Value scaleNode = gameObjects[i]["scale"];
		// check if the node exists
		if (scaleNode.type() != Json::nullValue)
		{
			x = scaleNode[0].asFloat();
			y = scaleNode[1].asFloat();
			z = scaleNode[2].asFloat();
		}
		else
		{
			x = 1; y = 1; z = 1;
		}

		glm::vec3 scale(x, y, z);

		string type = gameObjects[i]["type"].asString();

		add_models(name, type);

		TransformComponent* trans = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>();

		trans->SetPosition(pos);
		trans->SetOrientation(orient);
		trans->SetScale(scale);

		std::cout << gameObjects[i]["name"].asString() << "-" << type << " loaded\n";

	}

	return true;
}

bool FirstGame::add_models(string name, string type)
{
	if (type == "PlayerCharacter")
	{
		if (name == "Boo_") //id 1 in vector
		{
			GameObject* warrior = new PlayerCharacter();
			warrior->setName(name);
			TransformComponent * trans = warrior->getComponent<TransformComponent>();
			CameraComponent* cam = warrior->getComponent<CameraComponent>();
			ModelComponent* m_warrior = warrior->getComponent<ModelComponent>();
			mm.load_model("/" + name + "/" + name + ".fbx", m_memManager);
			m_warrior->themodel = mm.get_model("/" + name + "/" + name + ".fbx");
			m_inputHandler->addObjects(warrior);
			m_currentscene->v_gameObjects.push_back(warrior);
		}
	}

	else if (type == "StaticEnvironmentObject")
	{
		if (name == "floor") //id 0 in vector
		{
			GameObject* floor = new StaticEnvironmentObject();
			floor->setName(name);
			TransformComponent* trans = floor->getComponent<TransformComponent>();
			ModelComponent* m_floor = floor->getComponent<ModelComponent>();
			mm.load_model("/" + name + "/" + name + ".dae", m_memManager);
			m_floor->themodel = mm.get_model("/" + name + "/" + name + ".dae");
			m_currentscene->v_gameObjects.push_back(floor);
		}
		else if (name == "bush") //id 4 in vector
		{
			GameObject* object = new StaticEnvironmentObject();
			object->setName(name);
			TransformComponent* trans = object->getComponent<TransformComponent>();
			ModelComponent* bush = object->getComponent<ModelComponent>();
			mm.load_model("/" + name + "/" + name + ".3ds", m_memManager);
			bush->themodel = mm.get_model("/" + name + "/" + name + ".3ds");
			m_currentscene->v_gameObjects.push_back(object);
		}
		else if (name == "cemetary_gate") //id 4 in vector
		{
			GameObject* object = new StaticEnvironmentObject();
			object->setName(name);
			TransformComponent* trans = object->getComponent<TransformComponent>();
			ModelComponent* bush = object->getComponent<ModelComponent>();
			mm.load_model("/" + name + "/" + name + ".fbx", m_memManager);
			bush->themodel = mm.get_model("/" + name + "/" + name + ".fbx");
			m_currentscene->v_gameObjects.push_back(object);
			m_inputHandler->addObjects(object);
		}
	}

	else if (type == "DynamicEnvironmentObject")
	{
		if (name == "key1") //id 4 in vector
		{
			GameObject* object2 = new DynamicEnvironmentObject();
			object2->setName(name);
			TransformComponent* trans = object2->getComponent<TransformComponent>();
			ModelComponent* bush = object2->getComponent<ModelComponent>();
			mm.load_model("/key3/key3.3ds", m_memManager);
			bush->themodel = mm.get_model("/key3/key3.3ds");
			m_currentscene->v_gameObjects.push_back(object2);
		}
		else if (name == "key3") //id 4 in vector
		{
			GameObject* object1 = new DynamicEnvironmentObject();
			object1->setName(name);
			TransformComponent* trans = object1->getComponent<TransformComponent>();
			ModelComponent* bush1 = object1->getComponent<ModelComponent>();
			mm.load_model("/" + name + "/" + name + ".3ds", m_memManager);
			bush1->themodel = mm.get_model("/" + name + "/" + name + ".3ds");
			m_currentscene->v_gameObjects.push_back(object1);
		}
		else if (name == "key2") //id 4 in vector
		{
			GameObject* object = new DynamicEnvironmentObject();
			object->setName(name);
			TransformComponent* trans = object->getComponent<TransformComponent>();
			ModelComponent* bush = object->getComponent<ModelComponent>();
			mm.load_model("/key3/key3.3ds", m_memManager);
			bush->themodel = mm.get_model("/key3/key3.3ds");
			m_currentscene->v_gameObjects.push_back(object);
		}

	}
	else
	{
		std::cout << type << ": unknown type\n";
		return false; // not an object we can create
	}
	return true;
}


