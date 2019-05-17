#include "entityRenderer.h"

const string EntityRenderer::ENTITY_VERTEX_SHADER_PATH = "./src/shaders/carshader.vs";
const string EntityRenderer::ENTITY_FRAGMENT_SHADER_PATH = "./src/shaders/carshader.fs";

EntityRenderer::EntityRenderer() {
  shader = new Shader(ENTITY_VERTEX_SHADER_PATH.c_str(), ENTITY_FRAGMENT_SHADER_PATH.c_str());
}

void EntityRenderer::render(Entity& entity, Light* light, glm::mat4 view, glm::mat4 projection) {
  if (shader != nullptr && entity.isLoad) {
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setVec3("lightPos", light->position);
    shader->setMat4("model", entity.getModelMatrix());
    shader->setFloat("ambient", 1.0f);
		shader->setFloat("diffuse", 1.0f);
		shader->setFloat("specular", 0.5f);
    entity.loader->draw(*shader);
  }
}

EntityRenderer::~EntityRenderer() {
  delete shader;
}