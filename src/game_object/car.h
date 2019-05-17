#ifndef CAR_H
#define CAR_H

#include "entity.h"

using namespace std;

class Car : public Entity {
public:
  Car(const string& path);
  float speed;
  glm::vec3 direction; // unit vector
  void move(const float t);
};

#endif