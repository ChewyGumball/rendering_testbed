
#include <windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>

#include <fstream>

#include <Drawing\RenderPass.h>
#include <Resources\ModelInstance.h>
#include <Cameras\Camera.h>
#include <Scene\SceneLoader.h>
#include <ModelLoader.h>
#include <Text\Font.h>

#include <Text/RenderableText.h>

#include <Util\FileUtils.h>
#include <Util\StringUtils.h>
#include <random>

#include "Test1.h"

int width = 800;
int height = 800;

Test1* test;

void convert(int argc, char* argv[])
{
	std::string filename(argv[2]);
	std::string path = filename.substr(0, filename.find_last_of("/\\") + 1);

	std::shared_ptr<Mesh> mesh = ModelLoader::loadOBJFile(filename);
	std::ofstream convertedFile(argv[3], std::ios::out | std::ios::binary);

	std::vector<float> vertices = mesh->vertexData();
	std::vector<int> indices = mesh->indexData();

	int64_t vertexCount = vertices.size();
	int64_t indexCount = indices.size();

	VertexAttribute format = mesh->vertexFormat().formatData();

	convertedFile.write(&(format), sizeof(VertexAttribute));
	convertedFile.write(reinterpret_cast<char*>(&vertexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(vertices.data()), vertexCount * sizeof(float));
	convertedFile.write(reinterpret_cast<char*>(&indexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(int));

	convertedFile.close();
}

void printGLFWError(int error, const char* description)
{
	std::cout << "GLFW ERROR (" << error << "): " << description << std::endl;
}

void printHelp() 
{
	std::cout << "USAGE: Testbed.exe [command] [options] [input filename] [output filename]" << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\thelp    - prints this help text" << std::endl;
	std::cout << "\tconvert - converts the .obj file specified to a binary representation that loads faster" << std::endl;
	std::cout << "\trender  - renders the scene specified in the given scene file" << std::endl;
	std::cout << "\t\tOptions:" << std::endl;
	std::cout << "\t\t\t-opengl    - use the OpenGL Renderer" << std::endl;
	std::cout << "\t\t\t-directx10 - use the DirectX10 Renderer" << std::endl;
}


void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	test->handleKeyboardInput(key, scancode, action, mods);
}

void handleMouseInput(GLFWwindow* window, double xpos, double ypos) {
	test->handleMouseInput(xpos, ypos);
}

void renderScene(std::string sceneFile)
{
	if (glfwInit())
	{
		GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
		if (window)
		{
			glfwMakeContextCurrent(window);
			glfwSetKeyCallback(window, handleKeyboardInput);
			glfwSetCursorPosCallback(window, handleMouseInput);

			GLenum error = glewInit();
			if (error == GLEW_OK) {
				test = new Test1(window, width, height, sceneFile);
				test->draw();
				delete test;
			}
			else {
				std::printf("ERROR: %s\n", glewGetErrorString(error));
			}

			glfwDestroyWindow(window);
		}

		glfwTerminate();
	}	
}

void directx(std::string sceneFile)
{
	if (!glfwInit())
	{
		return;
	}
	glfwSetErrorCallback(printGLFWError);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return;
	}
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwTerminate();
}

int main(int argc, char *argv[])
{
	if (argc >= 3 && std::string(argv[1]) == "convert")
	{
		convert(argc, argv);
	}
	else if (argc >= 4 &&std::string(argv[1]) == "render")
	{
		if (std::string(argv[2]) == "-opengl")
		{
			renderScene(std::string(argv[3]));
		}
		else if (std::string(argv[2]) == "-directx10")
		{
			directx(std::string(argv[3]));
		}
		else {
			std::cout << "Invalid renderer: " << argv[2] << std::endl;
		}
	}
	else
	{
		if (argc != 2 || std::string(argv[1]) != "help") {
			std::cout << "Invalid Arguments" << std::endl << std::endl;
		}
		printHelp();
	}

	return 0;
}
