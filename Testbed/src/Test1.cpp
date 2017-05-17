#include "Test1.h"

#include <Util/FileUtils.h>
#include <Util/StringUtils.h>

#include <Scene/SceneLoader.h>
#include <Resources/ModelInstance.h>

namespace {
	void rotateModelInstance(std::shared_ptr<Renderer::ModelInstance> instance, glm::vec3 axis, float angle) {
		instance->instanceData().set("transform", glm::rotate(instance->instanceData().getMat4("transform"), angle, axis));
	}
}

void Test1::handleKeyboardInput(int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_P) {
			wireframe = !wireframe;
			gui->wireframe(wireframe);
		}

		if (key == GLFW_KEY_M) {
			rotateInstances = !rotateInstances;
		}

		if (key == GLFW_KEY_V) {
			vsync = !vsync;
			if (vsync) {
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}
		}
	}
}

void Test1::handleMouseInput(double xPos, double yPos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		for (auto camera : cameras) {
			if (yPos != mouseY)
			{
				camera->pitch(static_cast<float>(yPos - mouseY) * -0.01f);
			}
			if (xPos != mouseX)
			{
				camera->yaw(static_cast<float>(xPos - mouseX) * -0.01f);
			}
		}
	}

	mouseY = yPos;
	mouseX = xPos;
}

void Test1::draw()
{
	vsync = false;
	glfwSwapInterval(0);

	double lastTime = glfwGetTime();
	double cumulative = 0;
	int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		doCameraMovement();

		double currentTime = glfwGetTime();
		if (rotateInstances) {
			for (auto instance : instances)
			{
				rotateModelInstance(instance, glm::vec3(0.0f, 1.0f, 0.0f), 0.7f * static_cast<float>(currentTime - lastTime));
			}
		}
		
		uint64_t tricount = 0;
		for (std::shared_ptr<Scene::RenderPass> pass : scene.passes())
		{
			tricount += pass->draw();
		}
		tricount += gui->draw();

		glfwSwapBuffers(window);
		cumulative += currentTime - lastTime;
		lastTime = currentTime;
		frames++;
		if (cumulative >= 1)
		{
			Util::File::MonitorFiles();
			fpsCounter.text(Util::String::Format(formatString, cumulative / frames, frames, frames / cumulative, tricount, vsync ? "on" : "off"));
			cumulative = 0;
			frames = 0;
		}
	}
}

void Test1::doCameraMovement()
{
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
	}
}

Test1::Test1(GLFWwindow* window, std::shared_ptr<Renderer::IRenderer> renderer, int windowWidth, int windowHeight, std::string sceneFileName)
	: window(window),
	scene(Scene::SceneLoader::loadWorld(renderer, sceneFileName)),
	cameras({ scene.camera("layer1Camera"), scene.camera("layer1CameraQuaternion") }),
	instances(scene.modelInstances("layer1Models")),
	rotateInstances(false),
	wireframe(false),
	guiCamera(std::make_shared<Scene::Cameras::Camera>(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec4(0, windowWidth, 0, windowHeight))),
	gui(std::make_shared<Scene::RenderPass>(renderer)),
	f(std::make_shared<Scene::Text::Font>("F:/Users/Ben/Documents/Projects/RenderingTestbed/Testbed/consola.ttf", 16)),
	formatString("%4.4fms per frame\r\n%d frames, %5.4f fps, %I64d triangles\nVSYNC: %s"),
	fpsCounter(Util::String::Format(formatString, 0, 0, 0.f, 0ull, "off"), f, glm::vec3(5, windowHeight, 0), glm::vec4(0.4, 0.7, 1, 1))
{
	gui->clearBuffers(false);
	gui->depthTest(false);
	gui->camera(guiCamera);
	gui->wireframe(wireframe);
	
	fpsCounter.addToRenderPass(gui);

	renderer->createPendingResources();

	glfwGetCursorPos(window, &mouseX, &mouseY);
}


Test1::~Test1()
{
}
