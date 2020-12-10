/**
 * @file Terrain.h
 * @author Alexander Eckhoff Larsen
 * @brief Header file for the Terrain class
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
#include "Camera.h"

struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

/**
 * @class Terrain
 * @brief Handles the creation and drawing of the terrain 
 */
class Terrain
{
private:
	int   width, depth;
	int   treeCount = 0;
	float step; //Step value for grid. Higher step, more distance between vertices

	//Landscape and everything needed
	std::vector <Vertex>	   tVertices;
	std::vector <unsigned int> tIndices;

	VertexArray*        tVAO;
	VertexBuffer*       tVBO;
	IndexBuffer*        tIBO;
	VertexBufferLayout* tVBLayout;
public:
	Texture*			gScale;
private:
	//Water VAO
	std::vector <Vertex>	   wVertices;
	std::vector <unsigned int> wIndices;

	VertexArray*  wVAO;
	VertexBuffer* wVBO;
	IndexBuffer*  wIBO;
	VertexBufferLayout* wVBLayout;
	Texture* water;
	Texture* waterSpec;

	//Dirt buffers
	std::vector <Vertex>	   dVertices;
	std::vector <unsigned int> dIndices;

	VertexArray*  dVAO;
	VertexBuffer* dVBO;
	IndexBuffer*  dIBO;
	VertexBufferLayout* dVBLayout;

	//Grass buffers
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
	Texture* grassSpec;

	Texture* snow;
	Texture* snowSpec;
	
	Texture* noSpec;



	Renderer* m_Renderer;
	//Shader* m_Shader;
public:
	//void generateFloor();
	std::vector <Vertex>	   gVertices;
	Shader* m_Shader;
	std::vector<std::vector<int>> map2d;

	Terrain(Shader* shader, Renderer* renderer, int m_depth, int breadth, float m_step);
	~Terrain();

	void draw(glm::mat4 projection, glm::mat4 view, const float dt);
	void Transform(float dt);
	void Light(const float dt, Camera* camera, float time);
	int getTreeCount() { return treeCount; }
private:
	void makePositionsAndIndices();
	void generateMaze();
	glm::vec3 calcNormals(float x, float z);
	void setVertex(Vertex& v, glm::vec3 p, const glm::vec3& offset);
};
