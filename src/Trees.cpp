#include "Trees.h"
#include <iostream>

//int gMap[1200][1200];

Trees::Trees(Model* m_Tree, Terrain* terrain)
{
	tree = m_Tree;
	generateModelMatrices(terrain);
	addModelMatrices();
	//map.resize(1120, std::vector<int>(1120));
	
}

void Trees::generateModelMatrices(Terrain* terrain)
{	
	//200 som mod-verdi
	for (int i = 0; i < terrain->gVertices.size()-800; i++) {
		if (i % 10000 == 0) {
			//glm::mat4 translation; 
			
			if (terrain->gVertices[i].location.x < 1040 && terrain->gVertices[i].location.x > 35 &&
				terrain->gVertices[i].location.z < 1040 && terrain->gVertices[i].location.z > 35) {
				

				points.push_back(terrain->gVertices[i].location); //Get all points for all trees in a vector
				Hbox box;
				box.lr = terrain->gVertices[i].location + glm::vec3(-30.f, 0.f, -10.f);
				box.ll = terrain->gVertices[i].location + glm::vec3(-10.f, 0.f, -10.f);
				box.ur = terrain->gVertices[i].location + glm::vec3(-30.f, 0.f,  10.f);
				box.ul = terrain->gVertices[i].location + glm::vec3(-10.f, 0.f,  10.f);
				boxes.push_back(box);

				/*box.lr = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f, -7.f);
				box.ll = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f, -7.f);
				box.ur = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f,  7.f);
				box.ul = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f,  7.f);
				boxes.push_back(box);
				std::cout << "ul: " << box.ul.x << "," << box.ul.y << "," << box.ul.z << "                   " << "ur: " << box.ur.x << "," << box.ur.y << "," << box.ur.z << "\n\n";*/


				glm::mat4 translation = glm::translate(glm::mat4(1), terrain->gVertices[i].location - glm::vec3(0.f, 0.8f, 0.0f)); //Pinetree2
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * -90.f), glm::vec3(1.f, 0.f, 0.f));   //For pineTree2
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.060f, 0.065f, 0.085f)); //Pinetree2*/
				/*glm::mat4 translation = glm::translate(glm::mat4(1), terrain->gVertices[i].location + glm::vec3(0.f, 30.0f, 0.0f));
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * 180.f), glm::vec3(1.f, 0.f, 0.f));
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(3.0f, 3.0f, 3.0f));*/
				glm::mat4 transformation = translation * rotation * scale;
				modelMatrices.push_back(transformation);
				treeCount++;
			}
		}
	}

	//x = static_cast<int>(x);
	//z = static_cast<int>(z);

	for (int i = 0; i < boxes.size(); i++) {
		int xBeg = static_cast<int>(boxes[i].ur.x);
		int xEnd = static_cast<int>(boxes[i].ul.x);

		int zBeg = static_cast<int>(boxes[i].lr.z);
		int zEnd = static_cast<int>(boxes[i].ur.z);

		for (int j = xBeg; j <= xEnd; j++) {
			for (int k = zBeg; k <= zEnd; k++) {
				gMap[j][k] = 1;
			}
		}
	}
	/*for (int x = 20; x < 33; x++) {
		for (int z = 190; z < 290; z++) {
			std::cout << gMap[x][z] << " ";
		}
		std::cout << "\n";
	}*/

	std::cout << "Treecount TREES.CPP: " << treeCount << std::endl;
}

void Trees::addModelMatrices()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, treeCount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < tree->meshes.size(); i++) {
		VAO = tree->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void Trees::Draw(Shader* shader, glm::mat4 projection, glm::mat4 view)
{
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tree->textures_loaded[0].id);
	for (unsigned int i = 0; i < tree->meshes.size(); i++) {
		glBindVertexArray(tree->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, tree->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, treeCount);
		glBindVertexArray(0);
	}
}