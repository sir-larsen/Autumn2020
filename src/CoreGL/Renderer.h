/**
 * @file Renderer.h
 * @brief The header file for the Renderer class
 * 
 */
#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

/**
 * @class Renderer
 * @brief Handles everything related to rendering objects. 
 */
class Renderer
{
public:
	void Draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const;
	void Clear(float f0, float f1, float f2, float f3) const;
};
