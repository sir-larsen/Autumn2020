#pragma once
#include <GL/glew.h>
#include <stdlib.h>
#include "Terrain.h"
#include "../Engine/model.h"
#include "Global.h"
#include "../Engine/Camera.h"


enum Direction {
	North = 0,
	South = 1,
	East = 2,
	West = 3
};

class Movobj
{
private:
	bool cam;
	int ID;
	float  movementSpeed;
	float  rotationAngle;
	//float posX, posY, posZ;

	Model* object;
	Terrain* terrain;
	Camera* m_Camera;
public:
	float posX, posY, posZ;
	bool firstDirection, beenAdjusted;
	Direction direction;
	Movobj(Terrain* m_Terrain, Model* model, int id, Camera* camera);
	void draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct);
	void setSpawn();
	glm::vec3 getPos();
	void print();
	void move(float dt);
	void Translate(Direction dir, float dt);
};