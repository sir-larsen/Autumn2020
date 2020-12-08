#pragma once
#include <GL/glew.h>
#include "Terrain.h"
#include "model.h"

class Trees
{
public:
	Trees(Model* m_Tree, Terrain* terrain);

	void Draw(Shader* shader, glm::mat4 projection, glm::mat4 view);
	//void eatPellet(Camera* camera, Maze3D* maze);
	//bool allEaten;
	//int pelletCount;
	int treeCount = 0;
private:
	Model* tree;
	unsigned int VAO, VBO;
	std::vector <glm::mat4> modelMatrices;

	void generateModelMatrices(Terrain* terrain);
	void addModelMatrices();
};