#ifndef ENTITY_SHADER_H
#define ENTITY_SHADER_H

#include "../headers.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glad/glad.h>

#include "../game_object/light.h"
#include "../game_object/car.h"
#include "../model/Model.h"
#include <string>
#include <vector>

using namespace std;

class EntityShader : public Shader {
private:
  GLuint location_texMap;
  GLuint location_cubeMap;
  GLuint location_shadowMap;
  GLuint location_clip_plane;

  GLuint location_projection;
  GLuint location_model;
  GLuint location_view;
  GLuint location_inv_view;

  GLuint location_num_lights;
  GLuint location_shininess;
  GLuint location_emission;

  GLuint location_mtl_ambient;
  GLuint location_mtl_diffuse;
  GLuint location_mtl_specular;

  GLuint location_depth_pv;
  GLuint location_render_shadows;
public:
  EntityShader(string, string);

  virtual void bindUniformLocations();

  void loadLights(std::vector<Light*> lights);
  void loadLight(Light* light, int i);
  void loadView(glm::mat4 view);
  void loadEntity(Car* car);
  void loadModelComponent(const ModelComponent& component);
  void loadProjection(glm::mat4 proj);
  void loadDepth(glm::mat4 pv);
  void loadClipPlane(glm::vec4 clip);
};

#endif