#include "MyGameObjects.h"

StaticEnvironmentObject::StaticEnvironmentObject()
{
	addComponent(new TransformComponent);
	addComponent(new CameraComponent);
	addComponent(new ModelComponent);

}

void StaticEnvironmentObject::OnUpdate(float dt)
{
	// get the transform component details
	TransformComponent* transform = getComponent<TransformComponent>();
	// third person view - stay at a set distance from the player
	glm::quat inverseOrientation = inverse(transform->m_orientation);
	getComponent<CameraComponent>()->m_position =
		-1.0f*(transform->m_position + glm::vec3(0, 300, 1200) * inverseOrientation);
	getComponent<CameraComponent>()->m_orientation = inverseOrientation;
}

void StaticEnvironmentObject::OnMessage(const std::string msg)
{

}

StaticEnvironmentObject::~StaticEnvironmentObject()
{
}

DynamicEnvironmentObject::DynamicEnvironmentObject()
{
	addComponent(new TransformComponent);
	addComponent(new CameraComponent);
	addComponent(new ModelComponent);

	EulerRotationAngles = {0,0,0};
}

DynamicEnvironmentObject::~DynamicEnvironmentObject()
{

}

void DynamicEnvironmentObject::OnUpdate(float dt)
{
	// get the transform component details
	TransformComponent* transform = getComponent<TransformComponent>();

	transform->m_orientation = glm::quat(glm::radians(EulerRotationAngles));

	// just get the current values (in degrees)
	EulerRotationAngles = glm::degrees(glm::eulerAngles(transform->m_orientation));

	EulerRotationAngles.y += 1.f;

	if(EulerRotationAngles.y >= 89.f)
	{
		EulerRotationAngles.y = -89.f;
	}
}

void DynamicEnvironmentObject::OnMessage(const std::string msg)
{
	if (msg.compare(0, 18, "rotateCameraStatic") == 0)
	{
		TransformComponent* transform = getComponent<TransformComponent>();
		float rotationValue;
		if (msg == "rotateCameraStaticLeft") rotationValue = -0.05;
		else rotationValue = 0.05;
		// same rotation for both first and third person cameras
		getComponent<TransformComponent>()->yaw(rotationValue);

	}
	else if (msg.compare(0, 10, "MoveStatic") == 0)
	{
		// determine the correct translation vector
		glm::vec3 translationVector(0, 0, 0);
		if (msg == "MoveStaticForward")translationVector.z = -10;
		else if (msg == "MoveStaticBack")translationVector.z = 10;
		else if (msg == "MoveStaticLeft")translationVector.x = -10;
		else if (msg == "MoveStaticRight")translationVector.x = 10;
		else if (msg == "MoveStaticUP")translationVector.y = 10; // For Level editing purposes
		else if (msg == "MoveStaticDown")translationVector.y = -10; // For Level editing purposes
		TransformComponent* transform = getComponent<TransformComponent>();
		transform->translate(translationVector
			* inverse(transform->m_orientation));
	}
}
