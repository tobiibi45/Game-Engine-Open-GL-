#pragma once
#include "GLFW_EngineCore.h"
#include "LevelEditor.h"
#include "LevelCameraEngine.h"

class LevelEditorIMGUI
{
public:
	LevelEditorIMGUI();
	~LevelEditorIMGUI();
	void init(GLFWwindow* m_window);
	void update(GLFWwindow* m_window, LevelCamera* l_camera, LevelEditor* level_editor);
	void render(LevelCamera* l_camera, LevelEditor* level_editor);
	void camera(GLfloat xOffset, GLfloat yOffset);
	bool changeInput;
	bool go_position;

	string name_;

	glm::vec3 &quat_to_vec3(const glm::quat q);
	TransformComponent* transform;

	static int	listboxGameobjectCurrentIndex;

private:
	bool showdemo;
	bool change_input_type;
};
