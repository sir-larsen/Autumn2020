#include "Movobj.h"
int gMap[1200][1200]; //Generated "map" for checking trees for collision

Movobj::Movobj(Terrain* m_Terrain, Model* model, int id, Camera* camera)
{
	object = model;
	terrain = m_Terrain;
	ID = id;
	movementSpeed = 9.f;
	rotationAngle = 0.f;
	direction = North;
	if (ID == 0) cam = true;
	m_Camera = camera;
}


void Movobj::draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, float ct)
{
	if ((int)ct % 1 == 0)
		posY = terrain->gScale->getHeight(round(posX), round(posZ));

	shader->use();
	shader->setMat4("u_ProjectionMat", projection);
	shader->setMat4("u_ViewMat", view);
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(posX, posY, posZ));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.10f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * -90.f), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 rotation2 = glm::rotate(glm::mat4(1), glm::radians(rotationAngle), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 transformation = translation * rotation2 * rotation * scale;
	shader->setMat4("u_TransformationMat", transformation);
	if (cam) m_Camera->setObjPos(glm::vec3((float)posX, (float)posY, (float)posZ));
	//m_Camera->setObjPos(glm::vec3(posX, posY, posZ));

	move(dt);

	object->Draw(*shader);
}

void Movobj::print() {
	for (int x = 20; x < 33; x++) {
		for (int z = 190; z < 290; z++) {
			std::cout << gMap[x][z] << " ";
		}
		std::cout << "\n";
	}
}

void Movobj::setSpawn()
{
	int x = rand() % 230 + 20;
	int z = rand() % 230 + 20;
	int d = rand() % 4;
	posX = x; posZ = z;
	posY = terrain->gScale->getHeight((int)x, (int)z);
	if (d == 0)
		direction = North;
	else if (d == 1)
		direction = South;
	else if (d == 2)
		direction = East;
	else if (d == 3)
		direction = West;
}

glm::vec3 Movobj::getPos()
{
	return glm::vec3(posX, posY, posZ);
}

void Movobj::move(float dt)
{
	int gridPosX = round(posX);
	int gridPosZ = round(posZ);

	bool collision = false;
	for (int i = 0; i < 3; i++)
		if (gMap[gridPosX + i][gridPosZ] == 1 || posX > 290) {
			if (posX > 290)
				posX = 278;
			direction = South;
			//posX -= 5;
			collision = true;
		}
	if (!collision) {
		for (int i = 0; i < 3; i++)
			if (gMap[gridPosX - i][gridPosZ] == 1 || posX < 5) {
				if (posX < 5)
					posX = 12;
				direction = North;
				collision = true;
			}
	}
	if (!collision) {
		for (int i = 0; i < 3; i++)
			if (gMap[gridPosX][gridPosZ + i] == 1 || posZ > 290) {
				if (posZ > 290)
					posZ = 278;
				direction = West;
				//posZ -= 5;
				collision = true;
			}
	}
	if (!collision) {
		for (int i = 0; i < 3; i++)
			if (gMap[gridPosX][gridPosZ - i] == 1 || posZ < 5) {
				if (posZ < 5)
					posZ = 12;
				direction = East;
				//posZ += 5;
				collision = true;
			}
	}

	switch (direction)
	{
	case North: Translate(North, dt); break;
	case South: Translate(South, dt); break;
	case East:  Translate(East, dt); break;
	case West:  Translate(West, dt); break;
	default:    break;
	}

}

void Movobj::Translate(Direction dir, float dt)
{
	float velocity = movementSpeed * dt;
	switch (dir)
	{
	case South: {
		posX -= velocity; posZ = floor(posZ); rotationAngle = 180.f;
		break;
	}
	case West: {
		posZ -= velocity; posX = floor(posX); rotationAngle = 90.f;
		break;
	}
	case North: {
		posX += velocity; posZ = floor(posZ); rotationAngle = 0.f;
		break;
	}
	case East: {
		posZ += velocity; posX = floor(posX); rotationAngle = 270.f;
		break;
	}
	default:    break;
	}
}
