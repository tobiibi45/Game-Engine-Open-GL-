#pragma once
#include "GameObject.h"

class StaticEnvironmentObject : public GameObject
{
public:
	StaticEnvironmentObject();
	~StaticEnvironmentObject();
	
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	
};

class DynamicEnvironmentObject : public GameObject
{
public:
	DynamicEnvironmentObject();
	~DynamicEnvironmentObject();

	glm::vec3 EulerRotationAngles;
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};


