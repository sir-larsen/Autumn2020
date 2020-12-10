/**
 * @file Terrain.cpp
 * @brief Source code for the Terrain class
 * 
 */
#include "Terrain.h"
#include "../Engine/Camera.h"

#include "../CoreGL/VertexBuffer.h"
#include "../CoreGL/IndexBuffer.h"
#include "../CoreGL/VertexArray.h"
#include "../CoreGL/Renderer.h"

#include <iostream>

/**
 * @brief Construct a new Maze:: Terrain object
*
 */
Terrain::Terrain(Shader* shader, Renderer* renderer, int m_depth, int breadth, float m_step)
	: m_Renderer(renderer),
	  m_Shader(shader)
{
	width = breadth;   //The x-direction of the landscape
	depth = m_depth;   
	step = m_step;
	generateMaze();
}

/**
 * @brief Destroy the terrain object
 * 
 */
Terrain::~Terrain(){
	delete dVAO;
	delete dVBO;
	delete dVBLayout;
	delete dIBO;

	delete gVAO;
	delete gVBO;
	delete gVBLayout;
	delete gIBO;

	delete sVAO;
	delete sVBO;
	delete sVBLayout;
	delete sIBO;

	delete snow;
	delete grass;
	delete dirt;

	delete noSpec;
	delete snowSpec;
	delete grassSpec;

	delete water;
	delete waterSpec;

	delete wVAO;
	delete wVBO;
	delete wVBLayout;
	delete wIBO;
}


/**
 * @brief Generates positions and everything needed to render the landscape
 * 
 */
void Terrain::makePositionsAndIndices()
{
	gScale = new Texture(5, "res/HeightMaps/Mosjoen_Height Map.png");

	Vertex terrain;
	int indexOff = 0;

	int gOffset, sOffset, dOffset;
	gOffset = sOffset = dOffset = 0;

	glm::vec3 offset;

	int vertices_width = width;
	int vertices_depth = depth;

	int tree = 0;
	for (int vx = 0; vx < vertices_width; vx++) {
		for (int vz = 0; vz < vertices_depth; vz++) {
			offset = glm::vec3(vx, 0.0f, vz);
			//float vy = 0.0f; // =getHeight(vx, vz); When height introduced
			int vyInt = gScale->getHeight(vx, vz);
			

			float vy = gScale->getHeight(offset.x + (0.0f), offset.z + (0.0f));

			if (vy < -37) {
				setVertex(terrain, glm::vec3(0.0f, vy, 0.0f), offset);
				dVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (0.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(0.0f, vy, 1.0f), offset);
				dVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 1.0f), offset);
				dVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (0.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 0.0f), offset);
				dVertices.push_back(terrain);

				dIndices.push_back(dOffset + 0);
				dIndices.push_back(dOffset + 1);
				dIndices.push_back(dOffset + 2);
				dIndices.push_back(dOffset + 2);
				dIndices.push_back(dOffset + 3);
				dIndices.push_back(dOffset + 0);
				dOffset += 4;
			}
			else if (vy > -16) {
				setVertex(terrain, glm::vec3(0.0f, vy, 0.0f), offset);
				sVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (0.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(0.0f, vy, 1.0f), offset);
				sVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 1.0f), offset);
				sVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (0.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 0.0f), offset);
				sVertices.push_back(terrain);

				sIndices.push_back(sOffset + 0);
				sIndices.push_back(sOffset + 1);
				sIndices.push_back(sOffset + 2);
				sIndices.push_back(sOffset + 2);
				sIndices.push_back(sOffset + 3);
				sIndices.push_back(sOffset + 0);
				sOffset += 4;
			}
			else {
				tree++;
				if (tree % 200 == 0) treeCount++;
				setVertex(terrain, glm::vec3(0.0f, vy, 0.0f), offset);
				gVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (0.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(0.0f, vy, 1.0f), offset);
				gVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (1.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 1.0f), offset);
				gVertices.push_back(terrain);

				vy = gScale->getHeight(offset.x + (1.0f), offset.z + (0.0f));
				setVertex(terrain, glm::vec3(1.0f, vy, 0.0f), offset);
				gVertices.push_back(terrain);

				gIndices.push_back(gOffset + 0);
				gIndices.push_back(gOffset + 1);
				gIndices.push_back(gOffset + 2);
				gIndices.push_back(gOffset + 2);
				gIndices.push_back(gOffset + 3);
				gIndices.push_back(gOffset + 0);
				gOffset += 4;
			}
		}
	}
	//Water********************
	terrain.location = glm::vec3(2.0f, -41.2f, 2.0f);
	terrain.normals = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	terrain.texCoords = glm::vec2(0.0f, 0.0f);
	wVertices.push_back(terrain);

	terrain.location = glm::vec3(2.0f, -41.2f, 1079.f);
	terrain.normals = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	terrain.texCoords = glm::vec2(25.0f, 0.0f);
	wVertices.push_back(terrain);

	terrain.location = glm::vec3(1079.0f, -41.2f, 1079.0f);
	terrain.normals = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	terrain.texCoords = glm::vec2(25.0f, 25.0f);
	wVertices.push_back(terrain);

	terrain.location = glm::vec3(1079.0f, -41.2f, 2.0f);
	terrain.normals = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	terrain.texCoords = glm::vec2(0.0f, 25.0f);
	wVertices.push_back(terrain);

	wIndices.push_back(0);
	wIndices.push_back(1);
	wIndices.push_back(2);
	wIndices.push_back(2);
	wIndices.push_back(3);
	wIndices.push_back(0);
}


void Terrain::generateMaze()
{
	makePositionsAndIndices();

	//Dirt VAO
	dVAO = new VertexArray;
	dVAO->Bind();
	dVBO = new VertexBuffer(&dVertices[0], dVertices.size() * sizeof(Vertex));

	dVBLayout = new VertexBufferLayout;
	dVBLayout->Push<float>(3);
	dVBLayout->Push<float>(3);
	dVBLayout->Push<float>(2);

	dVAO->AddBuffer(*dVBO, *dVBLayout);
	dIBO = new IndexBuffer(&dIndices[0], dIndices.size());
	//**************

	//Snow VAO
	sVAO = new VertexArray;
	sVAO->Bind();
	sVBO = new VertexBuffer(&sVertices[0], sVertices.size() * sizeof(Vertex));

	sVBLayout = new VertexBufferLayout;
	sVBLayout->Push<float>(3);
	sVBLayout->Push<float>(3);
	sVBLayout->Push<float>(2);

	sVAO->AddBuffer(*sVBO, *sVBLayout);
	sIBO = new IndexBuffer(&sIndices[0], sIndices.size());
	//*******************

	//Grass VAO
	gVAO = new VertexArray;
	gVAO->Bind();
	gVBO = new VertexBuffer(&gVertices[0], gVertices.size() * sizeof(Vertex));

	gVBLayout = new VertexBufferLayout;
	gVBLayout->Push<float>(3);
	gVBLayout->Push<float>(3);
	gVBLayout->Push<float>(2);

	gVAO->AddBuffer(*gVBO, *gVBLayout);
	gIBO = new IndexBuffer(&gIndices[0], gIndices.size());
	//*******************

	//Water VAO*************************
	wVAO = new VertexArray;
	wVAO->Bind();
	wVBO = new VertexBuffer(&wVertices[0], wVertices.size() * sizeof(Vertex));

	wVBLayout = new VertexBufferLayout;
	wVBLayout->Push<float>(3);
	wVBLayout->Push<float>(3);
	wVBLayout->Push<float>(2);

	wVAO->AddBuffer(*wVBO, *wVBLayout);
	wIBO = new IndexBuffer(&wIndices[0], wIndices.size());
	//**********************************

	m_Shader->use();
	
	dirt  = new Texture("res/textures/dirt.jpg", 0);

	grass = new Texture("res/textures/grass.jpg", 0);
	grassSpec = new Texture("res/textures/grass.jpg", 0);

	snow  = new Texture("res/textures/snow.jpg", 0);
	snowSpec = new Texture("res/textures/snow.jpg", 0);

	noSpec = new Texture("res/textures/spec.jpg", 0);

	water = new Texture("res/textures/water.jpg", 1);
	waterSpec = new Texture("res/textures/water.jpg", 1);

	m_Shader->setInt("material.diffuse", 0);
	m_Shader->setInt("material.specular", 1);
	
}

/**
 * @brief Draws the landscape
 * 
 */
void Terrain::draw(glm::mat4 projection, glm::mat4 view, const float dt)
{
	m_Shader->use();
	m_Shader->setMat4("u_ProjectionMat", projection);
	m_Shader->setMat4("u_ViewMat", view);
	Transform(dt);

	dirt->Bind(0);
	noSpec->Bind(1);
	m_Renderer->Draw(dVAO, dIBO, m_Shader);

	grass->Bind(0);
	grassSpec->Bind(1);
	m_Renderer->Draw(gVAO, gIBO, m_Shader);

	snow->Bind(0);
	snowSpec->Bind(1);
	m_Renderer->Draw(sVAO, sIBO, m_Shader);
	
	water->Bind(0);
	water->Bind(1);
	m_Renderer->Draw(wVAO, sIBO, m_Shader);
}

/**
* @brief Transforms the landscape
* @param dt - Delta time, will be used if one would want to rotate the maze
*/
void Terrain::Transform(float dt)
{
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(0.f));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1.f));
	glm::mat4 transformation = translation * scale;
	m_Shader->setMat4("u_TransformationMat", transformation);
	
}

void Terrain::Light(const float dt, Camera* camera, float time)
{
	m_Shader->setVec3("dirLight.direction", -0.3f, -0.5f, -0.5f);
	/*m_Shader->setVec3("dirLight.ambient", 0.12f, 0.12f, 0.12f);
	m_Shader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	m_Shader->setVec3("dirLight.specular", 0.65f, 0.65f, 0.65f);*/
	m_Shader->setVec3("dirLight.ambient", 0.03f, 0.02f, 0.28f);
	m_Shader->setVec3("dirLight.diffuse", sin(0.25*time-0.2), sin(0.25*time), sin(0.25*time));
	m_Shader->setVec3("dirLight.specular", 0.65f, 0.65f, 0.65f);

	/*m_Shader->setVec3("pointLight.position", glm::vec3(18.f, 1.3f, 14.f)/*camera->Position+glm::vec3(5.f,+0.7f,-25.f)*/
	/*m_Shader->setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	m_Shader->setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
	m_Shader->setVec3("pointLight.specular", 1.f, 1.f, 1.f);
	m_Shader->setFloat("pointLight.constant", 1.f);
	m_Shader->setFloat("pointLight.linear", 0.3);
	m_Shader->setFloat("pointLight.quadratic", 0.0032);*/

}

/*
* @brief Function for calculating the normals of vertices
*/
glm::vec3 Terrain::calcNormals(float x, float z)
{
	x = static_cast<int>(x);
	z = static_cast<int>(z);

	float heightL = gScale->getHeight(x - 1, z);
	float heightR = gScale->getHeight(x + 1, z);
	float heightD = gScale->getHeight(x, z - 1);
	float heightU = gScale->getHeight(x, z + 1);
	glm::vec3 normal = glm::vec3(heightL - heightR, 10.5f, heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}

/*
* @brief Helper function for setting the vertex data
*/
void Terrain::setVertex(Vertex& v, glm::vec3 p, const glm::vec3& offset)
{
	v.location = offset + p;
	v.normals  = calcNormals(offset.x + p.x, offset.z + p.z);

	if (p.x < 0.8f && p.z < 0.8f)
		v.texCoords = glm::vec2(0.0f, 0.0f);
	else if (p.x < 0.8f && p.z > 0.8f)
		v.texCoords = glm::vec2(1.0f, 0.0f);
	else if (p.x > 0.8f && p.z > 0.8f)
		v.texCoords = glm::vec2(1.0f, 1.0f);
	else
		v.texCoords = glm::vec2(0.0f, 1.0f);
}
