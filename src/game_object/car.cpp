#include "car.h"

void Car::move(const float t) {
  position += speed * t * direction;
}

Car::Car(const string& path): Entity(path) {
  speed = 0.0;
  direction = glm::vec3(0, 0, 1);
}