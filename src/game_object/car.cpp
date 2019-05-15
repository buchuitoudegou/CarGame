#include "car.h"

void Car::move(const float t) {
  position += speed * t * direction;
}

glm::mat4 Car::getModelMatrix() const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  return model;
}

Car::Car(const string& path) {
  position = glm::vec3(0, 0 ,0);
  speed = 0.0;
  direction = glm::vec3(0, 0, 1);
  modelPath = path;
  loader = new ModelLoader(path.c_str());
  isLoad = true;
}

Car::~Car() {
  delete loader;
}