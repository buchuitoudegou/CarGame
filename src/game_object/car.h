#ifndef CAR_H
#define CAR_H

#include "entity.h"
#include "../model_loader/loader.h"

using namespace std;

class Car : public Entity {
public:
  Car(const string& path);
  ~Car();
  float speed;
  glm::vec3 direction; // unit vector
  ModelLoader* loader = nullptr;
  void move(const float t);
  void draw(Shader* shader = nullptr) const;
};

#endif