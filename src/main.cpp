#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "game_object/car.h"
#include "camera/camera.h"
#include "stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
#include "renderers/entityRenderer.h"

using namespace std;
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow *window, Camera* cam);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	glm::vec3 lightPos = glm::vec3(-2.0f, 5.0f, -1.0f);
float planeVertices[48] = {
	// positions            // normals         // texcoords
	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

	25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};
unsigned int planeVAO;
unsigned int depthMapFBO;
int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
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

void initPlaneVAO() {
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void initShadow() {
	// ---------------------------------------------
	// frame depth map
	const unsigned int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &RendererManager::depthMap);
	glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RendererManager::depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

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

float SCR_WIDTH = 4000;
float SCR_HEIGHT = 4000;
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
	EntityRenderer entityRenderer = EntityRenderer();

	// init game object
	initPlaneVAO();
	initShadow();
	Car car = Car("res/car/newcar2/Avent.obj");
	ModelLoader loader = ModelLoader("res/car/newcar2/Avent.obj");
	// plane shader and shadow shader
	Shader shader("./src/shaders/shadow_mapping.vs", "./src/shaders/shadow_mapping.fs");
	// Shader shader("./src/shaders/carshader.vs", "./src/shaders/carshader.fs");
	Shader simpleDepthShader("./src/shaders/shadow_depth.vs", "./src/shaders/shadow_depth.fs");
	shader.setInt("shadowMap", 100);
	// ModelLoader loader("res/car/Mech_F_432/Material/mech_f_432.obj");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 800.0f);
	while (!glfwWindowShouldClose(window)) {
		curFrame = glfwGetTime();
		// process config
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0, 1, 0));
		// lightView = glm::lookAt(glm::vec3(RendererManager::headlight.position), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		simpleDepthShader.setMat4("model", model);
		// entityRenderer.render(car, &RendererManager::headlight, camera.getViewMat(), projection);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		model = glm::translate(model, glm::vec3(0, -1, 0));
		simpleDepthShader.setMat4("model", model);
		car.loader->draw(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// // reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ----------------------------------
		// render plane
		model = glm::mat4(1.0f);
		shader.use();
		shader.setBool("useInColor", true);
		shader.setVec3("bcolor", glm::vec3(1, 1, 1));
		shader.setMat4("model", model);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader.setMat4("projection", projection);
		shader.setMat4("view", camera.getViewMat());
		shader.setVec3("viewPos", camera.position);
		shader.setVec3("lightPos", lightPos);
		// shader.setVec3("lightPos", glm::vec3(RendererManager::headlight.position));
		shader.setFloat("ambient", 1.0f);
		shader.setFloat("diffuse", 1.0f);
		shader.setFloat("specular", 0.5f);
		glActiveTexture(GL_TEXTURE0 + 100);
		glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		model = glm::translate(model, glm::vec3(0, 0.2, 0));
		shader.setBool("useInColor", false);
		shader.setMat4("model", model);
		shader.setVec3("bcolor", glm::vec3(-1, -1, -1));
		// loader.draw(shader);
		// ----------------------------------

		// ----------------------------------
		// render car
		// entityRenderer.render(car, &RendererManager::headlight, camera.getViewMat(), projection);
		car.loader->draw(shader);
		move(curFrame - lastFrame);
		// 		// render sky box
		// skybox.render(camera.getViewMat(), projection);
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




