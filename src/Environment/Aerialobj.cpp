#include "Aerialobj.h"

Aerialobj::Aerialobj(Model* model)
{
	object = model;
	movementSpeed = 21.f;
	rotationAngle = 0.f;
	direction2 = North2;
	up = true;
	setSpawn();
	headingX = 1;
	headingZ = 1;
}

void Aerialobj::draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct)
{
	
	
	/*if ((int)ct % 8 == 0) {
		int dir = rand() % 4;
		if (dir == 0)
			direction2 = North2;
		else if (dir == 1)
			direction2 = South2;
		else if (dir == 2)
			direction2 = East2;
		else if (dir == 3)
			direction2 = West2;
	}*/
	if ((int)ct % 6 == 0)
		up = !up;

	shader->use();
	shader->setMat4("u_ProjectionMat", projection);
	shader->setMat4("u_ViewMat", view);
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(posX, posY, posZ));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.1f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(rotationAngle), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 transformation = translation * rotation * scale;
	shader->setMat4("u_TransformationMat", transformation);
	//m_Camera->setObjPos(glm::vec3(posX, posY, posZ));

	move(dt);

	object->Draw(*shader);
}

void Aerialobj::setSpawn()
{
	posX = 100.f; posZ = 120.f; posY = 118.f;

}

void Aerialobj::move(float dt)
{
	float velocity = movementSpeed * dt;
	if (posX > 1070)
		headingX = -1;
	if (posX < 0)
		headingX = 1;
	if (posZ > 1070)
		headingZ = -1;
	if (posZ < 0)
		headingZ = 1;

	posX += velocity * headingX;
	posZ += velocity * headingZ;

	if (up)
		posY += velocity / 9;
	else
		posY -= velocity / 9;

	if (headingX == 1 && headingZ == 1)
		rotationAngle = 45.f;
	if (headingX == -1 && headingZ == -1)
		rotationAngle = 45.f - 180.f;
	
	/*if (posZ > 1070) {
		direction2 = West2;
		posZ = 1050;
	}
	else if (posZ < 20) {
		direction2 = East2;
		posZ = 40;
	}
	
	switch (direction2)
	{
	case North2: Translate(North2, dt); break;
	case South2: Translate(South2, dt); break;
	case East2:  Translate(East2, dt); break;
	case West2:  Translate(West2, dt); break;
	default:    break;
	}*/
}

void Aerialobj::Translate(Direction2 dir, float dt)
{
	float velocity = movementSpeed * dt;
	/*switch (dir)
	{
	case South2: {
		posX -= velocity; rotationAngle = 180.f;
		if (up)
			posY += velocity / 6;
		else
			posY -= velocity / 6;
		break;
	}
	case West2: {
		posZ -= velocity; rotationAngle = 90.f;
		if (up)
			posY += velocity / 6;
		else
			posY -= velocity / 6;
		break;
	}
	case North2: {
		posX += velocity; rotationAngle = 0.f;
		if (up)
			posY += velocity / 6;
		else
			posY -= velocity / 6;
		break;
	}
	case East2: {
		posZ += velocity; rotationAngle = 270.f;
		if (up)
			posY += velocity / 5;
		else
			posY -= velocity / 5;
		break;
	}
	default:    break;
	}*/
}