/**
 * @file VertexArray.h
 * @brief The header file for the VertexArray class
 * 
 */
#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

/**
 * @class VertexArray
 * @brief Boilerplate OpenGL code handling VertexArrays. 
 */
class VertexArray
{
private:
	unsigned int rendererID;
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	
	void Bind() const;
	void Unbind() const;
	void changeData(VertexBuffer* VBO, const void* data, unsigned int size);
};

