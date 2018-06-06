#pragma once
#include <map>
#include "GameObject.h"
#include <vector>
#include "TransformComponent.h"
#include "CameraComponent.h"


class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& gameobject) = 0;
};


class RotateCameraLeft : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("rotateCameraLeft");
	}
};

class RotateCameraRight : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("rotateCameraRight");
	}
};
class MovePlayerLeft : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerLeft");

	}
};
class MovePlayerRight : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerRight");
	}

};

class MovePlayerForward : public InputCommand
{
public:
	void execute(GameObject&gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerForward");
	}
};

class MovePlayerBack : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerBack");
	}
};

class MoveStaticLeft : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticLeft");
	}
};
class MoveStaticRight : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticRight");
	}

};

class MovePlayerUp : public InputCommand // For Level editing purposes
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerUP");
	}

};
class MovePlayerDown : public InputCommand // For Level editing purposes
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerDown");
	}

};

class MoveStaticUp : public InputCommand // For Level editing purposes
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticUP");
	}

};
class MoveStaticDown : public InputCommand // For Level editing purposes
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticDown");
	}

};

class MoveStaticForward : public InputCommand
{
public:
	void execute(GameObject&gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticForward");
	}
};

class MoveStaticBack : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MoveStaticBack");
	}
};

class FirstPersonCamera : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<LevelCamera>())
			gameobject.OnMessage("SetCameraFirstPerson");
	}
};

class ThirdPersonCamera : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<LevelCamera>())
			gameobject.OnMessage("SetCameraThirdPerson");
	}
};

struct InputHandler
{

	std::vector<GameObject*> v_objectsRequiringInput;
	std::map<int, InputCommand*> m_controlMapping;

	InputHandler()
	{
		// the idea will be to map the keys directly from a config file that can be loaded in
		// and changed on the fly
		m_controlMapping[91] = new RotateCameraLeft; // [
		m_controlMapping[93] = new RotateCameraRight; // ]

		//For PLayer Character
		m_controlMapping[65] = new MovePlayerLeft; // A
		m_controlMapping[68] = new MovePlayerRight; // D
		m_controlMapping[87] = new MovePlayerForward; // W
		m_controlMapping[83] = new MovePlayerBack; // S
		m_controlMapping[81] = new MovePlayerUp; // Q
		m_controlMapping[69] = new MovePlayerDown; // E
		m_controlMapping[90] = new FirstPersonCamera; // z
		m_controlMapping[88] = new ThirdPersonCamera; // x
		//For Static Movement
		m_controlMapping[74] = new MoveStaticLeft; // j
		m_controlMapping[76] = new MoveStaticRight; // l
		m_controlMapping[73] = new MoveStaticForward; // i
		m_controlMapping[75] = new MoveStaticBack; // k
		m_controlMapping[85] = new MoveStaticUp; // u
		m_controlMapping[79] = new MoveStaticDown; // o
	}

	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for (const auto& mapEntry : m_controlMapping)
		{
			if (keyBuffer[mapEntry.first])
			{
				for (int i = 0; i < v_objectsRequiringInput.size(); i++)
				{
					mapEntry.second->execute(*v_objectsRequiringInput[i]);
				}
			}
		}

	}

	void addObjects(GameObject* objects)
	{
		v_objectsRequiringInput.push_back(objects);
	}

	void deleteObjects(int id)
	{
		v_objectsRequiringInput.erase(v_objectsRequiringInput.begin() + id);

	}

};
