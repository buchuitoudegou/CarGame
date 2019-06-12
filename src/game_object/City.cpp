#include "City.h"



City::City(const string& path)
{
	loader = new ModelLoader(path.c_str());
	position = glm::vec3(0, -55.5, 0);
}


City::~City()
{
	delete loader;
}

void City::draw(Shader* shader) const {
	loader->draw(*shader);
}


glm::mat4 City::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	model = glm::translate(model, position);
	
	return model;
}
