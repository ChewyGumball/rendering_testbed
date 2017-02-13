
#include <windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>

#include <fstream>

#include <Renderer\RenderPass.h>
#include <Models\ModelInstance.h>
#include <Renderer\Camera.h>
#include <Scene\SceneLoader.h>
#include <Models\ModelLoader.h>

#include <Util\FileUtils.h>
#include <random>

int width = 800;
int height = 800;
double mouseX, mouseY;

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

void handleInput(std::vector<std::shared_ptr<Camera>> cameras, GLFWwindow* window) {
	glfwPollEvents();
	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);

	for (auto camera : cameras) {
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera->move(-0.1f * camera->right());
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera->move(0.1f * camera->right());
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera->move(0.1f * camera->forward());
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera->move(-0.1f * camera->forward());
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			camera->move(0.1f * camera->up());
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			camera->move(-0.1f * camera->up());
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
		{
			if (newMouseY != mouseY)
			{
				camera->pitch(static_cast<float>(newMouseY - mouseY) * -0.01f);
			}
			if (newMouseX != mouseX)
			{
				camera->yaw(static_cast<float>(newMouseX - mouseX) * -0.01f);
			}
		}
	}

	mouseY = newMouseY;
	mouseX = newMouseX;
}

void renderScene(std::string sceneFile)
{
	if (!glfwInit())
	{
		return;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		glfwTerminate();
		std::printf("ERROR: %s\n", glewGetErrorString(error));
		return;
	}

	glfwSwapInterval(0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Scene scene = SceneLoader::loadScene(sceneFile);
	std::vector<std::shared_ptr<ModelInstance>> instances = scene.modelInstances("layer1Models");
	std::vector<std::shared_ptr<ModelInstance>> instances2 = scene.modelInstances("cameraTest");
	const std::vector<std::shared_ptr<RenderPass>>& passes = scene.passes();
	
	std::vector<std::shared_ptr<Camera>> cameras{ scene.camera("layer1Camera"), scene.camera("layer1CameraQuaternion") };

	double lastTime = glfwGetTime();
	double cumulative = 0;
	int frames = 0;

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		for (auto instance : instances)
		{
			instance->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.7f * static_cast<float>(currentTime - lastTime));
		}

		handleInput(cameras, window);

		uint64_t tricount = 0;
		for (std::shared_ptr<RenderPass> pass : passes)
		{
			tricount += pass->draw();
		}

		glfwSwapBuffers(window);
		cumulative += currentTime - lastTime;
		lastTime = currentTime;
		frames++;
		if (cumulative >= 1)
		{
			Util::File::MonitorFiles();
			std::printf("%d frames, %I64d triangles\n", frames, tricount);
			cumulative = 0;
			frames = 0;
		}
	}

	glfwTerminate();
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
	else if (std::string(argv[1]) == "render" && argc >= 4)
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
		if (argc != 1 || std::string(argv[1]) != "help") {
			std::cout << "Invalid Arguments" << std::endl << std::endl;
		}
		printHelp();
	}

	return 0;
}
