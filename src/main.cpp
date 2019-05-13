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



// #define IMGUI_IMPL_OPENGL_LOADER_GLAD
// #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
// #include <GL/gl3w.h>    // Initialize with gl3wInit()
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
// #include <GL/glew.h>    // Initialize with glewInit()
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
// #include <glad/glad.h>  // Initialize with gladLoadGL()
// #else
// #include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
// #endif

// #include "imgui/imgui.h"
// #include "imgui/imgui_impl_glfw.h"
// #include "imgui/imgui_impl_opengl3.h"

#include "shaders/shader.h"
#include "renderers/SkyboxRenderer.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Camera* cam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


GLFWwindow* openGLallInit();
void initImGui(GLFWwindow* window);
void cleanAll();

void renderLoop(GLFWwindow* window);

float WIDTH = 800;
float HEIGHT = 800;
const float SKYBOX_SIZE = 200.0f;


int main()
{
	//��ʼ������
	GLFWwindow* window = openGLallInit();
	if (window == NULL)
		return -1;

	//��պе�����
	std::vector<std::string> skyboxTextures = {
		"res/sky/right.jpg",
		"res/sky/left.jpg",
		"res/sky/top.jpg",
		"res/sky/down.jpg",
		"res/sky/front.jpg",
		"res/sky/back.jpg"
	};

	//������պ�
	SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);

	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 1.0f, 800.0f);
	while (!glfwWindowShouldClose(window))
	{

		processInput(window, &cam);
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ��Ȼ���ҲҪ���
		glEnable(GL_DEPTH_TEST);

		skybox.render(cam.GetViewMatrix(), projection);

		//��Ⱦ
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}



void processInput(GLFWwindow *window, Camera* cam)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam->moveForward(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam->moveBack(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam->moveLeft(0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam->moveRight(0.1);
	}

	double xpos = 0, ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	cam->rotate(xpos, ypos);
}


GLFWwindow* openGLallInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "firWindow", NULL, NULL);
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

	//�趨�ӿ�
	glViewport(0, 0, WIDTH, HEIGHT);

	//ע��ص�����,��������Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	return window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

// void initImGui(GLFWwindow* window)
// {
// 	// Setup Dear ImGui context
// 	IMGUI_CHECKVERSION();
// 	ImGui::CreateContext();
// 	ImGuiIO& io = ImGui::GetIO(); (void)io;

// 	// Setup Dear ImGui style
// 	ImGui::StyleColorsDark();
// 	const char* glsl_version = "#version 130";
// 	// Setup Platform/Renderer bindings
// 	ImGui_ImplGlfw_InitForOpenGL(window, true);
// 	ImGui_ImplOpenGL3_Init(glsl_version);
// }

// void cleanAll()
// {
// 	// Cleanup
// 	ImGui_ImplOpenGL3_Shutdown();
// 	ImGui_ImplGlfw_Shutdown();
// 	ImGui::DestroyContext();

// 	glfwTerminate();
// }

void renderLoop(GLFWwindow* window)
{
	
	
	
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	
	
}

