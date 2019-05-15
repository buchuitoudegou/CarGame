#include "EntityShader.h"

EntityShader::EntityShader(string vpath, string fpath): Shader(vpath.c_str(), fpath.c_str()) {
  
}

void EntityShader::bindUniformLocations(){
  // Setup named attributes
  glBindAttribLocation(ID, 0, "a_vertex");
  glBindAttribLocation(ID, 1, "a_normal");
  glBindAttribLocation(ID, 2, "a_tex_coord");

  location_texMap = glGetUniformLocation(ID, "texMap");
  location_cubeMap = glGetUniformLocation(ID, "cubeMap");
  location_shadowMap = glGetUniformLocation(ID, "shadowMap");
  location_clip_plane = glGetUniformLocation(ID, "clip_plane");

  location_projection = glGetUniformLocation(ID, "projection");
  location_model = glGetUniformLocation(ID, "model");
  location_view = glGetUniformLocation(ID, "view");
  location_inv_view = glGetUniformLocation(ID, "inv_view");

  location_shininess = glGetUniformLocation(ID, "shininess");
  location_emission = glGetUniformLocation(ID, "emission");
  location_num_lights = glGetUniformLocation(ID, "num_lights");
  location_mtl_ambient = glGetUniformLocation(ID, "mtl_ambient");
  location_mtl_diffuse = glGetUniformLocation(ID, "mtl_diffuse");
  location_mtl_specular = glGetUniformLocation(ID, "mtl_specular");

  location_depth_pv = glGetUniformLocation(ID, "depth_pv");
  location_render_shadows = glGetUniformLocation(ID, "render_shadows");
}

void EntityShader::loadLights(std::vector<Light*> lights){
  loadUniformValue(location_num_lights, int(lights.size()));
  for(size_t i = 0; i < lights.size(); i++){
    loadLight(lights[i], i);
  }
}

void EntityShader::loadLight(Light* light, int i){
  loadLightUniform("position", i, light->position);
  loadLightUniform("specular", i, light->specular);
  loadLightUniform("diffuse", i, light->diffuse);
  loadLightUniform("ambient", i, light->ambient);
  loadLightUniform("radius", i, light->radius);
  loadLightUniform("coneAngle", i, light->coneAngle);
  loadLightUniform("coneDirection", i, light->coneDirection);
}

void EntityShader::loadView(glm::mat4 view){
  loadUniformValue(location_view, view);
  loadUniformValue(location_inv_view, glm::inverse(view));
}

void EntityShader::loadEntity(Car* entity){
  loadUniformValue(location_texMap, 0);
  loadUniformValue(location_cubeMap, 1);
  glm::mat4 model = entity->getModelMatrix();

  loadUniformValue(location_model, model);
}

void EntityShader::loadModelComponent(const ModelComponent& component){
  loadUniformValue(location_mtl_ambient, component.getMaterial().ambient, 3);
  loadUniformValue(location_mtl_diffuse, component.getMaterial().diffuse, 3);
  loadUniformValue(location_mtl_specular, component.getMaterial().specular, 3);
  loadUniformValue(location_emission, component.getMaterial().emission, 3);
  loadUniformValue(location_shininess, component.getMaterial().shininess);
}

void EntityShader::loadProjection(glm::mat4 proj){
  loadUniformValue(location_projection, proj);
  loadUniformValue(location_render_shadows, 0);
}
void EntityShader::loadClipPlane(glm::vec4 clip){
  loadUniformValue(location_clip_plane, clip);
}

void EntityShader::loadDepth(glm::mat4 pv){
  loadUniformValue(location_shadowMap, 2);
  loadUniformValue(location_depth_pv, pv);
  loadUniformValue(location_render_shadows, 1);
}
