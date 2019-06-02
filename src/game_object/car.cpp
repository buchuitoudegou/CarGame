#include "car.h"

const float Car::turnAngle = 0.5f;

void Car::move(const float t) {
  glm::vec3 rot = glm::vec3(-cos(glm::radians(angle)), 0, sin(glm::radians(angle)));
  direction = rot;
  originAngle = angle;
  position += speed * t * direction;
}

Car::Car(const string& path) {
  speed = 9.0f;
  direction = glm::vec3(-1, 0, 0);
  angle = 0.0;
  originAngle = 0.0f;
  loader = new ModelLoader(path.c_str());
  position = glm::vec3(0, -0.5, 0);
}

Car::~Car() {
  delete loader;
}

void Car::draw(Shader* shader) const {
  loader->draw(*shader);
}

void Car::rotate(const float offset) {
  angle = (angle > 360) ? angle - 360 : angle;
  angle = (angle < -360) ? angle + 360 : angle;
  if (abs(angle - originAngle) <= 22.5 || abs(angle + offset - originAngle) <= 22.5) {
    angle += offset;
  }
}

glm::mat4 Car::getModelMat() const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(originAngle), glm::vec3(0, 1, 0));
  return model;
}