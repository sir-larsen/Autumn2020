#include "Aerialobj.h"

Aerialobj::Aerialobj(Model* model)
{
	object = model;
	movementSpeed = 15.f;
	rotationAngle = 0.f;
	direction2 = North2;
	up = true;
	setSpawn();
	headingX = 1;
	headingZ = 1;
}

void Aerialobj::draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct)
{
	if ((int)ct % 8 == 0)
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
	posX = 100.f; posZ = 120.f; posY = 195.f;

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
		posY += velocity / 12;
	else
		posY -= velocity / 20;

	if (headingX == 1 && headingZ == 1)
		rotationAngle = 45.f;
	if (headingX == -1 && headingZ == -1)
		rotationAngle = 45.f - 180.f;
	
}
