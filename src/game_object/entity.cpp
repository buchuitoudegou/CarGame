#include "Entity.h"

glm::mat4 Entity::getModelMatrix() const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  return model;
}

Entity::Entity(const string& path) {
  position = glm::vec3(0, 0 ,0);
  modelPath = path;
  loader = new ModelLoader(path.c_str());
  isLoad = true;
}

Entity::~Entity() {
  delete loader;
}