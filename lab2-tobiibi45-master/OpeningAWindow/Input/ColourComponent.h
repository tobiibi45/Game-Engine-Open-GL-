#pragma once
#include "Components.h"
#include "Definitions.h"



class ColourComponent : public Components
{
	
public:
	ColourComponent() : m_colourValue(0) {};
	float m_colourValue;
	void OnUpdate(float dt) override {};
	

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += COLOR_ADJUSTMENT_VALUE;
		}
		else if (m == "decrease")
		{
			m_colourValue -= COLOR_ADJUSTMENT_VALUE;
		}
	}

};

class RedComponent : public ColourComponent {};
class GreenComponent : public ColourComponent {};
class BlueComponent : public ColourComponent {};

