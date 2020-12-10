/**
 * @file Trees.cpp
 * @brief The source file for the Trees class
 *
 */
#include "Trees.h"
#include <iostream>

/**
 * @brief Construct a new Trees object and generates the necessary data
 *
 */
Trees::Trees(Model* m_Tree, Terrain* terrain)
{
	tree = m_Tree;
	generateModelMatrices(terrain);
	addModelMatrices();
}

/**
 * @brief Generates the model matrices and also fills the 2d array map in Global.h with
 *		  the "hitbox" data for the map
 *
 */
void Trees::generateModelMatrices(Terrain* terrain)
{	
	//200 som mod-verdi
	for (int i = 0; i < terrain->gVertices.size()-800; i++) {
		if (i % 10000 == 0) { //Mod value for managing amount of trees
			//glm::mat4 translation; 
			
			if (terrain->gVertices[i].location.x < 1040 && terrain->gVertices[i].location.x > 35 &&
				terrain->gVertices[i].location.z < 1040 && terrain->gVertices[i].location.z > 35) {
				

				points.push_back(terrain->gVertices[i].location); //Get all points for all trees in a vector
				Hbox box;
				box.lr = terrain->gVertices[i].location + glm::vec3(-20.f, 0.f, -17.f);  //Generating the positions for the tree "hitbox" which will be
				box.ll = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f, -17.f);   //Used in the 2d array map
				box.ur = terrain->gVertices[i].location + glm::vec3(-20.f, 0.f, 17.f);
				box.ul = terrain->gVertices[i].location + glm::vec3(-7.f, 0.f, 17.f);
				boxes.push_back(box);

				glm::mat4 translation = glm::translate(glm::mat4(1), terrain->gVertices[i].location - glm::vec3(0.f, 0.8f, 0.0f)); //Pinetree2
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * -90.f), glm::vec3(1.f, 0.f, 0.f));   //For pineTree2
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.060f, 0.065f, 0.085f)); //Pinetree2*/
				glm::mat4 transformation = translation * rotation * scale;
				modelMatrices.push_back(transformation);
				treeCount++;
			}
		}
	}

	for (int i = 0; i < boxes.size(); i++) { //Going through the "hitbox" vector to position the tree areas in the 2d array map
		int xBeg = static_cast<int>(boxes[i].ur.x); //Filling the array with respect to the coordinates so ground objects can have collisions with trees
		int xEnd = static_cast<int>(boxes[i].ul.x);

		int zBeg = static_cast<int>(boxes[i].lr.z);
		int zEnd = static_cast<int>(boxes[i].ur.z);

		for (int j = xBeg; j <= xEnd; j++) {
			for (int k = zBeg; k <= zEnd; k++) {
				gMap[j][k] = 1; //filling with 1s to indicate occupied space
			}
		}
	}

	std::cout << "Treecount TREES: " << treeCount << std::endl;
}

/*
* @brief binds model matrices and enable attribs in the shader
*
*/
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

/*
* @brief Draws the instanced trees
*/
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