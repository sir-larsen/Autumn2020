/**
 * @file Texture.cpp 
 * @brief The source file for the Texture class
 * 
 */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"
#include "GL/glew.h"
#include <iostream>

/**
 * @brief Construct a new Texture:: Texture object for greyscale height map
 * 
 * @param filepath - The filepath to the file containing the texture
 */
Texture::Texture(int hoi, const std::string& filepath)
	: m_RendererID(0),
	m_FilePath(filepath),
	m_LocalBuffer(nullptr),
	m_Width(0),
	m_Height(0),
	m_BPP(0)
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
}

/**
 * @brief Construct a new Texture:: Texture object
 *
 * @param filepath - The filepath to the file containing the texture
 */
Texture::Texture(const std::string& filepath)
	: m_RendererID(0),
	m_FilePath(filepath),
	m_LocalBuffer(nullptr),
	m_Width(0),
	m_Height(0),
	m_BPP(0)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	
	glGenTextures(1, &m_RendererID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT/*GL_CLAMP_TO_EDGE*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT/*GL_CLAMP_TO_EDGE*/);

	//glBindTexture(GL_TEXTURE_2D, m_RendererID);
	Bind(0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	Unbind();

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

/**
 * @brief Destroy the Texture:: Texture object
 * 
 */
Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

/**
 * @brief Binds the object's texture to a slot
 * 
 * @param slot - The slot that is to be bound to. 
 */
void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

/**
 * @brief Unbinds the texture. 
 * 
 */
void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D,0);
}
float Texture::getHeight(int width, int depth)
{
	float elevation = static_cast<float>(*(m_LocalBuffer + ((width * m_Width + depth)*4)));
	//std::cout << "elevation: " << elevation << std::endl;
	return elevation;
}