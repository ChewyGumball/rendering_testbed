#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <cstdio>
#include <string>

#include <fstream>

#include <Renderer\LayerPass.h>
#include <Models\Model.h>
#include <Models\ModelInstance.h>
#include <Models\Mesh.h>
#include <Renderer\Camera.h>
#include <Models/ModelLoader.h>

#include <Util\FileUtils.h>
#include <random>

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

std::shared_ptr<Shader> finalScreenShader()
{
	return std::make_shared<Shader>(
		R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\final.vert)",
		R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\final.frag)",
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

std::shared_ptr<Mesh> dragonBin()
{
	return ModelLoader::loadBinFile(R"(F:\Users\Ben\Desktop\dragon.mbin)");
}

std::shared_ptr<Mesh> cube()
{
	return ModelLoader::loadOBJFile(R"(F:\Users\Ben\Desktop\cube.obj)");
}
std::shared_ptr<Mesh> fbx()
{
	//return ModelLoader::loadFBXFile(R"(F:\Users\Ben\Documents\Projects\New Unity Project\Assets\Art\World\Wall\Wall_Tower.fbx)");
	return ModelLoader::loadFBXFile(R"(F:\Users\Ben\Documents\Projects\New Unity Project\Assets\Art\World\Wall\Wall_Prefab_02.fbx)");
}

std::vector<std::shared_ptr<ModelInstance>> makeLotsOfCubes()
{
	std::shared_ptr<Model> model = std::make_shared<Model>(cube(), phongShader());

	std::vector<std::shared_ptr<ModelInstance>> instances;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(-30, 30);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			std::shared_ptr<ModelInstance> instance = std::make_shared<ModelInstance>(model);
			instance->translate(glm::vec3(distribution(generator), distribution(generator), 5 + distribution(generator)));
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
	std::shared_ptr<Model> model = std::make_shared<Model>(buddhaBin(), phongShader());
	instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

std::vector<std::shared_ptr<ModelInstance>> makeFBX()
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	std::shared_ptr<Model> model = std::make_shared<Model>(fbx(), phongShader());
	instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

std::vector<std::shared_ptr<ModelInstance>> makeDragon()
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	std::shared_ptr<Model> model = std::make_shared<Model>(dragonBin(), phongShader());
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(-5, 5);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::shared_ptr<ModelInstance> instance = std::make_shared<ModelInstance>(model);
			//instance->translate(glm::vec3(distribution(generator), distribution(generator), distribution(generator)));
			instance->translate(glm::vec3(i * 2.5 - 0.5,j * 2.5  - 0.5, 5));
			instance->rotate(glm::vec3(1, 0, 0), distribution(generator));
			instance->rotate(glm::vec3(0, 1, 0), distribution(generator));
			instances.push_back(instance);
		}
	}
	//instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

std::vector<std::shared_ptr<ModelInstance>> makeCube()
{
	std::vector<std::shared_ptr<ModelInstance>> instances;
	std::shared_ptr<Model> model = std::make_shared<Model>(cube(), phongShader());
	instances.push_back(std::make_shared<ModelInstance>(model));

	return instances;
}

std::shared_ptr<ModelInstance> screenQuad(std::shared_ptr<TextureBuffer> buffer)
{
	std::vector<Vertex> vertices{
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0)),
		Vertex(glm::vec3( 1, -1, 0), glm::vec2(1, 0)),
		Vertex(glm::vec3( 1,  1, 0), glm::vec2(1, 1)),
		Vertex(glm::vec3(-1,  1, 0), glm::vec2(0, 1))
	};

	std::vector<int> indices{
		0, 1, 2,
		0, 2, 3
	};
	std::shared_ptr<Mesh> m = std::make_shared<Mesh>(VertexFormats::Position_Texture, Vertex::flatten(VertexFormats::Position_Texture, vertices), indices);
	std::shared_ptr<Model> quad = std::make_shared<Model>(m, finalScreenShader());
	quad->setTexture("screenTexture", buffer);

	return std::make_shared<ModelInstance>(quad);
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
	glEnable(GL_CULL_FACE);

	//Camera c(glm::vec3(1, 1, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0, 1.0f);
	Camera c(glm::vec3(-1500,0,0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 45.0, 1.0f);

	LayerPass pass1;
	std::shared_ptr<TextureBuffer> colourBuffer = std::make_shared<TextureBuffer>(width, height, GL_RGB, GL_RGB8);
	std::shared_ptr<TextureBuffer> depthBuffer = std::make_shared<TextureBuffer>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24);

	pass1.setColourBuffer(colourBuffer);
	pass1.setDepthBuffer(depthBuffer);

	LayerPass finalPass;
	finalPass.addModelInstance(screenQuad(colourBuffer));

	//auto instances = makeLotsOfCubes();
	//auto instances = makeDragon();
	auto instances = makeFBX();
	for (auto instance : instances)
	{
		pass1.addModelInstance(instance);
	}

	pass1.addPointLight(PointLight(glm::vec3(15, 20, 7), glm::vec3(0.5, 0.2, 0.9), 10000));
	pass1.addPointLight(PointLight(glm::vec3(-1, -1, 6), glm::vec3(0, 0.4, 0.1), 10000));
	
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
			//instance->rotate(glm::vec3(1, 1, 0), 1 * (currentTime - lastTime));
			//instance->rotate(glm::vec3(0, 1, 0), 0.3 * (currentTime - lastTime));
		}
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			c.move(0.1f * c.right() * 100.f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			c.move(-0.1f * c.right()* 100.f);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			c.move(0.1f * c.forward()* 100.f);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			c.move(-0.1f * c.forward()* 100.f);
		}

		double newMouseX, newMouseY;
		glfwGetCursorPos(window, &newMouseX, &newMouseY);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
		{
			if (newMouseX != mouseX)
			{
				c.rotateLocal(c.up(), static_cast<float>(newMouseX - mouseX) * 0.01f);
			}
			if (newMouseY != mouseY)
			{
				c.rotateLocal(c.right(), static_cast<float>(newMouseY - mouseY) * -0.01f);
			}
		}
		mouseY = newMouseY;
		mouseX = newMouseX;



		pass1.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pass1.draw(c);
		finalPass.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		finalPass.draw(c, false);
		glfwSwapBuffers(window);
		cumulative += currentTime - lastTime;
		lastTime = currentTime;
		frames++;
		if (cumulative >= 1)
		{
			Util::File::MonitorFiles();
			uint64_t tricount = pass1.trianglesDrawn();

			std::printf("%d frames, %I64d triangls\n", frames, tricount);
			cumulative = 0;
			frames = 0;
		}
	}

	glfwTerminate();
	return 0;
}
