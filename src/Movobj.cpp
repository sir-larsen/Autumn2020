#include "Movobj.h"

Movobj::Movobj(Terrain* m_Terrain, Model* model, int id)
{
	object = model;
	terrain = m_Terrain;
	ID = id;
	movementSpeed = 1.f;
	rotationAngle = 0.f;
}


void Movobj::draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt)
{
	shader->use();
	shader->setMat4("u_ProjectionMat", projection);
	shader->setMat4("u_ViewMat", view);
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(posX, posY, posZ));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.3f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1*-90.f), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 transformation = translation * rotation * scale;
	shader->setMat4("u_TransformationMat", transformation);
	
	//Move(dt)
	
	object->Draw(*shader);
}

void Movobj::setSpawn(float x, float z)
{
	posX = x; posZ = z;
	posY = terrain->gScale->getHeight((int)x, (int)z);
}

glm::vec3 Movobj::getPos()
{
	return glm::vec3(posX, posY, posZ);
}
