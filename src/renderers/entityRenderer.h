#ifndef CAR_RENDERER_H
#define CAR_RENDERER_H

#include <string>
#include "../headers.h"
#include "../game_object/entity.h"
#include "../shaders/shader.h"
#include "../game_object/light.h"
#include "../skyloader/loader.h"

using namespace std;

class EntityRenderer {
public:
  static const string ENTITY_VERTEX_SHADER_PATH;
  static const string ENTITY_FRAGMENT_SHADER_PATH;
  EntityRenderer();
  Shader* shader;
  void render(Entity& entity, Light* light, glm::mat4 view, glm::mat4 projection);
  ~EntityRenderer();
};

#endif