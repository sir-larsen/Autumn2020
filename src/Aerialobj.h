#pragma once
#include <GL/glew.h>
#include "model.h"

enum Direction2 {
	North2 = 0,
	South2 = 1,
	East2 = 2,
	West2 = 3
};

class Aerialobj
{
private:
	float  movementSpeed;
	float  rotationAngle;
	Model* object;
	float posX, posY, posZ;
	Direction2 direction2;
	bool up;
	int headingX;
	int headingZ;

public:
	Aerialobj(Model* model);
	void draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct);
	void setSpawn();
	void move(float dt);
	void Translate(Direction2 dir, float dt);
};