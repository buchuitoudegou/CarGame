#ifndef CAR_RENDERER_H
#define CAR_RENDERER_H

#include <string>
#include "../headers.h"
#include "../game_object/car.h"
#include "../shaders/EntityShader.h"
#include "../game_object/light.h"
#include "../skyloader/loader.h"

using namespace std;

class CarRenderer {
public:
  static const string CAR_VERTEX_SHADER_PATH;
  static const string CAR_FRAGMENT_SHADER_PATH;
  CarRenderer();
  Shader* shader;
  void render(Car& car, Light* light, glm::mat4 view, glm::mat4 projection);
  ~CarRenderer();
};

#endif