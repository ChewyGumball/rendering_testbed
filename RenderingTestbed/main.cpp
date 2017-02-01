#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <cstdio>
#include <string>

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

	char format = mesh->vertexFormat().formatData();

	convertedFile.write(&(format), sizeof(char));
	convertedFile.write(reinterpret_cast<char*>(&vertexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(vertices.data()), vertexCount * sizeof(float));
	convertedFile.write(reinterpret_cast<char*>(&indexCount), sizeof(int64_t));
	convertedFile.write(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(int));

	convertedFile.close();
}

void printHelp() {}

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

	glfwSwapInterval(1);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Scene scene = SceneLoader::loadScene(sceneFile);
	std::vector<std::shared_ptr<ModelInstance>> instances = scene.modelInstances("layer1Models");
	Camera& c = scene.camera("layer1Camera");
	const std::vector<std::shared_ptr<RenderPass>>& passes = scene.passes();

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
			c.move(0.1f * c.right());
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			c.move(-0.1f * c.right());
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			c.move(0.1f * c.forward());
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			c.move(-0.1f * c.forward());
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

		for (std::shared_ptr<RenderPass> pass : passes)
		{
			pass->clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pass->draw(c, false);
		}
		glfwSwapBuffers(window);
		cumulative += currentTime - lastTime;
		lastTime = currentTime;
		frames++;
		if (cumulative >= 1)
		{
			Util::File::MonitorFiles(); 
			uint64_t tricount = 0;
			for (std::shared_ptr<RenderPass> pass : passes)
			{
				tricount += pass->trianglesDrawn();
			}

			std::printf("%d frames, %I64d triangles\n", frames, tricount);
			cumulative = 0;
			frames = 0;
		}
	}

	glfwTerminate();
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		if (std::string(argv[1]) == "convert")
		{
			convert(argc, argv);
		} 
		else if (std::string(argv[1]) == "render")
		{
			renderScene(std::string(argv[2]));
		}
		return 0;
	}
	else
	{
		printHelp();
	}
}
