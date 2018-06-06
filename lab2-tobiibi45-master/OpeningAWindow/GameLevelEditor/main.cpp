#define USE_GLFW 1
#define USE_PLAYSTATION 0
#ifdef USE_GLFW
#include "GLFW_IMGUI_EngineCore.h"
#include "GLFW_EngineCore.h"
#else USE_PLAYSTATION
#include "PS4_EngineCore.h"
#endif

#include "LevelEditor.h"
#include <FirstGame.h>

bool load_editor = true;

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	if (load_editor)
	{
		engineCore = new GLFW_IMGUI_EngineCore;
	}
	else
	{
		engineCore = new GLFW_EngineCore;
	}

#else USE_PLAYSTATION
	engineCore = new PS4_EngineCore;
#endif
	if (load_editor)
	{
		if (!engineCore->initWindow(1920, 1080, "LevelEditorSample"))
			return -1;

		LevelEditor le;
		engineCore->runEngine(le);
	}
	else
	{
		if (!engineCore->initWindow(1920, 1080, "Maze Jogger"))
			return -1;

		FirstGame fg;
		engineCore->runEngine(fg);
	}
	return 0;

}
