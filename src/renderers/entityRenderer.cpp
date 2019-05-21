#include "entityRenderer.h"

const string EntityRenderer::ENTITY_VERTEX_SHADER_PATH = "./src/shaders/carshader.vs";
const string EntityRenderer::ENTITY_FRAGMENT_SHADER_PATH = "./src/shaders/carshader.fs";

EntityRenderer::EntityRenderer() {}

void EntityRenderer::render(Shader* shader, Entity& entity, Light* light) {
  if (shader != nullptr && entity.isLoad) {
    shader->use();
    // shader->setMat4("projection", projection);
    // shader->setMat4("view", view);
    shader->setVec3("lightPos", light->position);
    shader->setMat4("model", entity.getModelMatrix());
    shader->setFloat("ambient", 1.0f);
		shader->setFloat("diffuse", 1.0f);
		shader->setFloat("specular", 0.5f);
    shader->setBool("useInColor", false);
    // render shadow
    // glActiveTexture(GL_TEXTURE0 + 100);
		// glBindTexture(GL_TEXTURE_2D, RendererManager::depthMap);
    entity.loader->draw(*shader);
  }
}

EntityRenderer::~EntityRenderer() {}