#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <cstdio>
#include <string>

#include <Renderer\OpenGL\OpenGLRenderer.h>
#include <Models\Model.h>
#include <Models\ModelInstance.h>
#include <Models\Mesh.h>
#include <Renderer\Camera.h>

#include <Util\FileUtils.h>

int width = 800;
int height = 800;


std::shared_ptr<Shader> colourShader()
{
	return std::make_shared<Shader>(
		Util::File::ReadWholeFile(R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\colour.vert)"),
		Util::File::ReadWholeFile(R"(F:\Users\Ben\Documents\Projects\RenderingTestbed\RenderingTestbed\src\Shaders\colour.frag)")
		);
}

std::shared_ptr<Mesh> cube()
{

	std::vector < Vertex > vertices = {
		//front
		Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),

		//top
		Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),

		//left
		Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),

		//right
		Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),

		//back
		Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),

		//bottom
		Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
	};

	std::vector<int> indices = {
		0,1,2,
		3,4,5,

		6,7,8,
		9,10,11,

		12,13,14,
		15,16,17,

		18,19,20,
		21,22,23,

		24,25,26,
		27,28,29,

		30,31,32,
		33,34,35
	};

	return std::make_shared<Mesh>(vertices, indices);
}

int main(int argc, char *argv[])
{
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

	Model model(cube(), colourShader());
	std::shared_ptr<ModelInstance> instance = std::make_shared<ModelInstance>(model);
	std::shared_ptr<ModelInstance> instance2 = std::make_shared<ModelInstance>(model);

	instance->rotate(glm::vec3(1, 0, 0), 45);
	instance->rotate(glm::vec3(0, 0, 1), 45);

	instance2->scale(glm::vec3(0.75f, 0.5, 1));
	instance2->translate(glm::vec3(2, 0, 0));
	instance2->rotate(glm::vec3(1, 0, 0), 45);

	renderer.addModelInstance(instance);
	renderer.addModelInstance(instance2);

	Camera c(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 90.0, 1.0f);

	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		instance->rotate(glm::vec3(0, 1, 0), 1 * (currentTime - lastTime));
		instance2->rotate(glm::vec3(0, 1, 0), 1 * (currentTime - lastTime));
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.draw(c);
		glfwSwapBuffers(window);
		lastTime = currentTime;
	}

	glfwTerminate();
	return 0;
}
