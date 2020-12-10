#pragma once
#include <GL/glew.h>
#include "Terrain.h"
#include "../Engine/model.h"
#include "Global.h"

struct Hbox { //Trees "hitbox" Calculating these to add position to the array map
	glm::vec3 lr;
	glm::vec3 ll;
	glm::vec3 ur;
	glm::vec3 ul;
};

class Trees
{
public:
	Trees(Model* m_Tree, Terrain* terrain);
	void Draw(Shader* shader, glm::mat4 projection, glm::mat4 view);
	int treeCount = 0;
private:
	std::vector<glm::vec3> points;
	std::vector<Hbox> boxes;
	Model* tree;
	unsigned int VAO, VBO;
	std::vector <glm::mat4> modelMatrices;

	void generateModelMatrices(Terrain* terrain);
	void addModelMatrices();
};