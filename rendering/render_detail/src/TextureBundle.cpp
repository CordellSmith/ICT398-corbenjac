//
//  TextureBundle.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../TextureBundle.h"

namespace rendering {
	void TextureBundle::AddTexture(std::string filepath) {
		m_Textures.push_back(new Texture(filepath));
		m_Textures.shrink_to_fit();
	}

	void TextureBundle::SetTextureUniforms(Shader * shader, int count) {
		std::string uniform_name;
		for (int i = 0; i < count; i++) {
			uniform_name = "u_Texture" + std::to_string(i);
			shader->SetUniform1i(uniform_name, m_Textures[i]->GetID());
		}
		
	}

	void TextureBundle::Bind() const {
		//if (m_Textures.size() == 1) {
		//	m_Textures[0]->Bind();
		//}
		//else {
			for (unsigned int i = 0; i < m_Textures.size(); i++) {
				m_Textures[i]->Bind(i + 1);
			}
		//}
	}

	void TextureBundle::Unbind() const {
		for (unsigned int i = 0; i < m_Textures.size(); i++) {
			m_Textures[0]->Unbind();
		}
	}
};