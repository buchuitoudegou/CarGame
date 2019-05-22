#include "headers.h"

using namespace std;
// -----------------------------
// window params
float SCR_WIDTH = 4000;
float SCR_HEIGHT = 4000;
const float SKYBOX_SIZE = 500.0f;
// ------------------------------
// plane params
float planeVertices[] = {
			-25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f, // Bottom-left
			25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f, // top-right
			25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f, // bottom-right         
			25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f,  // top-right
			-25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f,  // bottom-left
			-25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f,// top-left
			
			-25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f, // bottom-left
			25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f,  // bottom-right
			25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f,  // top-right
			25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f, // top-right
			-25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f,  // top-left
			-25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f,  // bottom-left
			
			-25.f, 0.5f, 25.f, -1.0f, 0.0f, 0.0f, // top-right
			-25.f, 0.5f, -25.f, -1.0f, 0.0f, 0.0f, // top-left
			-25.f, -0.5f, -25.f, -1.0f, 0.0f, 0.0f,  // bottom-left
			-25.f, -0.5f, -25.f, -1.0f, 0.0f, 0.0f, // bottom-left
			-25.f, -0.5f, 25.f, -1.0f, 0.0f, 0.0f,  // bottom-right
			-25.f, 0.5f, 25.f, -1.0f, 0.0f, 0.0f, // top-right
			
			25.f, 0.5f, 25.f, 1.0f, 0.0f, 0.0f, // top-left
			25.f, -0.5f, -25.f, 1.0f, 0.0f, 0.0f, // bottom-right
			25.f, 0.5f, -25.f, 1.0f, 0.0f, 0.0f, // top-right         
			25.f, -0.5f, -25.f, 1.0f, 0.0f, 0.0f,  // bottom-right
			25.f, 0.5f, 25.f, 1.0f, 0.0f, 0.0f,  // top-left
			25.f, -0.5f, 25.f, 1.0f, 0.0f, 0.0f, // bottom-left     
			
			-25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f, // top-right
			25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f, // top-left
			25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f,// bottom-left
			25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f, // bottom-left
			-25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f, // bottom-right
			-25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f, // top-right
			
			-25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f,// top-left
			25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f, // bottom-right
			25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f, // top-right     
			25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f, // bottom-right
			-25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f,// top-left
			-25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f // bottom-left 
};
unsigned int planeVAO;
// -------------------------------
// shadow params
unsigned int depthMapFBO;
float SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
// -------------------------------
// control params
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
// -------------------------------
// camera
Camera camera(glm::vec3(0.0f, 300.f, 7.0f));
// -------------------------------
// game objs
vector<Entity*> objs;
// -------------------------------
// renderer
EntityRenderer entityRenderer;
// ------------------------------
// projection
glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 800.0f);

GLFWwindow* openGLallInit();
void initImGui(GLFWwindow* window);

void initPlaneVAO() {
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void drawPlane(Shader* shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, -1, 0));
	shader->setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderScene(Shader* shader, vector<Entity*> objs, EntityRenderer* renderer) {
	drawPlane(shader);
	for (auto i = 0; i < objs.size(); ++i) {
		shader->setVec3("bcolor", glm::vec3(-1, -1, -1));
		renderer->render(shader, *objs[i], &RendererManager::headlight);
	}
}

void shadowMapping(Shader& simpleDepthShader, glm::mat4 lightSpaceMatrix) {
	//Shader simpleDepthShader("shaders/shadow_depth.vs", "shaders/shadow_depth.fs");
	simpleDepthShader.use();
	simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(&simpleDepthShader, objs, &entityRenderer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void initShadow() {
	// ---------------------------------------------
	// frame depth map
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &RendererManager::depthMap);
	glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
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

int main() {
	GLfloat curFrame = 0.0f, lastFrame = 0.0f;
	GLFWwindow* window = openGLallInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	if (window == NULL)
		return -1;
	
	std::vector<std::string> skyboxTextures = {
		"res/sky/right.jpg",
		"res/sky/left.jpg",
		"res/sky/top.jpg",
		"res/sky/down.jpg",
		"res/sky/front.jpg",
		"res/sky/back.jpg"
	};
	// init renderer
	RendererManager::init();
	SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);
	// init game object
	initPlaneVAO();
	initShadow();
	Car car = Car("res/car/newcar2/Avent.obj");

	// Entity terrain = Entity("./res/terrain/Terrain.obj");
	// terrain.position = glm::vec3(625, -20, 0);

	objs.push_back(&car);
	//objs.push_back(&terrain);
	// plane shader
	Shader simpleDepthShader("./src/shaders/shadow_depth.vs", "./src/shaders/shadow_depth.fs");
	Shader shader("./src/shaders/shadow_mapping.vs", "./src/shaders/shadow_mapping.fs");
	shader.use();
	shader.setInt("shadowMap", 30);
	
	while (!glfwWindowShouldClose(window)) {
		curFrame = glfwGetTime();
		// process config
		float near_plane = 1.0f, far_plane = 77.5f;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(
			glm::vec3(RendererManager::headlight.position), 
			glm::vec3(0), 
			glm::vec3(0, 1, 0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		// ----------------------------------
		// render shadow
		shadowMapping(simpleDepthShader, lightSpaceMatrix);
		
		// ----------------------------------
		// render sky box
		skybox.render(camera.getViewMat(), projection);
		// ----------------------------------
		// render scene
		glm::mat4 model = glm::mat4(1.0f);
		shader.use();
		shader.setBool("useInColor", true);
		shader.setVec3("bcolor", glm::vec3(1, 1, 1));
		shader.setMat4("model", model);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader.setMat4("projection", projection);
		shader.setMat4("view", camera.getViewMat());
		shader.setVec3("viewPos", camera.position);
		shader.setVec3("lightPos", glm::vec3(
			RendererManager::headlight.position.x,
			RendererManager::headlight.position.y,
			RendererManager::headlight.position.z));
		shader.setFloat("ambient", 1.0f);
		shader.setFloat("diffuse", 1.0f);
		shader.setFloat("specular", 0.45f);
		glActiveTexture(GL_TEXTURE30);
		glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
		renderScene(&shader, objs, &entityRenderer);

		move(curFrame - lastFrame);
		lastFrame = curFrame;
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void initImGui(GLFWwindow* window) {
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
