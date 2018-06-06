#pragma once
#include "MyGameObjects.h"
#include <Box2D/Box2D.h>

class PlayerCharacter : public GameObject
{
	enum CameraViewState { firstPersonCamera, thirdPersonCamera };

public:
	PlayerCharacter();
	~PlayerCharacter();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;

	CameraViewState m_cameraState;
	glm::vec3 EulerRotationAngles;
};

