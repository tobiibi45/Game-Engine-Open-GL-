
#include "LevelEditor.h"
#include <fstream>
#include <sstream>
#include <json.h>
#include <GameObject.h>
#include <InputHandler.h>
#include <Model.h>
#include <ModelManager.h>
#include "PlayerCharacter.h"

LevelEditor::LevelEditor() : scene_change(SceneChange::firstScene)
{
	audioEngine->play2D(BackgroundSong, true); // plays background music

	levEditLoaded = false;
	render_win = false;

	load_scene_1 = true;
	load_scene_2 = false;

	m_currentscene = nullptr;
	m_engineInterfacePtr = nullptr;
	trans = nullptr;

	//the_world = new b2World(b2Vec2(1, 1));

	LevelEditorCamera = new LevelCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_memManager = new MemoryManager(100000000, 64);

	sm.load_scene("level1");
	sm.load_scene("level2");

	//const b2Vec2 gravity(0.f, 9.8f);
	//my_world = new b2World(gravity);

	if (sm.find_scene("level1") && load_scene_1)
	{
		m_currentscene = sm.gets_scene("level1");
		loadLevelJSON("assets/levels/level2.json");
	}
	

}

LevelEditor::~LevelEditor()
{

}

void LevelEditor::update(float dt)
{

	if (m_currentscene)
		m_currentscene->update(dt);

}


bool LevelEditor::add_models(string name, string type)
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

bool LevelEditor::delete_models(int id)
{

	if (id >= 0)
	{
		m_currentscene->delete_object(id);
		return true;
	}
	return false;
	
}

bool LevelEditor::add_input(GameObject* object)
{
	if (object != nullptr)
	{
		m_inputHandler->addObjects(object);
		return true;
	}
	return false;
}

bool LevelEditor::remove_input(int id)
{
	if (id >= 0)
	{
		m_inputHandler->deleteObjects(id);
		return true;
	}
	return false;
}

void LevelEditor::render()
{
	// perform all the logic for your game world
	if (m_currentscene)
		m_currentscene->render(m_engineInterfacePtr);

	m_engineInterfacePtr->renderText("Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(40, 40, 40));
	if (!levEditLoaded)
	{
		m_engineInterfacePtr->renderText("Press Y to load Level Editor", 0.05f, 0.01f, 1.0f, glm::vec3(90, 50, 70));
		m_engineInterfacePtr->renderText("Also, turn Music up for full emersion", 0.5f, 0.01f, 1.0f, glm::vec3(90, 50, 70));
	}
	if(render_win)
	{
		m_engineInterfacePtr->renderText("Congrats, You have escaped the maze", 0.01f, 0.4f, 1.5f, glm::vec3(90, 50, 70));
	}
}

bool LevelEditor::loadLevelJSON(std::string levelJSONFile)
{

	m_inputHandler = new InputHandler();
	m_currentscene->camera = this->LevelEditorCamera;
	//m_currentscene->m_camera = new CameraComponent();

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

bool LevelEditor::saveLevelJSON(std::string file_name)
{
	std::ofstream jsonData;
	Json::Value root;
	std::string type;

	jsonData.open(file_name.c_str());

	for (auto i = 0; i < m_currentscene->v_gameObjects.size(); i++)
	{
		std::string name = m_currentscene->v_gameObjects[i]->getName();

		if (name == "Boo_")
		{
			type = "PlayerCharacter";
		}
		else if (name == "key3" || name == "key2" || name == "key1")
		{
			type = "DynamicEnvironmentObject";
		}
		else 
		{
			type = "StaticEnvironmentObject";
		}

		root["GameObjects"][i]["name"] = name;
		root["GameObjects"][i]["type"] = type;

		Json::Value orientation(Json::arrayValue);

		auto w = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_orientation.w;
		auto x = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_orientation.x;
		auto y = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_orientation.y;
		auto z = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_orientation.z;

		// check if the node exists
		if (orientation.type() != Json::nullValue)
		{
			orientation.append(w);
			orientation.append(x);
			orientation.append(y);
			orientation.append(z);
		}
		else
		{
			orientation.append(0);
			orientation.append(0);
			orientation.append(0);
			orientation.append(0);
		}

		root["GameObjects"][i]["orientation"] = orientation;

		Json::Value position(Json::arrayValue);

		auto Px = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_position.x;
		auto Py = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_position.y;
		auto Pz = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_position.z;

		// has to have a position node
		if (position.type() != Json::nullValue)
		{
			position.append(Px);
			position.append(Py);
			position.append(Pz);
		}
		else
		{
			position.append(0);
			position.append(0);
			position.append(0);
		}

		root["GameObjects"][i]["position"] = position;

		Json::Value scale(Json::arrayValue);

		auto Sx = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_scale.x;
		auto Sy = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_scale.y;
		auto Sz = m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_scale.z;

		// has to have a position node
		if (scale.type() != Json::nullValue)
		{
			scale.append(Sx);
			scale.append(Sy);
			scale.append(Sz);
		}
		else
		{
			scale.append(1);
			scale.append(1);
			scale.append(1);
		}

		root["GameObjects"][i]["scale"] = scale;
	}

	Json::StyledWriter styledWriter;
	jsonData << styledWriter.write(root);

	jsonData.close();

	return true;
}
