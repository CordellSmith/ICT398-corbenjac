/**
* @class TextureManager
* @brief Texture manager class that handles the loading, storing, and managing of all textures
* @author Jack Matters
*
* @date 03/07/2018
* @version 1.0	Initial start. Created the layout of the class, using a singleton and the SOIL API.
*
* @date 07/07/2018
* @version 1.1	Changed from SOIL to SOIL2, as SOIL was deprecated. Got file loading and storing in map
*				working. Next to work on is a better system for loading files (from lua), and using the
*				loaded in textures.
*
* @date 10/07/2018
* @version 1.2	Added a few more useful functions, and some tidy up.
*
* @date 17/07/2018
* @version 1.3	Altered LoadTexture to check to see if texture is already in map before loading the texture.
*				Also added an function to help with that check.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

// Includes
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "GL\glew.h"
#include "soil2.h"
#include "stb_image.h"

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	std::string m_path;
};

class TextureManager
{
	public:

			/**
			* @brief Singleton instance
			*
			* This function returns a singleton instance of the texture manager class so that there is only
			* one instance of a texture manager.
			*
			* @return static TextureManager&
			*/
		static TextureManager& Instance()
		{
			static TextureManager instance;

			return instance;
		}

			/**
			* @brief Activate a texture
			*
			* Sets the given texture to be active
			*
			* @param texID - Texture to set active
			*
			* @return bool - True
			*/
		bool SetActiveTexture(unsigned int texID);

			/**
			* @brief Load a texture
			*
			* Load a texture from file using the SOIL API
			*
			* @param filename - Name of file to load
			*
			* @return int - texID (given ID of loaded texture)
			*/
		int LoadTexture(std::string filename);

			/**
			* @brief Add a texture to the map
			*
			* Adds a texture that is loaded into the map for storing and later access
			*
			* @param filename - Name of file loaded
			* @param texID - ID of texture
			*
			* @return void
			*/
		void AddTextureToMap(std::string filename, int texID);

			/**
			* @brief Return a texture
			*
			* Returns a texture ID to be used
			*
			* @param filename - Name of texture file to be returned
			*
			* @return int - texID (given ID and requested texture)
			*/
		int GetTextureID(std::string filename);

			/**
			* @brief Release all textures
			*
			* Releases all textures from memory (called in de-constructor
			*
			* @return void
			*/
		void ReleaseAllTextures();

			/**
			* @brief Find out if texture is loaded
			*
			* Checks map to see if a texture is already loaded, prior to loading it
			*
			* @param filePath - File path of texture
			*
			* @return int - texID of loaded texture, or 0 if not in map
			*/
		int TexLoaded(std::string filePath);

	private:

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*
			* @return null
			*/
		TextureManager();

			/**
			* @brief De-constructor
			*
			* This is the deconstructor
			*/
		~TextureManager();

			/**
			* @brief
			*
			* An unordered map containing all textures for easy lookup
			*/
		std::unordered_map<std::string, unsigned int> m_textureMap;

		/// Number of textures
		int m_numTextures;

	protected:

		/// Width of texture loaded
		int m_width;
		
		/// Height of texture loaded
		int m_height;
};

#endif