#pragma once
#include <InputHandler.h>
#include "MyGameObjects.h"

class RotateCameraLeftCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override
	{
		gameObject.OnMessage("rotateCameraLeft");
	}
};

class RotateCameraRightCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override
	{
		gameObject.OnMessage("rotateCameraLeft");
	}
};

class RotateCameraUpCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override
	{
		gameObject.OnMessage("rotateCameraLeft");
	}
};

class RotateCameraDownCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override
	{
		gameObject.OnMessage("rotateCameraLeft");
	}
};