#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void moveForward(float const distance);
	void moveBack(float const distance);
	void moveRight(float const distance);
	void moveLeft(float const distance);
	glm::mat4 GetViewMatrix();
	void rotate(float xpos, float ypos);

	glm::vec3 getPosition();

private:
	float pitch = 0.f, yaw = -90.f;
	bool first = true;
	float prexPos = 500, preyPos = 500;
	float SENSITIVITY = 0.1f;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float speed = 0.05;

	void updates();
};

#endif // !CAMERA_H


