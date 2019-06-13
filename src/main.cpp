#include "headers.h"

using namespace std;
// -----------------------------
// window params
float SCR_WIDTH = 4000;
float SCR_HEIGHT = 4000;
const float SKYBOX_SIZE = 500.0f;
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
float preAngle = 0;
float relativeDirection = 10;
float relativeHeight = 3;
Camera camera(glm::vec3(relativeDirection, -0.5 + relativeHeight, 0));
// -------------------------------
// game objs
vector<Entity*> objs;
vector<Shader*> shaders;
// ------------------------------
// projection
glm::mat4 lightSpaceMatrix;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 1.0f, 10000.0f);

GLFWwindow* openGLallInit();
void initImGui(GLFWwindow* window);
void renderImgui(bool menu);
void renderScene(Shader* shader = nullptr);
void shadowMapping(Shader& simpleDepthShader, glm::mat4 lightSpaceMatrix);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void initShadow();
void mouseCallback(GLFWwindow*, double xpos, double ypos);
void move(GLfloat dtime, Car& car);

float deltaTime;

int main() {
	GLfloat curFrame = 0.0f, lastFrame = 0.0f;
	camera.yaw = 190;
	camera.pitch = 5.250;
	
	camera.updateCamera();
	GLFWwindow* window = openGLallInit();
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	initImGui(window);
	if (window == NULL)
		return -1;
	
	 std::vector<std::string> skyboxTextures = {
	 	"res/sky/sky_rt.jpg",
	 	"res/sky/sky_lf.jpg",
	 	"res/sky/sky_up.jpg",
	 	"res/sky/sky_dn.jpg",
	 	"res/sky/sky_bk.jpg",
	 	"res/sky/sky_ft.jpg"
	 };
	// // init renderer
	RendererManager::init();
	// SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);
	// init shadow
	initShadow();	
	float near_plane = 1.0f, far_plane = 10000.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(
		glm::vec3(RendererManager::headlight.position), 
		glm::vec3(0), 
		glm::vec3(0, 1, 0));
	lightSpaceMatrix = lightProjection * lightView;
	// ----------------------------------
	// init game object
	Plane plane;
	City city = City("res/ApocalypticCity/Apocalyptic City.obj");
	// Car car = Car("res/car/newcar2/Avent.obj");
	Car car = Car("res/car/Car-Model/Car.obj");

	// objs.push_back(&plane);
	objs.push_back(&city);
	objs.push_back(&car);
	
	// ----------------------------------
	// shader
	Shader shadowShader("./src/shaders/glsl/shadow_depth.vs", "./src/shaders/glsl/shadow_depth.fs");
	Shader carShader("./src/shaders/glsl/shadow_mapping.vs", "./src/shaders/glsl/shadow_mapping.fs");
	Shader cityShader("./src/shaders/glsl/1.model_loading.vs", "./src/shaders/glsl/1.model_loading.fs");
	// Shader planeShader("./src/shaders/glsl/shadow_mapping.vs", "./src/shaders/glsl/shadow_mapping.fs");
	carShader.setInt("texture_diffuse_1", 0);
	carShader.setInt("shadowMap", 1);
	cityShader.setInt("shadowMap", 1);
	// planeShader.setInt("texture_diffuse_1", 0);
	// planeShader.setInt("shadowMap", 1);

	// shaders.push_back(&planeShader);
	shaders.push_back(&cityShader);
	shaders.push_back(&carShader);

	while (!glfwWindowShouldClose(window)) {
		camera.updateCamera();
		curFrame = glfwGetTime();
		// ----------------------------------
		// process config
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		// ----------------------------------
		// render shadow
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plane.texture);
		shadowMapping(shadowShader, lightSpaceMatrix);
		// ----------------------------------
		// render sky box
		// skybox.render(camera.getViewMat(), projection);
		// ----------------------------------
		// render scene
		renderScene();
		// ----------------------------------
		// render imgui
		renderImgui(true);

		move(curFrame - lastFrame, car);
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void renderScene(Shader* shader) {
	for (int i = 0; i < objs.size(); ++i) {
		auto currentShader = shader == nullptr ? shaders[i] : shader;
		auto model = objs[i]->getModelMat();
		EntityRenderer::render(
			currentShader,
			objs[i],
			objs[i]->useVertColor,
			projection,
			camera.getViewMat(),
			model,
			lightSpaceMatrix,
			RendererManager::headlight.position,
			camera.position,
			objs[i]->vertColor
		);
	}
}

void shadowMapping(Shader& simpleDepthShader, glm::mat4 lightSpaceMatrix) {
	//Shader simpleDepthShader("shaders/shadow_depth.vs", "shaders/shadow_depth.fs");
	simpleDepthShader.use();
	simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(&simpleDepthShader);
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
	//camera.mouseMoveHandler(-xoffset, -yoffset);
	xPos = xpos;
	yPos = ypos;
}


void move(GLfloat dtime, Car& car) {
	if (keys[GLFW_KEY_W]) {
		car.speedup(dtime);
	}
	else if (keys[GLFW_KEY_S]) {
		car.speeddown(dtime);
	}
	else {
		car.friction(dtime);
	}
	if (car.speed != 0)
	{
		car.move(dtime);
		glm::vec3 relativePosition = glm::vec3(-relativeDirection * car.direction.x, relativeHeight, -relativeDirection * car.direction.z);
		camera.position = car.position + relativePosition;
		camera.yaw -= car.angle - preAngle;
		preAngle = car.angle;
		camera.updateCamera();
	}
	

	//cout << car.speed << endl;

	
	if (keys[GLFW_KEY_A]) {
		car.rotate(Car::turnAngle);
	}
	if (keys[GLFW_KEY_D]) {
		car.rotate(-Car::turnAngle);
	}
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
	const char* glsl_version = "#version 330";
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
	glEnable(GL_TEXTURE_2D);  
	// glfwSetCursorPosCallback(window, mouseCallback);
	// glfwSetKeyCallback(window, keyCallback);
	return window;
}


void renderImgui(bool menu) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Menu", &menu, ImGuiWindowFlags_MenuBar);
	ImGui::SliderFloat("yaw", &camera.yaw, -360, 360);
	ImGui::SliderFloat("pitch", &camera.pitch, -89, 89);
	ImGui::SliderFloat("c x", &camera.position.x, -10, 100);
	ImGui::SliderFloat("c y", &camera.position.y, -10, 100);
	ImGui::SliderFloat("c z", &camera.position.z, -10, 100);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}