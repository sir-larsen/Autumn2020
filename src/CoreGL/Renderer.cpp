/**
 * @file Renderer.cpp
 * @brief The source code for the Renderer class
 * 
 */
#include "Renderer.h"
#include <iostream>

/**
 * @brief Draws an object.
 * 
 * @param va 		- The object to be drawn's VertexArray.
 * @param ib 		- The object to be drawn's IndexBuffer.
 * @param shader 	- The object to be drawn's Shader.
 */
void Renderer::Draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const
{
	shader->use();
	va->Bind();
	ib->Bind();
	glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);

}

/**
 * @brief Clears the screen in RGB colors.
 * 
 * @param f0 	- Red
 * @param f1  	- Green
 * @param f2 	- Blue
 * @param f3 	- Transparency
 */
void Renderer::Clear(float f0,float f1, float f2, float f3) const
{
	glClearColor(f0,f1,f2,f3);
	glClear(GL_COLOR_BUFFER_BIT);

}
