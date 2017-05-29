#pragma once
#include <vector>
#include <memory>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <Renderer/IRenderer.h>

#include <Cameras/Camera.h>
#include <Text/Font.h>

#include <Scene/World.h>
#include <Text/RenderableText.h>

class Test1
{
private:
	GLFWwindow* window;
	Scene::World scene;

	double mouseX, mouseY;
	bool rotateInstances;
	bool wireframe;
	bool vsync;

	std::shared_ptr<Scene::Text::Font> f;
	std::string formatString;
	std::string cameraFormatString;
	Scene::Text::RenderableText fpsCounter;
	Scene::Text::RenderableText cameraDisplay;

	std::shared_ptr<Scene::Cameras::Camera> guiCamera;
	std::shared_ptr<Scene::RenderPass> gui;

	std::vector<std::shared_ptr<Scene::Cameras::Camera>> cameras;
	std::shared_ptr<Scene::ModelGroup> instances;
	
	void doCameraMovement();

public:
	Test1(GLFWwindow* window, std::shared_ptr<Renderer::IRenderer> renderer, int windowWidth, int windowHeight, std::string sceneFileName);
	~Test1();
	void handleKeyboardInput(int key, int scancode, int action, int mods);
	void handleMouseInput(double xPos, double yPos);
	void draw();
};

