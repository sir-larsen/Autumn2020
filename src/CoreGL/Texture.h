/**
 * @file Texture.h
 * @brief The header file for the Texture class
 * 
 */
#pragma once
#include <string>

/**
 * @class Texture
 * @brief Boilerplate OpenGL code for handling texture. 
 */
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& filepath, int hoi);
	Texture(int hoi, const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	float getHeight(int width, int depth);

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
};


