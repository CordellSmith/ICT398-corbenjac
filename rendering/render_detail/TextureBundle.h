//
//  TextureBundle.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTUREBUNDLE_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTUREBUNDLE_H_

#include <vector>
#include <memory>
#include "Texture.h"
#include "Shader.h"

namespace rendering {
	class TextureBundle {
	private:
		std::vector<Texture *> m_Textures;
		unsigned int m_ShaderID;

	public:
			/**
			 * @brief		Default constructor. Currently does nothing.
			 */
		TextureBundle() { }

			/**
			 * @brief		Adds a new texture into the texture bundle, from a file path.
			 * @param[in]	filepath	The file path of the texture being added.
			 */
		void AddTexture(std::string filepath);

			/**
			 * @brief		Sets the shader texture uniforms from this texture bundle.
			 * @param[in]	shader	The shader that is being used.
			 * @param[in]	count	The number of textures to be used formt his texture bundle.
			 */
		void SetTextureUniforms(Shader * shader, int count);

			/**
			 * @brief		Sets the textures of this texture buundle as active.
			 */
		void Bind() const; 

			/**
			 * @brief		Sets the textures of this texture buundle as inactive.
			 */
		void Unbind() const;
	};
};

#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_TEXTUREBUNDLE_H_ */