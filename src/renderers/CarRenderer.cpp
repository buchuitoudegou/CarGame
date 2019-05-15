#include "CarRenderer.h"

const string CarRenderer::CAR_VERTEX_SHADER_PATH = "./src/shaders/carshader.vs";
const string CarRenderer::CAR_FRAGMENT_SHADER_PATH = "./src/shaders/carshader.fs";

CarRenderer::CarRenderer() {
  shader = new Shader(CAR_VERTEX_SHADER_PATH.c_str(), CAR_FRAGMENT_SHADER_PATH.c_str());
}

void CarRenderer::render(Car& car, Light* light, glm::mat4 view, glm::mat4 projection) {
  if (shader != nullptr && car.isLoad) {
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setVec3("lightPos", light->position);
    shader->setMat4("model", car.getModelMatrix());
    shader->setFloat("ambient", 1.0f);
		shader->setFloat("diffuse", 1.0f);
		shader->setFloat("specular", 0.5f);
    car.loader->draw(*shader);
  }
}

CarRenderer::~CarRenderer() {
  delete shader;
}