#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"
#include <Box2D/Box2D.h>
#include <irrKlang/include/irrKlang.h>

class Game
{
public:

	virtual ~Game() = default;
	IEngineCore* m_engineInterfacePtr;
	InputHandler* m_inputHandler;
	b2World* the_world;

	//Creates a audio engine.
	irrklang::ISoundEngine* audioEngine = irrklang::createIrrKlangDevice();

	//Plays spooky background music in the game.
	irrklang::ISoundSource*  BackgroundSong = audioEngine->addSoundSourceFromFile("assets/Audio/GameSoundMix.ogg");

	//Plays a sparkle sound when the player collects a key.
	irrklang::ISoundSource*  Sparkle = audioEngine->addSoundSourceFromFile("assets/Audio/Sparkle.ogg");

	virtual void update(float dt) = 0;
	virtual void render() = 0;
};
