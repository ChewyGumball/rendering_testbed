#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <cstdio>
#include <string>

#include <fstream>

#include <Renderer\OpenGL\OpenGLRenderer.h>
#include <Models\Model.h>
#include <Models\ModelInstance.h>
#include <Models\Mesh.h>
#include <Renderer\Camera.h>
#include <Models/ModelLoader.h>

#include <Util\FileUtils.h>
#include <random>
#include "main.h"

int width = 800;
int height = 800;


std::shared_ptr<Shader> colourShader()
{
	return std::make_shared<Shader>(
		Util::File::ReadWholeFile(R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\colour.vert)"),
		Util::File::ReadWholeFile(R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\colour.frag)")
		);
}

std::shared_ptr<Shader> phongShader()
{
	return std::make_shared<Shader>(
		R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\normal.vert)",
		R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\normal.frag)",
		true
		);
}

std::shared_ptr<Mesh> buddha()
{
	return ModelLoader::loadOBJFile(R"(F:\Users\Ben\Desktop\buddha.obj)");
}

std::shared_ptr<Mesh> buddhaBin()
{
	return ModelLoader::loadBinFile(R"(F:\Users\Ben\Desktop\buddha.mbin)");
}

std::shared_ptr<Mesh> cube()
{
	return ModelLoader::loadOBJFile(R"(F:\Users\Ben\Desktop\cube.obj)");
}

std::vector<std::shared_ptr<ModelInstance>> makeLotsOfCubes()
{
	Model model(cube(), colourShader());

	std::vector<std::shared_ptr<ModelInstance>> instances;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(-30, 30);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			std::shared_ptr<ModelInstance> instance = std::make_shared<ModelInstance>(model);
			instance->translate(glm::vec3(distribution(generator), distribution(generator), distribution(generator)));
			instance->rotate(glm::vec3(1, 0, 0), distribution(generator));
			instance->rotate(glm::vec3(0, 1, 0), distribution(generator));
			instances.push_back(instance);
		}
	}

	return instances;
}

std::vector<std::shared_ptr<ModelInstance>> makeBuddha()
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	Model model(buddhaBin(), phongShader());
	instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

std::vector<std::shared_ptr<ModelInstance>> makeCube()
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	Model model(cube(), phongShader());
	instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

void convert(int argc, char* argv[])
{
	std::string filename(argv[2]);
	std::string path = filename.substr(0, filename.find_last_of("/\\") + 1);

	std::shared_ptr<Mesh> mesh = ModelLoader::loadOBJFile(filename);
	std::ofstream convertedFile(argv[3], std::ios::out | std::ios::binary);

	std::vector<float> vertices = mesh->rawVertexData();
	std::vector<int> indices = mesh->indexData();

	int64_t vertexCount = vertices.size();
	int64_t indexCount = indices.size();

	char format = mesh->vertexFormat().formatData();

	convertedFile.write(&(format), sizeof(char));
	convertedFile.write(reinterpret_cast<char*>(&vertexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(vertices.data()), vertexCount * sizeof(float));
	convertedFile.write(reinterpret_cast<char*>(&indexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(int));

	convertedFile.close();
}

int main(int argc, char *argv[])
{
	if (argc > 1 && std::string(argv[1]) == "convert")
	{
		convert(argc, argv);
		return 0;
	}
	/*
	if (argc != 2)
	{
		std::printf("Arguments: {filename} - file name of model to load and render");
		return 0;
	}
	std::string modelFile(argv[1]);

	*/

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, "Hellow World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		glfwTerminate();
		std::printf("ERROR: %s\n", glewGetErrorString(error));
		return -1;
	}

	glfwSwapInterval(1);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	OpenGLRenderer renderer;

	//auto instances = makeLotsOfCubes();
	auto instances = makeBuddha();
	for (auto instance : instances)
	{
		renderer.addModelInstance(instance);
	}

	renderer.addPointLight(PointLight(glm::vec3(1, 1, -1), glm::vec3(0.5, 0.2, 0.9)));
	renderer.addPointLight(PointLight(glm::vec3(-1, 1, -1), glm::vec3(0, 0.4, 0.1)));

	Camera c(glm::vec3(1, 1, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0, 1.0f);

	double lastTime = glfwGetTime();
	double cumulative = 0;
	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		Util::File::MonitorFiles();
		for (auto instance : instances)
		{
			instance->rotate(glm::vec3(0, 1, 0), 0.7 * (currentTime - lastTime));
			//instance->rotate(glm::vec3(1, 1, 0), 1 * (currentTime - lastTime));
			//instance->rotate(glm::vec3(0, 1, 0), 0.3 * (currentTime - lastTime));
		}
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.draw(c);
		glfwSwapBuffers(window);
		cumulative += currentTime - lastTime;
		lastTime = currentTime;
		frames++;
		if (cumulative >= 1)
		{
			std::printf("%d frames\n", frames);
			cumulative = 0;
			frames = 0;
		}
	}

	glfwTerminate();
	return 0;
}
