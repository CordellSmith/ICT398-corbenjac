//
//  Shader.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../RenderCommon.h"

namespace rendering {
	ShaderProgramSource Shader::ParseShader(const std::string &filepath) {
		std::ifstream stream(filepath);
		std::cout << "Reading File: " << filepath << std::endl;

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return{ ss[0].str(), ss[1].str() };
	}

	/*
	ShaderProgramSource Shader::CreateFromString(const std::string source) {
		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			std::cout << line << std::endl;
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return{ ss[0].str(), ss[1].str() };
	}
	*/
	unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
	{
		unsigned int id = glCreateShader(type);
		const char *src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char *message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
	{

		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		unsigned int program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "LINK ERROR OH NO" << std::endl;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	Shader::Shader() : m_FilePath(""), m_RendererID(0) { };

	Shader::Shader(const std::string &filename)
		: m_FilePath(filename), m_RendererID(0)
	{
		ShaderProgramSource source = ParseShader(filename);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	void Shader::SetSource(const std::string &filename) {
		m_FilePath = filename;
		m_RendererID = 0;
		ShaderProgramSource source = ParseShader(filename);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1i(const std::string name, int val)
	{
		glUniform1i(GetUniformLocation(name), val);
	}

	void Shader::SetUniform1f(const std::string name, float val) {
		glUniform1f(GetUniformLocation(name), val);
	}

	void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::SetUniformMat4fv(const std::string name, const float * matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
	}

	void Shader::SetUniform3f(const std::string name, float v0, float v1, float v2)
	{
		//
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
		//);
	}
	//caching for uniforms

	int Shader::GetUniformLocation(const std::string &name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) { return m_UniformLocationCache[name]; }

		int location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1) { std::cout << "Warning: uniform " << name << " does not exist." << std::endl; }

		m_UniformLocationCache[name] = location;

		return location;
	}

};		/* rendering */
