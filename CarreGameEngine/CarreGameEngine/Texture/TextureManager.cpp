/*
* Implementation of TextureManager.h file
* Author - Jack Matters
*/

//Includes
#include "TextureManager.h"
#include <sstream>

// Default constructor
TextureManager::TextureManager()
{
	m_numTextures = 0;
}

// De-constructor
TextureManager::~TextureManager()
{
	ReleaseAllTextures();
}

// Set texture to be active
bool TextureManager::SetActiveTexture(unsigned int texID)
{
	// Enable and bind the passed in texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	return true;
}

// Load texture from file
int TextureManager::LoadTexture(std::string filePath)
{
	// Check to see if texture is already loaded and in map, and return texID if true
	if (TexLoaded(filePath) != 0)
	{
		std::cout << "File already loaded: " << filePath << std::endl;
		return TexLoaded(filePath);
	}

	// Load in the file
	//GLuint newTex = SOIL_load_OGL_texture
	//(
	//	file,
	//	SOIL_LOAD_AUTO,
	//	SOIL_CREATE_NEW_ID,
	//	SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	//);

	//// Check for error in loading file
	//if (newTex == 0)
	//{
	//	std::cout << "SOIL loading error: " << filename << " - " << SOIL_last_result() << std::endl;
	//	return -1;
	//}

	// Generate a new texID
	GLuint newTex;
	glGenTextures(1, &newTex);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		m_width = width;
		m_height = height;

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, newTex);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		// Increment number of textures and return ID
		m_numTextures++;
		std::cout << "Successfully added Texture. Texture Count = " << m_numTextures << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load: " << filePath << std::endl;
		stbi_image_free(data);
	}

	// Add new texture to make
	AddTextureToMap(filePath, newTex);

	return newTex;
}

// Add loaded texture to map
void TextureManager::AddTextureToMap(std::string filename, int texID)
{
	// If loaded texture is not already in map, load it
	if (!m_textureMap[filename])
	{
		m_textureMap[filename] = texID;
	}
}

// Return a texture ID
int TextureManager::GetTextureID(std::string filename)
{
	// Get iterator to start of map
	std::unordered_map<std::string, unsigned int>::iterator it = m_textureMap.begin();

	// Search map for texture
	while (it != m_textureMap.end())
	{
		// If texture found, return the given ID
		if ((*it).first == filename)
		{
			return (*it).second;
		}

		// Increment iterator
		it++;
	}

	// If texture is not in map, load it in and add to map
	int texID = LoadTexture(filename);

	// Return texture ID
	return texID;
}

// Release all textures from memory
void TextureManager::ReleaseAllTextures()
{
	// Get iterator to start of map
	std::unordered_map<std::string, unsigned int>::iterator it = m_textureMap.begin();

	// Loop through map, unloading all textures from map
	while (it != m_textureMap.end())
	{
		std::cout << "unloading Texture: " << (*it).first << ", ID: " << (*it).second;
		glDeleteTextures(1, &(*it).second);
		std::cout << " ... done" << std::endl;
		++it;
	}

	// Clear the map
	m_textureMap.clear();
}

// Check to see if a texture is already loaded or not
int TextureManager::TexLoaded(std::string filePath)
{
	// Get iterator to start of map
	std::unordered_map<std::string, unsigned int>::iterator it = m_textureMap.begin();

	// Search map for texture
	while (it != m_textureMap.end())
	{
		// If texture found, return true
		if ((*it).first == filePath)
		{
			return (*it).second;;
		}

		// Increment iterator
		it++;
	}

	// If texture not found, return 0
	return 0;
}