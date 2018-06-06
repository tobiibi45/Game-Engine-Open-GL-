#pragma once

#pragma once
#include "Components.h"
#include "ColourComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ParticleEmiiterComponent.h"
#include "CameraComponent.h"
#include <unordered_map>
#include "LevelCameraEngine.h"
#include <typeindex>

class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if (iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}

		// return null if we don't have a component of that type
		return nullptr;

	}

	struct bodyUserData {
		int type;
	};

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}

	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string msg) = 0;

	const char* getName()
	{
		return name.c_str();
	}

	void setName(string n)
	{
		name = n;
	}

	void add_keys(int i)
	{
		player_keys = player_keys + i;
	}

	int get_key()
	{
		return player_keys;
	}

private:
	std::unordered_map<std::type_index, Components*> m_components;

	int player_keys = 0;
	std::string name;
};
