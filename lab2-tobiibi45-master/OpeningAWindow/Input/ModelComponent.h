#pragma once
#include "Components.h"
#include "Model.h"



class ModelComponent : public Components
{

public:
	Model* themodel;
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}

	ModelComponent() : themodel(nullptr) {}
	ModelComponent(Model* model) : themodel(model) {}

	void setModel(Model* model)
	{
		themodel = model;
	}
};

