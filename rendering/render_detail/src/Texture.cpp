//
//  Texture.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../Texture.h"

#include "../RenderCommon.h"
#include "../../../game_object/ModelAndImageryCache.h"


//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

namespace rendering {

	Texture::Texture(const std::string filepath)
		: m_ID(0), m_Width(0), m_Height(0), m_BitsPerPixel(0) {
		
		TextureCache cache;
		m_ID = cache.GetTexture(filepath, &m_Width, &m_Height, &m_BitsPerPixel);


		//stbi_set_flip_vertically_on_load(1);
		//unsigned char * texture_buffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, STBI_rgb_alpha);
	}

	// Don't use this, it does nothing.
	Texture::Texture(unsigned char * data, unsigned int count_width, unsigned int count_height) 
		: m_ID(0), m_Width(0), m_Height(0), m_BitsPerPixel(0) { }

	Texture::~Texture() { 
		glDeleteTextures(1, &m_ID); 
	}

	//TODO: Investigate the use of: glBindSampler(m_ID, SamplerHandle);
	void Texture::Bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture::Unbind() const { 
		glBindTexture(GL_TEXTURE_2D, 0); 
	}
};