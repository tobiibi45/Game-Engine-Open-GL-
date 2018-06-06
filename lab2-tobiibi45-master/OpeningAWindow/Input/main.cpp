#define USE_GLFW 1
#define USE_PLAYSTATION 0
#ifdef USE_GLFW
	#include "GLFW_EngineCore.h"
#else USE_PLAYSTATION
	#include "PS4_EngineCore.h"
#endif

#include "Game.h"

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#else USE_PLAYSTATION
	engineCore = new PS4_EngineCore;
#endif

	if (!engineCore->initWindow(800, 600, "InputExample"))
		return -1;

	Game* game;

	engineCore->runEngine(&game);

	return 0;
}
