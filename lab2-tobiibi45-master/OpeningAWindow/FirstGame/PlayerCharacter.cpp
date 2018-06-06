#include "PlayerCharacter.h"
#include <Box2D/Box2D.h>
#include "InputHandler.h"

static const float SCALE = 30.f;

PlayerCharacter::PlayerCharacter()
{
	addComponent(new TransformComponent);
	addComponent(new CameraComponent);
	addComponent(new ModelComponent);

	m_cameraState = thirdPersonCamera;
}

void PlayerCharacter::OnUpdate(float dt)
{
	switch (m_cameraState)
	{
		case thirdPersonCamera:
		{
			// get the transform component details
			TransformComponent* transform = getComponent<TransformComponent>();
			// third person view - stay at a set distance from the player
			glm::quat inverseOrientation = inverse(transform->m_orientation);

			getComponent<CameraComponent>()->m_position = -1.0f*(transform->m_position + glm::vec3(0, 300, 1200) * inverseOrientation);
			getComponent<CameraComponent>()->m_orientation = inverseOrientation;
			if (getComponent<LevelCamera>() != nullptr)
			{
				getComponent<LevelCamera>()->SetPosition(-1.0f*(transform->m_position + glm::vec3(0, 300, 1200) * inverseOrientation));
			}
			break;
		}

		case firstPersonCamera:
		{
			// get the transform component details
			TransformComponent* transform = getComponent<TransformComponent>();
			// first person view - just have the camera in place on the player
			// need inverse pos and orientation
			getComponent<CameraComponent>()->m_position = -1.0f * transform->m_position;
			if (getComponent<LevelCamera>() != nullptr)
			{
				getComponent<LevelCamera>()->SetPosition(-1.0f * transform->m_position);
			}

			break;
		}
	}

}
void PlayerCharacter::OnMessage(const std::string msg)
{
	if (msg.compare(0, 14, "SetCamera") == 0)
	{
		if (msg == "SetCameraFirstPerson")
		{
			m_cameraState = firstPersonCamera;
		}
		else if (msg == "SetCameraThirdPerson")
		{
			m_cameraState = thirdPersonCamera;
		}
	}

	else if (msg.compare(0, 12, "rotateCamera") == 0)
	{
		TransformComponent* transform = getComponent<TransformComponent>();
		float rotationValue;
		if (msg == "rotateCameraLeft") rotationValue = 0.05;
		else rotationValue = -0.05;
		// same rotation for both first and third person cameras
		getComponent<TransformComponent>()->yaw(rotationValue);

	}
	else if (msg.compare(0, 10, "MovePlayer") == 0)
	{
		// determine the correct translation vector
		glm::vec3 translationVector(0, 0, 0);
		if (msg == "MovePlayerForward")translationVector.z = 40;
		else if (msg == "MovePlayerBack")translationVector.z = -40;
		else if (msg == "MovePlayerLeft")translationVector.x = 40;
		else if (msg == "MovePlayerRight")translationVector.x = -40;
		else if (msg == "MovePlayerUP")translationVector.y = 10; // For Level editing purposes
		else if (msg == "MovePlayerDown")translationVector.y = -10; // For Level editing purposes
		TransformComponent* transform = getComponent<TransformComponent>();
		transform->translate(translationVector
			* inverse(transform->m_orientation));
	}

}

PlayerCharacter::~PlayerCharacter()
{
	
}
