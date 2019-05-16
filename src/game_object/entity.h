#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include "../model_loader/loader.h"
using namespace std;

class Entity {
public:
  Entity(const string& path);
  ~Entity();
  string modelPath;
  glm::vec3 position;
  glm::mat4 getModelMatrix() const;
  ModelLoader* loader = nullptr;
  bool isLoad = false;
};

#endif