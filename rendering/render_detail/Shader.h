//
//  Shader.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_DETAIL_SHADER_H_
#define ZENENGINE_CORE_RENDERING_DETAIL_SHADER_H_

#include <string>
#include <unordered_map>

namespace rendering {
	/**
	 * @brief Represents the uncompiled shader/s
	 */
	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	/**
	 * @brief Abstraction for a shader
	 * @author Anna
	 */
	class Shader {
	public:
		Shader();
		Shader(const std::string &filename);
		~Shader();

		unsigned int GetID() {
			return m_RendererID;
		}
		/**m
		 * @brief makes shader active
		 */
		void Bind() const;

		/**
		 * @brief Unbinds the active shader
		 */
		void Unbind() const;

		/**
		 * @brief makes new shader
		 */
		void SetSource(const std::string &filename);

		/**
		 * @brief sets a uniform of type int
		 * @param name the name of the uniform
		 * @param val the int value
		 */
		void SetUniform1i(const std::string name, int val);
		
		/**
		 * @brief sets a uniform of type float
		 * @param name the name of the uniform
		 * @param val the float value
		 */
		void SetUniform1f(const std::string name, float val);

		/**
		 * @brief sets a uniform of type vec4f from 4 floats
		 * @param name the name of the uniform
		 * @param v0
		 * @param v1
		 * @param v2
		 * @param v3
		 */
		void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);
		
		/**
		 * @brief sets a uniform of type mat4f
		 * @param name the name of the uniform
		 * @param mat the matrix value
		 */
		void SetUniformMat4fv(const std::string name, const float * mat);
		
		/**
		 * @brief sets a uniform of type vec3f from 3 floats
		 * @param name the name of the uniform
		 * @param v0
		 * @param v1
		 * @param v2
		 */
		void SetUniform3f(const std::string name, float v0, float v1, float v2);

	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		std::unordered_map<std::string, int> m_UniformLocationCache;

		/**
		 * @brief parses the shader file 
		 * @param filepath
		 */
		ShaderProgramSource ParseShader(const std::string &filepath);
	
		/**
		* @brief compiles the shader source
		* @param type what kind of shader
		* @param source the parsed shader source
		*/
		unsigned int CompileShader(unsigned int type, const std::string &source);
		
		/**
		* @brief creates a full shader program (ready to use)
		* @param vertexShader
		* @param fragmentShader
		*/
		unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

		/**
		* @brief get memory location of the uniform
		* @param name of the uniform
		*/
		int GetUniformLocation(const std::string &name);
	};	/* Shader */
};		/* rendering */
#endif	/* ZENENGINE_CORE_RENDERING_DETAIL_SHADER_H_ */