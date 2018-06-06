#pragma once
#include <string>

class Components
{
public:
	virtual ~Components() = default;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string m) = 0;
};