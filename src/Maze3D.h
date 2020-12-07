/**
 * @file Maze.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Maze class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "ScenarioLoader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
//#include "Shader.h"
#include "Camera.h"

struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;

};

/**
 * @class Maze
 * @brief Handles the creation and drawing of the maze. 
 */
class Maze3D
{
private:
	int width,
		height,
		pelletCount;

	ScenarioLoader* m_LoadedLevel;
	std::vector <unsigned int> mapIndices;
	std::vector <glm::vec3> mazePositions;
	std::vector <Vertex> maze3DVertices;

	std::vector <unsigned int> floorIndices;
	std::vector <glm::vec3>    floorPositions;
	std::vector <Vertex>       floorVertices;

	std::vector <unsigned int> nIndices;
	std::vector <Vertex>       nVertices;


	Renderer* m_Renderer;
	//Shader* m_Shader;

	VertexArray*		floorVAO;
	VertexBuffer*		floorVBO;
	IndexBuffer*		floorIBO;
	VertexBufferLayout* floorVBLayout;
	//Texture*			floorPic1;

	//Nyggz kødd
	VertexArray* nVAO;
	VertexBuffer* nVBO;
	IndexBuffer* nIBO;
	VertexBufferLayout* nVBLayout;
	//*************

	VertexArray*		mazeVAO;
	VertexBuffer*		mazeVBO;
	VertexBufferLayout* mazeVBLayout;
	IndexBuffer*		mazeIBO;
	Texture*			Diffuse;
	Texture*			Specular;
	Texture* nyggz;
public:
	//void generateFloor();

	Shader* m_Shader;
	std::vector<std::vector<int>> map2d;

	Maze3D(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer);
	~Maze3D();

	void draw(glm::mat4 projection, glm::mat4 view, const float dt);
	std::vector <glm::vec3> getMazePositions() { return mazePositions; }
	int getHeight()	{ return height; }
	int getWidth()	{ return width; }
	int getPelletCount() { return pelletCount; }
	std::vector<std::vector<int>> getMap();
	void Transform(float dt);
	void Light(const float dt, Camera* camera);
	glm::vec3 findSpawn();
private:
	void countPellets();
	void make2dArray();
	void makeIndices();
	void makePositionsAndIndices();
	void generateMaze();
};
