//
//  Texture.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTURE_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTURE_H_

#include <string>

namespace rendering {
	class Texture {
	private:
		unsigned int   m_ID;
		
		int m_Width;
		int m_Height;
		int m_BitsPerPixel;

	public:
			/**
			 * @brief		Constructor that sets up texture from a file path.
			 * @param[in]	path	String representing the file path of the texture.
			 */
		Texture(const std::string filepath);

			/** 
			 * @brief		[NON_FUNCTIONAL] Constructor that sets up a texture from a char array, height and width.
			 * @param[in]	data			The raw image data of the texture.
			 * @param[in]	count_width		The width of the texture.
			 * @param[in]	count_height	The height of the texture.
			 */
		Texture(unsigned char * data, unsigned int count_width, unsigned int count_height);

			/**
			 * @brief		Destructor. Deletes texture from use.
			 */
		~Texture();

			/**
			 * @brief		Sets this texture as active.
			 * @param[in]	slot	
			 */
		void Bind(unsigned int slot = 0) const;

			/**
			 * @brief		Sets this texture as inactive.
			 */
		void Unbind() const;

			/**
			 * @brief		Gets the ID of this texture.
			 * @return		The ID of the texture.
			 */
		unsigned int GetID() const { return m_ID; }

			/**
			 * @brief		Returns the width of the texture.
			 * @return		The width of the texture.
			 */
		int GetWidth()  const { return m_Width;  }

			/**
			 * @brief		Returns the height of the texture.
			 * @return		The height of the texture.
			 */
		int GetHeight() const { return m_Height; }
	};
};

#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTURE_H_ */