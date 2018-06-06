#pragma once
#include "LevelEditorIMGUI.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw_gl3.h>

int LevelEditorIMGUI::listboxGameobjectCurrentIndex;
LevelEditorIMGUI::LevelEditorIMGUI()
{
	
}

void LevelEditorIMGUI::init(GLFWwindow* m_window)
{
	//SETUP IMGUI BINDING
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_window, true);

	showdemo = false;
	changeInput = true;
	change_input_type = false;
	go_position = true;

	listboxGameobjectCurrentIndex = -1;
}


void LevelEditorIMGUI::update(GLFWwindow* m_window, LevelCamera* l_camera, LevelEditor* level_editor)
{
	
	//SETUP IMGUI
	ImGui_ImplGlfwGL3_NewFrame();

	//static int	listboxGameobjectCurrentIndex = -1;
	static char nameBuffer[64] = "";
	const char* gameObjectMeshes[] = { "key1", "my_key", "key3", "floor", "bush", "Boo_", "cemetary_gate" };
	static int currentSelectedGameObjectType = 0;
	static int currentSelectedGameObjectName = 0;
	const char* gameObjectTypes[] = { "StaticEnvironmentObject", "DynamicEnvironmentObject", "PlayerCharacter" };

	if (showdemo)
	{
		ImGui::ShowDemoWindow();
	}

	else
	{
		int window_width, window_height;
		glfwGetFramebufferSize(m_window, &window_width, &window_height);

		ImGui::Begin("main");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(window_width * 0.25, window_height * 0.25));

		//Show demo
		static bool showdemowindow = false;
		if (ImGui::Button("Show Demo Window"))
		{
			showdemowindow = true;
		}

		if (showdemowindow)
		{
			showdemo = true;
		}

		//Change Inpput to model
		static bool change_the_input = false;
		if (ImGui::Button("Change imput to model"))
		{
			change_the_input = true;
		}

		if (change_the_input)
		{
			changeInput = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Change imput to camera"))
		{
			change_the_input = false;
		}

		if (!change_the_input)
		{
			changeInput = false;
		}

		static bool change_scene = false;
		if (ImGui::Button("Load Scene 1"))
		{
			level_editor->load_scene_1 = true;
			change_scene = false;
		}

		if (!change_scene)
		{

		}

		ImGui::SameLine();
		if (ImGui::Button("Load Scene 2"))
		{
			level_editor->load_scene_2 = true;
			change_scene = true;
		}

		if (change_scene)
		{

		}

		static bool removeInput = false;
		bool isremoving = false;
		if (ImGui::Button("Remove INput"))
		{
			removeInput = true;
			isremoving = true;
		}

		if (removeInput && isremoving)
		{
			//remove input
			for (auto i = 0; i < level_editor->m_currentscene->v_gameObjects.size(); i++)
			{
				string _name_ = level_editor->m_currentscene->v_gameObjects[i]->getName();
				if (_name_ == name_)
				{
					level_editor->remove_input(i);
				}
			}
			
			isremoving = false;
		}
		ImGui::SameLine();
		static bool addInput = false;
		bool isaddingin = false;
		if (ImGui::Button("Add INput"))
		{
			addInput = true;
			isaddingin = true;
		}

		if (addInput && isaddingin)
		{
			level_editor->add_input(level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]);
			isaddingin = false;
		}


		static bool go_to_positon = false;
		if (ImGui::Button("Set Camera to Model"))
		{
			go_to_positon = true;
		}

		if (go_to_positon)
		{
			go_position = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Camera from Model"))
		{
			go_to_positon = false;
		}

		if (!go_to_positon)
		{
			go_position = false;
		}

		ImGui::Combo("GameObject Types", &currentSelectedGameObjectType, gameObjectTypes, IM_ARRAYSIZE(gameObjectTypes));
		ImGui::Combo("GameObject Names", &currentSelectedGameObjectName, gameObjectMeshes, IM_ARRAYSIZE(gameObjectMeshes));

		if (ImGui::Button("Add Object"))
		{
			level_editor->add_models(gameObjectMeshes[currentSelectedGameObjectName], gameObjectTypes[currentSelectedGameObjectType]);
		}


		ImGui::End();

		ImGui::Begin("Scene Objects");
		ImGui::SetWindowPos(ImVec2(0, window_height * 0.25));
		ImGui::SetWindowSize(ImVec2(window_width * 0.25, window_height * 1));

		static auto	getGameObjectNameFunc = [](void* vec, int idx, const char** out_text)
		{
				std::vector<GameObject*>* vector = static_cast<std::vector<GameObject*>*>(vec);
				if (idx < 0 || idx >= vector->size())
				{
					return false;
				}
				*out_text = vector->at(idx)->getName();
				return true;
		};
		

		ImGui::ListBox ("", &listboxGameobjectCurrentIndex, getGameObjectNameFunc, static_cast	<void*>(&level_editor->m_currentscene->v_gameObjects),
				level_editor->m_currentscene->v_gameObjects.size(), 40);

		ImGui::End();

		ImGui::Begin("Camera Properties");
		ImGui::SetWindowPos(ImVec2(window_width * 0.75, window_height * 0.75));
		ImGui::SetWindowSize(ImVec2(window_width * 0.25, window_height * 0.25));

		static float f0 = l_camera->GetMouseSensitivity();
		ImGui::SliderFloat("Mouse Sensivity", &f0, 0.25, 1.0);
		l_camera->SetMouseSensitivity(f0);

		static float f1 = l_camera->GetMovementSpeed();
		ImGui::SliderFloat("Movement Speed", &f1, 1.0f, 1000.0f);
		l_camera->SetMovementSpeed(f1);

		//Viewport slider
		static float viewportSize = 1.0f;
		ImGui::SliderFloat("ViewportSize", &viewportSize, 0.25, 1.0);

		ImGui::Text("Camera FOV: %f", l_camera->GetZoom());

		ImGui::End();

		ImGui::Begin("Object Properties");
		ImGui::SetWindowPos(ImVec2(window_width * 0.25, window_height * 0.75));
		ImGui::SetWindowSize(ImVec2(window_width * 0.5, window_height * 0.25));

		
		if (listboxGameobjectCurrentIndex >= 0)
		{
			// on change of name (if), update name
			if (ImGui::InputText("Name", nameBuffer, 64, ImGuiInputTextFlags_CharsNoBlank))
				level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]->setName(nameBuffer);
			// make sure the current name is in the text box – every frame
			strcpy_s(nameBuffer, level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]->getName());
			name_ = level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]->getName();
			//Transform
			static glm::vec3 EulerRotationAngles(0);
			static glm::vec3 scale(1);
			transform = level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]->getComponent<TransformComponent>();

			if (transform != nullptr)
			{
				// create input values for the position and orientation – access position directly
				ImGui::InputFloat3("Position", &transform->m_position.x); // vec3 x,y,z in order
				if (ImGui::InputFloat3("Euler Angles", &EulerRotationAngles.x))
				{
					// The value has changed, so need to update the orientation as it is stored as quat
					transform->m_orientation = glm::quat(glm::radians(EulerRotationAngles));
				}
				// just get the current values (in degrees)
				EulerRotationAngles = glm::degrees(glm::eulerAngles(transform->m_orientation));

				if(ImGui::InputFloat3("Scale", &scale.x))
				{
					transform->SetScale(scale);
				}
				scale = transform->m_scale;
			}

			if (ImGui::Button("Delete this Object"))
			{
				for (auto i = 0; i < level_editor->m_currentscene->v_gameObjects.size(); i++)
				{
					string _name_ = level_editor->m_currentscene->v_gameObjects[i]->getName();
					if (_name_ == name_)
					{
						level_editor->delete_models(i);
					}
				}
			}
		}


		if (ImGui::Button("Save Level"))
		{
			level_editor->saveLevelJSON("assets/levels/level2.json");
		}

		if (go_position)
		{
			if (transform != nullptr)
			{
				std::string name_list = level_editor->m_currentscene->v_gameObjects[listboxGameobjectCurrentIndex]->getName();
				if (name_list == "Boo_")
				{
					l_camera->SetPosition((transform->position() + glm::vec3(0.f, 300.f, 500.f)));
				}
				else
				{
					l_camera->SetPosition((transform->position() + glm::vec3(0.f, 800.f, 0.f)));
				}
			}
		}

		ImGui::End();
		glViewport(window_width - window_width * viewportSize, window_height - window_height * viewportSize, window_width * viewportSize, window_height * viewportSize);
	}
}

void LevelEditorIMGUI::camera(GLfloat xOffset, GLfloat yOffset)
{
	if (listboxGameobjectCurrentIndex >= -1 && (name_ == "Boo_" || "duck"))
	{
		if (transform != nullptr)
		{
			/*transform->yaw(xOffset);*/
		}
	}
}

void LevelEditorIMGUI::render(LevelCamera* l_camera, LevelEditor* level_editor)
{
	//SETUP IMGUI
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}


glm::vec3 &LevelEditorIMGUI::quat_to_vec3(const glm::quat q)
{
	auto roll = 0, pitch = 0, yaw = 0;

	// roll (x-axis rotation)
	double sinr = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	roll = atan2(sinr, cosr);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of ran
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	yaw = atan2(siny, cosy);

	glm::vec3 vec = { roll, pitch, yaw };
	return vec;
}


LevelEditorIMGUI::~LevelEditorIMGUI()
{

}