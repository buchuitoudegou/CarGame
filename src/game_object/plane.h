#ifndef PLANE_H
#define PLANE_H

#include "./entity.h"
#include "../stb_image/stb_image.h"
#include "../renderers/RendererManager.h"

class Plane: public Entity {
public:
  float planeVertices[48] = {
		// positions            // normals         // texcoords
    25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

    25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
    25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
  };
  unsigned int planeVAO, texture;
  void initPlaneVAO();
  Plane();
  void draw(Shader* shader = nullptr) const;
  glm::mat4 getModelMat() const;
  void initTexture();
};

#endif