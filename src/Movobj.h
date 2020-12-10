#pragma once
#include <GL/glew.h>
#include "Terrain.h"
#include "model.h"
#include "terrain.h"
#include "Global.h"
#include "Camera.h"

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
	float posX, posY, posZ;

	Model*   object;
	Terrain* terrain;
	Camera* m_Camera;
public:
	bool firstDirection, beenAdjusted;
	Direction direction;
	Movobj(Terrain* m_Terrain, Model* model, int id, Camera* camera);
	void draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct);
	void setSpawn(float x, float z);
	glm::vec3 getPos();
	void print();
	void move(float dt);
	void Translate(Direction dir, float dt);
};