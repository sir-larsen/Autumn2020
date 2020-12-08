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
 * @class Terrain
 * @brief Handles the creation and drawing of the maze. 
 */
class Terrain
{
private:
	int width, depth;
	float step; //Step value for grid. Higher step, more distance between vertices

	ScenarioLoader* m_LoadedLevel;

	//Landscape and everything needed
	std::vector <Vertex>	   tVertices;
	std::vector <unsigned int> tIndices;

	VertexArray*        tVAO;
	VertexBuffer*       tVBO;
	IndexBuffer*        tIBO;
	VertexBufferLayout* tVBLayout;

	Texture*			gScale;
	//*******************************
	//Landscape and everything needed
	std::vector <Vertex>	   dVertices;
	std::vector <unsigned int> dIndices;

	VertexArray* dVAO;
	VertexBuffer* dVBO;
	IndexBuffer* dIBO;
	VertexBufferLayout* dVBLayout;

	//*******************************
	//Landscape and everything needed
	std::vector <Vertex>	   gVertices;
	std::vector <unsigned int> gIndices;

	VertexArray* gVAO;
	VertexBuffer* gVBO;
	IndexBuffer* gIBO;
	VertexBufferLayout* gVBLayout;

	//*******************************
	//Landscape and everything needed
	std::vector <Vertex>	   sVertices;
	std::vector <unsigned int> sIndices;

	VertexArray* sVAO;
	VertexBuffer* sVBO;
	IndexBuffer* sIBO;
	VertexBufferLayout* sVBLayout;

	//*******************************
	Texture* dirt;
	//Texture* sDirt;

	Texture* grass;
	Texture* snow;


	Renderer* m_Renderer;
	//Shader* m_Shader;
public:
	//void generateFloor();

	Shader* m_Shader;
	std::vector<std::vector<int>> map2d;

	Terrain(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer, int m_depth, int breadth, float m_step);
	~Terrain();

	void draw(glm::mat4 projection, glm::mat4 view, const float dt);
	/*std::vector <glm::vec3> getMazePositions() { return mazePositions; }
	int getHeight()	{ return height; }
	int getWidth()	{ return width; }
	int getPelletCount() { return pelletCount; }
	std::vector<std::vector<int>> getMap();*/
	void Transform(float dt);
	void Light(const float dt, Camera* camera);
	//glm::vec3 findSpawn();
private:
	/*void countPellets();
	void make2dArray();*/
	void makeIndices();
	void makePositionsAndIndices();
	void generateMaze();
	glm::vec3 calcNormals(float x, float z);
	void setVertex(Vertex& v, glm::vec3 p, const glm::vec3& offset);
};
