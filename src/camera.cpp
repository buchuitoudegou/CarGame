#include "camera.h"
Camera::Camera(glm::vec3 position, glm::vec3 up)
{
	Position = position;
	WorldUp = up;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	updates();
}

void Camera::moveForward(float const distance)
{

	Position += Front * distance;
}
void Camera::moveBack(float const distance)
{
	Position -= Front * distance;
}
void Camera::moveRight(float const distance)
{
	Position += Right * distance;
}
void Camera::moveLeft(float const distance)
{
	Position -= Right * distance;
}
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, (Position + Front), Up);
}
void Camera::rotate(float xpos, float ypos)
{
	float xoffset = xpos - prexPos;
	float yoffset = preyPos - ypos;
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	prexPos = xpos;
	preyPos = ypos;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;



	updates();
}

glm::vec3 Camera::getPosition()
{
	return Position;
}

void Camera::updates()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}