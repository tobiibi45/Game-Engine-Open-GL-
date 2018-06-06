#include "GLFW_IMGUI_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "ft2build.h"
#include FT_FREETYPE_H 
#include <ImGUI/imgui.h>
#include "LevelEditor.h"
#include "LevelCameraEngine.h"
#include "LevelEditorIMGUI.h"
#include <chrono>
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace std::chrono_literals;
// use a fixed timestep of 1 / (60 fps) approx = 16 milliseconds
constexpr std::chrono::milliseconds dt(16ms);

std::vector<bool> GLFW_IMGUI_EngineCore::m_keyBuffer;
int GLFW_IMGUI_EngineCore::m_screenWidth;
int GLFW_IMGUI_EngineCore::m_screenHeight;
GLfloat GLFW_IMGUI_EngineCore::lastX;
GLfloat GLFW_IMGUI_EngineCore::lastY;
GLfloat GLFW_IMGUI_EngineCore::m_xOffset;
GLfloat GLFW_IMGUI_EngineCore::m_yOffset;
bool GLFW_IMGUI_EngineCore::keys[1024];
bool GLFW_IMGUI_EngineCore::firstMouse;
bool GLFW_IMGUI_EngineCore::mouse_enter;

//LevelCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
LevelCamera * l_camera = new LevelCamera(glm::vec3(60.0f, 200.0f, 6200.0f));
LevelEditorIMGUI* l_main = new LevelEditorIMGUI();
GLFW_IMGUI_EngineCore::~GLFW_IMGUI_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_IMGUI_EngineCore::initWindow(int width, int height, std::string windowName)
{
	showdemo = false;
	changeInput = false;
	change_input_type = false;
	go_position = false;
	found_player = false;
	load_level_editor = false;
	
	list_found = false;

	nbFrames = 0;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	m_screenWidth = width;
	m_screenHeight = height;

	lastX = m_screenWidth / 2.0f;
	lastY = m_screenHeight / 2.0f;

	m_xOffset = 0.f;
	m_yOffset = 0.f;

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	firstMouse = true;
	mouse_enter = true;

	m_window = glfwCreateWindow(mode->width, mode->height, windowName.c_str(), glfwGetPrimaryMonitor(), nullptr);
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);

	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}
	

	l_main->init(m_window);


	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);
	glfwSetCursorPosCallback(m_window, mouseMoveCallbackEvent);
	glfwSetScrollCallback(m_window, scrollcallback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	glfwSetCursorEnterCallback(m_window, mouseenteredcallback);
	glfwSetMouseButtonCallback(m_window, mousebuttoncallback);
	
	glfwSetCursorPos(m_window, lastX, lastY);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();
	setupDefaultFont();

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	//initCubeModel();

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 0);

	glLineWidth(2);
	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable alpha transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool GLFW_IMGUI_EngineCore::runEngine(Game& game)
{

	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	game.m_engineInterfacePtr = this;

	le = static_cast<LevelEditor*>(&game);
	//LevelEditor *level_editor = static_cast<LevelEditor*>(&game);
	// message loop

	while (!glfwWindowShouldClose(m_window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (!load_level_editor)
		{
			for (auto i = 0; i < le->m_currentscene->v_gameObjects.size(); i++) //sets the camera to the player position at the beginning
			{
				string _name_ = le->m_currentscene->v_gameObjects[i]->getName();
				if (_name_ == "Boo_")
				{
					l_main->listboxGameobjectCurrentIndex = i;
					l_camera->SetPosition((le->m_currentscene->v_gameObjects[i]->getComponent<TransformComponent>()->m_position + glm::vec3(0.f, 300.f, 500.f)));
				}
			}
		}

		if (l_main->changeInput && l_main->listboxGameobjectCurrentIndex != -1)
		{
			// check if we have set up any input handling
			game.m_inputHandler->handleInputs(m_keyBuffer);
		}

		if (load_level_editor)
		{
			l_main->update(m_window, l_camera, le);
		}

		doCollisionsFloor();
		doCollisionsWalls();
		doCollisions("key1", 3);
		doCollisions("key2", 3);
		doCollisions("key3", 3);
		doCollisions("cemetary_gate", 4);

		game.update(1); // update game logic
		game.render(); // prepare game to send info to the renderer in engine core

		if (load_level_editor)
		{
			l_main->render(l_camera, le);
		}

		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		if (!l_main->changeInput)
		{
			do_movement();
		}
		do_movement_default();
	}

	return true;
}

void GLFW_IMGUI_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// screen is between 0 and 1 with (0,0) at the bottom left corner
// typical use: m_engineInterfacePtr->renderText("Awesome Game Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(0.9, 0.9f, 0.1f));
void GLFW_IMGUI_EngineCore::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	// set the window to orthographic
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));

	float pixelValueX = x * m_screenWidth;
	float pixelValueY = y * m_screenHeight;

	glUseProgram(m_fontShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_fontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(m_fontShaderProgram, "textColour"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = pixelValueX + ch.bearing.x * scale;
		GLfloat ypos = pixelValueY - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pixelValueX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// go back to default shader
	glUseProgram(m_defaultShaderProgram);
}


void GLFW_IMGUI_EngineCore::setCamera(const CameraComponent* cam)
{
	// set the view and projection components of our shader to the camera values
	glm::mat4 projection = glm::perspective(glm::radians(cam->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 10000.0f);

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(cam->position()));

}

void GLFW_IMGUI_EngineCore::setCameraLevel(LevelCamera *camera)
{
	// set the view and projection components of our shader to the camera values
	projection = glm::perspective(glm::radians(l_camera->GetZoom()), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 10000.0f);

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(l_camera->GetViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(l_camera->GetPosition()));

}

void GLFW_IMGUI_EngineCore::setParticles(const ParticleEmiiterComponent p, LevelCamera *camera)
{
	glUseProgram(m_particleShaderProgram);
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::mat4 viewProjectionMatrix = projection * viewMatrix;
	glUniform3f(glGetUniformLocation(m_particleShaderProgram, "CameraRight_worldspace"),
		viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glUniform3f(glGetUniformLocation(m_particleShaderProgram, "CameraUp_worldspace"),
		viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	glUniformMatrix4fv(glGetUniformLocation(m_particleShaderProgram, "VP"), 1, GL_FALSE,
		&viewProjectionMatrix[0][0]);

}

void GLFW_IMGUI_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLFW_IMGUI_EngineCore::drawModel(Model* model, const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->render(m_defaultShaderProgram, modelMatrix);
}

//-----------------------------Private functions------------------------------
void GLFW_IMGUI_EngineCore::mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos)
{
	if (mouse_enter)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		GLfloat xOffset = xPos - lastX;
		GLfloat yOffset = lastY - yPos;

		m_xOffset = xOffset;
		m_yOffset = yOffset;

		lastX = xPos;
		lastY = yPos;

		l_main->camera(xOffset, yOffset);
		l_camera->ProcessMouseMovement(xOffset, yOffset);
	}
}

void GLFW_IMGUI_EngineCore::mouseenteredcallback(struct GLFWwindow* window, int enter)
{
	mouse_enter = enter != 0;
}

void GLFW_IMGUI_EngineCore::mousebuttoncallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		//std::cout << "Right Button Pressed " << std::endl;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		//std::cout << "Right Button Realeased " << std::endl;
	}
}

void GLFW_IMGUI_EngineCore::scrollcallback(GLFWwindow* window, double xOffset, double yOffset)
{
	l_camera->ProcessMouseScroll(yOffset);
}

void GLFW_IMGUI_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if(key >= 0 && key < 1024)
	{
		if(GLFW_PRESS == action)
		{
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action)
		{
			keys[key] = false;
		}
	}
}

void GLFW_IMGUI_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

void GLFW_IMGUI_EngineCore::do_movement()
{
	if (keys[GLFW_KEY_W])
	{
		l_camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		l_camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		l_camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		l_camera->ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_T])
	{
		load_level_editor = false;
	}
	if (keys[GLFW_KEY_Y])
	{
		load_level_editor = true;
	}
}

void GLFW_IMGUI_EngineCore::do_movement_default()
{
	if (keys[GLFW_KEY_T])
	{
		load_level_editor = false;
	}
	if (keys[GLFW_KEY_Y])
	{
		load_level_editor = true;
	}
}

void GLFW_IMGUI_EngineCore::loadShader(string vertexShaderFile, string fragmentShaderFile, GLuint& shaderProgram)
{
	// Load contents of vertex file
	std::ifstream inFile(vertexShaderFile);
	if (!inFile) {
		string errorMsg = "Error opening shader file: " + vertexShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2(fragmentShaderFile);
	if (!inFile2) {
		string errorMsg = "Error opening shader file: " + fragmentShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// loading some default shaders to get things up and running
void GLFW_IMGUI_EngineCore::setDefaultShaders()
{
	loadShader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag", m_defaultShaderProgram);
	loadShader("assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag", m_fontShaderProgram);

	// set the default shader
	glUseProgram(m_defaultShaderProgram);
}

// based on tutorial from https://learnopengl.com/#!In-Practice/Text-Rendering
void GLFW_IMGUI_EngineCore::setupDefaultFont()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &font_VAO);
	glGenBuffers(1, &font_VBO);
	glBindVertexArray(font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
	// dynamic draw as the text may change frequently
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// a simple function to initialise a cube model in memory
void GLFW_IMGUI_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

bool GLFW_IMGUI_EngineCore::checkCollision(GameObject* first, GameObject* second, int id)
{
	if (id == 1) // collision with floor
	{
		if (first != nullptr && second != nullptr)
		{
			first->getComponent<TransformComponent>()->m_position.y -= 9.8f; // gravity
			if (
				first->getComponent<TransformComponent>()->m_position.y + 50 < second->getComponent<TransformComponent>()->m_position.y + second->getComponent<TransformComponent>()->m_scale.y &&
				first->getComponent<TransformComponent>()->m_position.y + 50 > second->getComponent<TransformComponent>()->m_position.y - first->getComponent<TransformComponent>()->m_scale.y
				)
			{
				//std::cout << "COLLISION NOT DETECTED " << second->getName() << "  and  " << first->getName() << std::endl;
				return true;
			}
		//	std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << std::endl;
			first->getComponent<TransformComponent>()->m_position.y = 0.f;
			return false;
		}
	}
	if (id == 2) // collision with wall
	{

		if (first != nullptr && second != nullptr)
		{
			auto dx = (first->getComponent<TransformComponent>()->m_position.x - second->getComponent<TransformComponent>()->m_position.x);
			auto dz = (first->getComponent<TransformComponent>()->m_position.z - second->getComponent<TransformComponent>()->m_position.z);

			auto dist = ((dx * dx) + (dz * dz));

			if (dist <= first->getComponent<TransformComponent>()->m_scale.x + first->getComponent<TransformComponent>()->m_position.x + first->getComponent<TransformComponent>()->m_scale.z + first->getComponent<TransformComponent>()->m_position.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				first->getComponent<TransformComponent>()->m_position.x = 59.f;
				first->getComponent<TransformComponent>()->m_position.z = -5200.f;
				return true;
			}
			//std::cout << "Bushes Xposs:  " << second->getComponent<TransformComponent>()->m_position.x << endl;
			//std::cout << "COLLISION NOT DETECTED " << s ond->getName() << "  and  " << first->getName() << std::endl;
			return false;
		}
	}

	if (id == 3) // collision with keys
	{

		if (first != nullptr && second != nullptr)
		{
			auto dx = (first->getComponent<TransformComponent>()->m_position.x - second->getComponent<TransformComponent>()->m_position.x);
			auto dz = (first->getComponent<TransformComponent>()->m_position.z - second->getComponent<TransformComponent>()->m_position.z);

			auto dist = ((dx * dx) + (dz * dz));

			if (dist <= first->getComponent<TransformComponent>()->m_scale.x + first->getComponent<TransformComponent>()->m_position.x + first->getComponent<TransformComponent>()->m_scale.z + first->getComponent<TransformComponent>()->m_position.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				//delete key
				for (auto i = 0; i < le->m_currentscene->v_gameObjects.size(); i++)
				{
					string _name_ = le->m_currentscene->v_gameObjects[i]->getName();
					if (_name_ == second->getName())
					{
						le->audioEngine->play2D(le->Sparkle);
						if (le->delete_models(i))
						{
							first->add_keys(1);
						}
						std::cout << "Key Found: " << second->getName() << endl;
					}
				}
				return true;
			}
			return false;
		}
	}

	if (id == 4) // collision with gate
	{

		if (first != nullptr && second != nullptr)
		{

			if (first->getComponent<TransformComponent>()->m_position.z < second->getComponent<TransformComponent>()->m_position.z + second->getComponent<TransformComponent>()->m_scale.z &&
				first->getComponent<TransformComponent>()->m_position.z > second->getComponent<TransformComponent>()->m_position.z - first->getComponent<TransformComponent>()->m_scale.z)
			{
				std::cout << "COLLISION DETECTED " << second->getName() << "  and  " << first->getName() << "  at   " << second->getComponent<TransformComponent>()->m_position.x << std::endl;
				if (first->get_key() < 2)
				{
					first->getComponent<TransformComponent>()->m_position.x = 4780.f;
					first->getComponent<TransformComponent>()->m_position.z = -7640.f;
				}
				else if (first->get_key() >= 2)
				{
					le->render_win = true;
				}
				return true;
			}

			/*if (first->get_key() >= 2)
			{
				for (auto i = 0; i < le->m_currentscene->v_gameObjects.size(); i++)
				{
					string _name_ = le->m_currentscene->v_gameObjects[i]->getName();
					if (_name_ == "cemetary_gate")
					{
						le->delete_models(i);
					}
				}
			}*/
			return false;
		}
	}
	return false;
}

void GLFW_IMGUI_EngineCore::doCollisionsWalls()
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto & v_gameObject : le->m_currentscene->v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		std::string name2 = v_gameObject->getName();
		if (name == "Boo_" && !found_player)
		{
			first = v_gameObject;
			found_player = true;
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
	}
	for (auto & v_gameObject : le->m_currentscene->v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		if (name == "bush")
		{
			second = v_gameObject;
			checkCollision(first, second, 2);
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
	}
}

void GLFW_IMGUI_EngineCore::doCollisionsFloor()
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto & v_gameObject : le->m_currentscene->v_gameObjects)
	{
		std::string name = v_gameObject->getName();
		if (name == "Boo_")
		{
			first = v_gameObject;
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
		else if (name == "floor")
		{
			second = v_gameObject;
			//std::cout << "NAME 2:  " << second->getName() << std::endl;
		}
		checkCollision(first, second, 1);
	}
}

void GLFW_IMGUI_EngineCore::doCollisions(string name_, int id)
{
	GameObject* first = nullptr;
	GameObject* second = nullptr;
	for (auto i = 0; i < le->m_currentscene->v_gameObjects.size(); i++)
	{
		std::string name = le->m_currentscene->v_gameObjects[i]->getName();
		if (name == "Boo_")
		{
			first = le->m_currentscene->v_gameObjects[i];
			//std::cout << "NAME 1:  " << first->getName() << std::endl;
		}
		else if (name == name_)
		{
			second = le->m_currentscene->v_gameObjects[i];
		}
		checkCollision(first, second, id);
	}
	
}



