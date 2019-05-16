#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "camera/camera.h"
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderers/RendererManager.h"
#include "./model_loader/loader.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shaders/shader.h"
#include "renderers/SkyboxRenderer.h"
#include "renderers/CarRenderer.h"

using namespace std;
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow *window, Camera* cam);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float xPos = 0.0f;
float yPos = 0.0f;
bool firstMouse = true;
bool autoRotate = false;
bool autoScale = false;
bool autoTranslate = false;
bool keys[1024];
double updateTime = 0.0;
float scale = 1.0f;
float scaleDir = 1.0f;
float movementx = 0.0f;
float moveDir = 1.0f;
float rotation = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		autoRotate = !autoRotate;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		autoTranslate = !autoTranslate;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		autoScale = !autoScale;
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


void mouseCallback(GLFWwindow*, double xpos, double ypos) {
	if (firstMouse) {
		xPos = xpos;
		yPos = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - xPos;
	float yoffset = yPos - ypos;
	camera.mouseMoveHandler(-xoffset, -yoffset);
	xPos = xpos;
	yPos = ypos;
}

void move(GLfloat dtime) {
	if (keys[GLFW_KEY_W]) {
		camera.keyboardHandler(FORWARD, dtime);
	}
	if (keys[GLFW_KEY_S]) {
		camera.keyboardHandler(BACKWARD, dtime);
	}
	if (keys[GLFW_KEY_A]) {
		camera.keyboardHandler(LEFT, dtime);
	}
	if (keys[GLFW_KEY_D]) {
		camera.keyboardHandler(RIGHT, dtime);
	}
}

GLFWwindow* openGLallInit();
void initImGui(GLFWwindow* window);
void cleanAll();

void renderLoop(GLFWwindow* window);

float SCR_WIDTH = 800;
float SCR_HEIGHT = 800;
const float SKYBOX_SIZE = 200.0f;


int main() {
	GLfloat curFrame = 0.0f, lastFrame = 0.0f;
	GLFWwindow* window = openGLallInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	if (window == NULL)
		return -1;
	glEnable(GL_DEPTH_TEST);
	std::vector<std::string> skyboxTextures = {
		"res/sky/right.jpg",
		"res/sky/left.jpg",
		"res/sky/top.jpg",
		"res/sky/down.jpg",
		"res/sky/front.jpg",
		"res/sky/back.jpg"
	};
	// init renderer
	RendererManager renderers;
	SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);
	CarRenderer carRenderer = CarRenderer();

	// init game object
	// Car car = Car("res/car/Mech_F_432/Material/mech_f_432.obj");
	// Car car = Car("res/car/Lancia Delta/Material/lacia_delta_9.obj");
	// Car car = Car("res/car/Lamborghini huracan/huracan in 2.8.obj");
	// Car car = Car("res/Barrel/Barrel02.obj");
	Car car = Car("res/car/newcar2/Avent.obj");
	// Shader shader("./src/shaders/carshader.vs", "./src/shaders/carshader.fs");
	// ModelLoader loader("res/car/Mech_F_432/Material/mech_f_432.obj");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 800.0f);
	while (!glfwWindowShouldClose(window)) {
		curFrame = glfwGetTime();
		// process config
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		// shader.use();
		// glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// glm::mat4 view = camera.getViewMat();
		// shader.setMat4("projection", projection);
		// shader.setMat4("view", view);
		// glm::mat4 model = glm::mat4(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		// model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		// model = glm::scale(model, glm::vec3(scale, scale, scale));
		// model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		// model = glm::translate(model, glm::vec3(movementx + 2, 0, 0));
		// shader.setMat4("model", model);
		// shader.setFloat("ambient", 1.0f);
		// shader.setFloat("diffuse", 1.0f);
		// shader.setFloat("specular", 0.5f);
		// shader.setVec3("viewPos", camera.position);
		// shader.setVec3("lightPos", glm::vec3(0, 0, 3));
		// move(curFrame - lastFrame);
		// loader.draw(shader);
		// ----------------------------------
		// render sky box
		skybox.render(camera.getViewMat(), projection);

		// ----------------------------------
		// render car
		carRenderer.render(car, &RendererManager::headlight, camera.getViewMat(), projection);
		move(curFrame - lastFrame);
		lastFrame = curFrame;
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void initImGui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 130";
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void cleanAll()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void renderLoop(GLFWwindow* window) {
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
}


GLFWwindow* openGLallInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "firWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	return window;
}




