#pragma once
#include <map>
#include "GameObject.h"
#include <InputHandler.h>

#include <iostream>

class MoveObjectLeft : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerLeft");
	}
};
class MoveObjectRight : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerRight");
	}

};

class MoveObjectForward : public InputCommand
{
public:
	void execute(GameObject&gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerForward");
	}
};

class MoveObjectBack : public InputCommand
{
public:
	void execute(GameObject& gameobject) override
	{
		if (gameobject.getComponent<TransformComponent>())
			gameobject.OnMessage("MovePlayerBack");
	}
};