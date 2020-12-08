/**
 * @file Maze.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Maze class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Terrain.h"
#include "ScenarioLoader.h"
#include "Camera.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"

#include <iostream>

/**
 * @brief Construct a new Maze:: Maze object
 * 
 * @param loadedLevel - A ScenarioLoader containing the level file
 * @param shader 	  - The maze's shader
 * @param renderer 	  - The maze's renderer
 * 
 * @see	make2dArray();
 * @see	generateMaze();
 */
Terrain::Terrain(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer, int m_depth, int breadth, float m_step)
	: m_LoadedLevel(loadedLevel),
	  m_Renderer(renderer),
	  m_Shader(shader)
{
	/*width = m_LoadedLevel->getHorizontalSize();
	height = m_LoadedLevel->getVerticalSize();*/

	width = breadth;   //The x-direction of the landscape
	depth = m_depth;   
	step = m_step;

	//map2d.resize(height, std::vector<int>(width));
	//make2dArray();
	generateMaze();
	//countPellets();
}

/**
 * @brief Destroy the Maze:: Maze object
 * 
 */
Terrain::~Terrain(){
	/*free(mazeVAO);
	free(mazeVBO);
	free(mazeVBLayout);
	free(mazeIBO);*/

	/*free(tVAO);
	free(tVBO);
	free(tVBLayout);
	free(gScale);*/

	free(dVAO);
	free(dVBO);
	free(dVBLayout);
	free(dIBO);

	free(gVAO);
	free(gVBO);
	free(gVBLayout);
	free(gIBO);

	free(sVAO);
	free(sVBO);
	free(sVBLayout);
	free(sIBO);

	free(snow);
	free(grass);
	free(dirt);

	free(noSpec);
	free(snowSpec);
	free(grassSpec);

}

/**
 * @brief Converts the 1D vector of the map from the ScenarioLoader into a 2D vector. 
 * 
 */
/*void Terrain::make2dArray()
{
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			map2d[i][j] = m_LoadedLevel->mazeMap[index++];
		}
	}
}*/

/*std::vector<std::vector<int>> Terrain::getMap()
{
	return map2d;
}*/

/**
 * @brief Generates the indices for each square in the maze.
 * NOT IN USE ANYMORE
 */
/*void Terrain::makeIndices()
{
	
}*/

/**
 * @brief Generates positions to be used by the squares.
 * 
 */
void Terrain::makePositionsAndIndices()
{
	gScale = new Texture(5, "res/HeightMaps/Mosjoen_Height Map.png");
	float hoi = gScale->getHeight(10, 12);
	std::cout << "Elevation hoi: " << hoi << std::endl;

	Vertex terrain;
	int indexOff = 0;

	int gOffset, sOffset, dOffset;
	gOffset = sOffset = dOffset = 0;

	glm::vec3 offset;

	int vertices_width = width;
	int vertices_depth = depth;

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
			else if (vy > -13) {
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

			/*terrain.location = offset + glm::vec3(0.0f, vy, 0.0f);
			terrain.normals = calcNormals(offset.x+0.0f, offset.z+0.0f);
			terrain.texCoords = glm::vec2(0.0f, 0.0f);*/
			/*if (terrain.location.y > 14) {
				sVertices.push_back(terrain);
				sIndices.push_back(0);
			}
			else if (terrain.location.y < -11)
				dVertices.push_back(terrain);
			else
				gVertices.push_back(terrain);*/
			/*tVertices.push_back(terrain);

			vy = gScale->getHeight(offset.x + (0.0f), offset.z + (1.0f));
			terrain.location = offset + glm::vec3(0.0f, vy, 1.0f);
			terrain.normals = calcNormals(offset.x+0.0f, offset.z+1.0f);
			terrain.texCoords = glm::vec2(1.0f, 0.0f);
			tVertices.push_back(terrain);

			vy = gScale->getHeight(offset.x + (1.0f), offset.z + (1.0f));
			terrain.location = offset + glm::vec3(1.0f, vy, 1.0f);
			terrain.normals = calcNormals(offset.x+1.0, offset.z+1.0f);
			terrain.texCoords = glm::vec2(1.0f, 1.0f);
			tVertices.push_back(terrain);

			vy = gScale->getHeight(offset.x + (1.0f), offset.z + (0.0f));
			terrain.location = offset + glm::vec3(1.0f, vy, 0.0f);
			terrain.normals = calcNormals(offset.x+1.0f, offset.z+0.0f);
			terrain.texCoords = glm::vec2(0.0f, 1.0f);
			tVertices.push_back(terrain);

			tIndices.push_back(indexOff + 0);
			tIndices.push_back(indexOff + 1);
			tIndices.push_back(indexOff + 2);
			tIndices.push_back(indexOff + 2);
			tIndices.push_back(indexOff + 3);
			tIndices.push_back(indexOff + 0);
			indexOff += 4;*/

			/*for (int i = 0; i < tVertices.size(); i++) {
				if (tVertices[i].location.y > 14) {
					sVertices.push_back(tVertices[i]);
					sIndices.push_back(tIndices[i]);
				}
				else if (tVertices[i].location.y < -11) {
					dVertices.push_back(tVertices[i]);
					dIndices.push_back(tIndices[i]);
				}
				else {
					gVertices.push_back(tVertices[i]);
					gIndices.push_back(tIndices[i]);
				}
					
			}*/

		}
	}



	/*//Adding floor
	Vertex floor;
	floor.location  = glm::vec3(-10.0f, 0.0f, -10.0f);
	floor.normals   = glm::vec3(-10.0f, 1.0f, -10.0f);
	floor.texCoords = glm::vec2(0.0f, 0.0f);
	floorVertices.push_back(floor);

	floor.location  = glm::vec3(-10.0f, 0.0f, 200.f);
	floor.normals   = glm::vec3(-10.0f, 1.0f, 200.f);
	floor.texCoords = glm::vec2(0.0f, 500.0f);
	floorVertices.push_back(floor);

	floor.location = glm::vec3(200.f, 0.0f, 200.f);
	floor.normals = glm::vec3(200.0f, 1.0f, 200.0f);
	floor.texCoords = glm::vec2(500.0f, 500.0f);
	floorVertices.push_back(floor);

	floor.location = glm::vec3(200.f, 0.0f, -10.0f);
	floor.normals = glm::vec3(200.0f, 1.0f, -10.0f);
	floor.texCoords = glm::vec2(500.0f, 0.0f);
	floorVertices.push_back(floor);

	floorIndices.push_back(indexOff + 0);
	floorIndices.push_back(indexOff + 1);
	floorIndices.push_back(indexOff + 2);
	floorIndices.push_back(indexOff + 2);
	floorIndices.push_back(indexOff + 3);
	floorIndices.push_back(indexOff + 0);*/




	/*int height2, width2;
	height2 = 36, width2 = 28;
	for (int i = 0; i < height2; i++) {
		for (int j = 0; j < width2; j++) {
			offset = glm::vec3(j, 0.0f, i);

			//Adding 24 vertices and 36 indices
			if (i < map2d.size() && j < map2d[i].size() && map2d[i][j] == 1) {
				Vertex vertex3dNormTex;

				//Adding bottom face

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, -1.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, -1.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding top face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 2.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 2.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 2.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 2.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding left face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(-1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(-1.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset +glm::vec3(-1.0f, 1.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(-1.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding right face
				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(2.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(2.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(2.0f, 1.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(2.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding north face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 1.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 1.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding south face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 0.0f, 2.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 0.0f, 2.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 1.0f, 2.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 1.0f, 2.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;


				//Adding top face
				/*vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 2.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(0.0f, 2.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 2.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 0.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = offset + glm::vec3(1.0f, 2.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(1.0f, 1.0f);
				maze3DVertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

			}
		}
	}*/
}



/**
 * @brief Generates the maze, from the generation of the positions/vertices, to the OpenGL stuff. 
 * @see makePositions();
 * @see makeIndices();
 */
void Terrain::generateMaze()
{
	makePositionsAndIndices();
	//makeIndices();

	/*mazeVAO = new VertexArray;
	mazeVAO->Bind();
	mazeVBO = new VertexBuffer(&maze3DVertices[0], maze3DVertices.size() * sizeof(Vertex));
	mazeVBO->Bind();
	mazeVBLayout = new VertexBufferLayout;
	mazeVBLayout->Push<float>(3);
	mazeVBLayout->Push<float>(3);
	mazeVBLayout->Push<float>(2);	
	
	mazeVAO->AddBuffer(*mazeVBO, *mazeVBLayout);

	mazeIBO = new IndexBuffer(&mapIndices[0], mapIndices.size());



	floorVAO = new VertexArray;
	floorVAO->Bind();
	floorVBO = new VertexBuffer(&floorVertices[0], floorVertices.size() * sizeof(Vertex));
	floorVBLayout = new VertexBufferLayout;
	floorVBLayout->Push<float>(3);
	floorVBLayout->Push<float>(3);
	floorVBLayout->Push<float>(2);

	floorVAO->AddBuffer(*floorVBO, *floorVBLayout);
	floorIBO = new IndexBuffer(&floorIndices[0], floorIndices.size());*/

	//Terrain - old not in use
	/*tVAO = new VertexArray;
	tVAO->Bind();
	tVBO = new VertexBuffer(&tVertices[0], tVertices.size() * sizeof(Vertex));

	tVBLayout = new VertexBufferLayout;
	tVBLayout->Push<float>(3);
	tVBLayout->Push<float>(3);
	tVBLayout->Push<float>(2);

	tVAO->AddBuffer(*tVBO, *tVBLayout);
	tIBO = new IndexBuffer(&tIndices[0], tIndices.size());*/
	//*******************************

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

	m_Shader->use();
	
	//m_Shader->setUniform4f("u_Color", 0.f, 0.305f, 0.7f, 1.f);
	/*Diffuse = new Texture("res/Mossy_Stone.jpg");
	Diffuse->Bind(0);
	Specular = new Texture("res/Mossy_Stone.jpg");
	Specular->Bind(1);
	nyggz = new Texture("res/Mossy_Stone.jpg");*/
	
	dirt  = new Texture("res/dirt.jpg");

	grass = new Texture("res/grass.jpg");
	grassSpec = new Texture("res/grass.jpg");

	snow  = new Texture("res/snow.jpg");
	snowSpec = new Texture("res/snow.jpg");

	noSpec = new Texture("res/spec.jpg");

	//MAZEPIC2 GEN OG BIND(1) SKULLE VÆRT HER
	m_Shader->setInt("material.diffuse", 0);
	m_Shader->setInt("material.specular", 1);
	
}

/**
 * @brief Counts how many !1 occour in the maze, counting how many "pellets" exists
*/
/*void Terrain::countPellets()
{
	for (int i = 0; i < m_LoadedLevel->mazeMap.size(); i++)
		if (m_LoadedLevel->mazeMap[i] != 1)
			pelletCount++;
}*/

/**
 * @brief Draws the maze. 
 * 
 */
void Terrain::draw(glm::mat4 projection, glm::mat4 view, const float dt)
{
	m_Shader->use();
	m_Shader->setMat4("u_ProjectionMat", projection);
	m_Shader->setMat4("u_ViewMat", view);
	Transform(dt);
	/*Diffuse->Bind(0);
	Specular->Bind(1);*/
	//m_Renderer->Draw(mazeVAO, mazeIBO, m_Shader);
	//m_Renderer->Draw(floorVAO, floorIBO, m_Shader);
	//nyggz->Bind(0);
	//nyggz->Bind(1);
	//m_Renderer->Draw(nVAO, nIBO, m_Shader);
	//m_Renderer->Draw(tVAO, tIBO, m_Shader);
	dirt->Bind(0);
	noSpec->Bind(1);
	m_Renderer->Draw(dVAO, dIBO, m_Shader);
	grass->Bind(0);
	grassSpec->Bind(1);
	m_Renderer->Draw(gVAO, gIBO, m_Shader);
	snow->Bind(0);
	snowSpec->Bind(1);
	m_Renderer->Draw(sVAO, sIBO, m_Shader);


	/*m_Shader->camera(projection,view);
	m_Shader->Transform(dt);
	//shader->light missing

	mazePic1->Bind(0);
	mazePic2->Bind(1);
	m_Renderer->Draw(mazeVAO, mazeIBO, m_Shader);*/
}

/**
* @brief Transforms the 3D maze
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

	m_Shader->setVec3("pointLight.position", glm::vec3(18.f, 1.3f, 14.f)/*camera->Position+glm::vec3(5.f,+0.7f,-25.f)*/);
	m_Shader->setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	m_Shader->setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
	m_Shader->setVec3("pointLight.specular", 1.f, 1.f, 1.f);
	m_Shader->setFloat("pointLight.constant", 1.f);
	m_Shader->setFloat("pointLight.linear", 0.3);
	m_Shader->setFloat("pointLight.quadratic", 0.0032);

}

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

	//return glm::normalize(glm::vec3(x, 1, z));
	//return normal;
	//glm::vec3 n = glm::normalize(glm::vec3((heightU - heightD), (heightU - heightD) * (heightR - heightL), (heightR - heightL)));
	//return n;
	//return norm;
}

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

/*glm::vec3 Terrain::findSpawn()
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (map2d[y][x] == 2)
			{
				return glm::vec3((float)x + .5f, 0.5f, (float)y + .5f);
			}

}*/