#ifndef CAR_H
#define CAR_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include "../model_loader/loader.h"
using namespace std;

class Car {
public:
  Car(const string& path);
  ~Car();
  string modelPath;
  glm::vec3 position;
  float speed;
  glm::vec3 direction; // unit vector
  void move(const float t);
  glm::mat4 getModelMatrix() const;
  ModelLoader* loader = nullptr;
  bool isLoad = false;
};

#endif