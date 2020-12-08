#include "Trees.h"
#include <iostream>

Trees::Trees(Model* m_Tree, Terrain* terrain)
{
	tree = m_Tree;
	generateModelMatrices(terrain);
	addModelMatrices();
}

void Trees::generateModelMatrices(Terrain* terrain)
{	
	//200 som mod-verdi
	for (int i = 0; i < terrain->gVertices.size()-800; i++) {
		if (i % 1100 == 0) {
			//glm::mat4 translation; 
			
			if (terrain->gVertices[i].location.x < 1063 && terrain->gVertices[i].location.x > 17 &&
				terrain->gVertices[i].location.z < 1063 && terrain->gVertices[i].location.z > 17) {
				
				glm::mat4 translation = glm::translate(glm::mat4(1), terrain->gVertices[i].location - glm::vec3(0.f, 0.8f, 0.0f));
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(1 * -90.f), glm::vec3(1.f, 0.f, 0.f));
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.027f, 0.019f, 0.027f));
				glm::mat4 transformation = translation * rotation * scale;
				modelMatrices.push_back(transformation);
				treeCount++;
			}
		}
	}

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