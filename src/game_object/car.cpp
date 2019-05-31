#include "car.h"

void Car::move(const float t) {
  position += speed * t * direction;
}

Car::Car(const string& path) {
  speed = 0.0;
  direction = glm::vec3(0, 0, 1);
  loader = new ModelLoader(path.c_str());
  position = glm::vec3(0, -0.5, 0);
}

Car::~Car() {
  delete loader;
}

void Car::draw(Shader* shader) const {
  loader->draw(*shader);
}
