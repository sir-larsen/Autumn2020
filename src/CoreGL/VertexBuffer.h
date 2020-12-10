/**
 * @file VertexBuffer.h
 * @brief The header file for the VertexBuffer class
 * 
 */
#pragma once
/**
 * @class VertexBuffer
 * @brief Boilerplate OpenGL code handling VertexBuffers
 */
class VertexBuffer
{
private:
	unsigned int renderer_ID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void updateBuffer(const void* data, unsigned int size);
};

