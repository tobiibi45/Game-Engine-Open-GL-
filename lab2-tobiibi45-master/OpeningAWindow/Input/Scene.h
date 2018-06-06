#pragma once

#include "IEngineCore.h"
#include <irrKlang/include/irrKlang.h>

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void update(float dt) = 0;
	virtual void render(IEngineCore* m_engineInterfacePtr) = 0;

	virtual void cleanup() = 0;

	//Creates a audio engine.
	irrklang::ISoundEngine* audioEngine = irrklang::createIrrKlangDevice();

	//Plays spooky background music in the game.
	irrklang::ISoundSource*  BackgroundSong = audioEngine->addSoundSourceFromFile("assets/Audio/GameSoundMix.ogg");

	//Plays a sparkle sound when the player collects a key.
	irrklang::ISoundSource*  Sparkle = audioEngine->addSoundSourceFromFile("assets/Audio/Sparkle.ogg");
};
