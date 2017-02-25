#pragma once
#include <vector>
#include <memory>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <Renderer/Camera.h>
#include <Fonts/Font.h>

#include <Scene/Scene.h>
#include <Scene/RenderableText.h>

class Test1
{
private:
	GLFWwindow* window;
	Scene scene;

	double mouseX, mouseY;
	bool rotateInstances;
	bool wireframe;

	std::shared_ptr<Font> f;
	std::string formatString;
	RenderableText fpsCounter;

	std::shared_ptr<Camera> guiCamera;
	std::shared_ptr<RenderPass> gui;

	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<ModelInstance>> instances;
	
	void doCameraMovement();

public:
	Test1(GLFWwindow* window, int windowWidth, int windowHeight, std::string sceneFileName);
	~Test1();
	void handleKeyboardInput(int key, int scancode, int action, int mods);
	void handleMouseInput(double xPos, double yPos);
	void draw();
};

