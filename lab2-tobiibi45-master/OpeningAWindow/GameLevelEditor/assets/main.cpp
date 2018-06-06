#define USE_GLFW 1
#define USE_PLAYSTATION 0
#ifdef USE_GLFW
#include "GLFW_IMGUI_EngineCore.h"
#include "GLFW_EngineCore.h"
#else USE_PLAYSTATION
#include "PS4_EngineCore.h"
#endif

#include "LevelEditor.h"

bool load_editor = true;

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_IMGUI_EngineCore;

#else USE_PLAYSTATION
	engineCore = new PS4_EngineCore;
#endif
	if (!engineCore->initWindow(1920, 1080, "LevelEditorSample"))
			return -1;

		LevelEditor le;
		engineCore->runEngine(le);
	return 0;
}
