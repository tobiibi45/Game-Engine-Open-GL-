#pragma once
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>

class LevelCamera;
class Model;
class CameraComponent;
class Game;

class IEngineCore
{
public:

	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game& game) = 0;

	// some simple drawing tools - will want to move these to a proper renderer class later
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) = 0;
	virtual	void setCamera(const CameraComponent* cam) = 0;
	virtual void setCameraLevel(LevelCamera* camera) = 0;
	virtual void drawCube(const glm::mat4& modelMatrix) = 0;
	virtual void drawModel(Model* model, const glm::mat4& modelMatrix) = 0;
};

