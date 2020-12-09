#pragma once
#include <GL/glew.h>
#include "Terrain.h"
#include "model.h"
#include "terrain.h"
#include "Global.h"

class Movobj
{
private:
	int ID;
	float  movementSpeed;
	float  rotationAngle;
	float posX, posY, posZ;

	Model*   object;
	Terrain* terrain;
public:
	Movobj(Terrain* m_Terrain, Model* model, int id);
	void draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt);
	void setSpawn(float x, float z);
	glm::vec3 getPos();
	void print();
	//Movement
	//Translation
};