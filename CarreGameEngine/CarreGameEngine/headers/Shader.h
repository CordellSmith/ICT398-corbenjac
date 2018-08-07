#pragma once

#include <iostream>
#include <string>										
#include <fstream>
#include <sstream>
#include "GL\glew.h"									

/// Struct to hold both vertex and fragment shaders (needs to be moved)
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

	/**
	* @class Shader
	* @brief Basic shader class
	*
	* This shader class is used to load and store our shader information as well as initialize
	* the shaders with OpenGL.
	*
	* @version 01
	* @date 29/03/2018 
	*
	* @version 02
	* @date 31/05/2018
	*/
class Shader
{
public:
		/**
		* @brief Default constructor
		*
		* This is the default constuctor that is empyty.
		*
		* @return null
		*/
	Shader() { }

		/**
		* @brief Destructor
		*
		* The destructor calls destroy that will free up any used memory.
		*
		* @return null
		*/
	~Shader() { Destroy(); }

		/**
		* @brief Loads the shader from file
		*
		* The function takes the file path given as a string parameter, opens up a file
		* stream to read the file and stores the file as a string in a local variable
		* that is then returned. It is used to read int and store the Vertex and Fragment 
		* shaders.
		* 
		*
		* @param std::string filePath
		* @return std::string
		*/
	std::string LoadShaderFile(std::string filePath);

		/**
		* @brief Initializes the shader program
		*
		* This function takes the vertex and fragment shaders given as string parameters
		* and creates both shdaders using glCreateShader(). The shader source is then set
		* and assiged to their respected ids. The shader program is created and assigned
		* to the m_shaderProgramId member variable and the two shaders are attached to this
		* program. The program is then linked and it runs a check for any GL errors.
		*
		* @return void
		*/
	void Initialize(std::string vertShader, std::string fragShader);
	
		/**
		* @brief Gets the uniform variable
		*
		* Returns an ID with the same name as the string parameter in the shader.
		*
		* @param std::string variable
		* @return GLint
		*/
	GLint GetVariable(std::string variable);

		/**
		* @brief Sets uniform value to int
		*
		* Calls glUniform1i() taking the id associated to the shader variable and changing
		* it to the new value set by the second parameter given.
		*
		* @param GLint id
		* @param int newValue
		* @return void
		*/
	void SetInt(GLint id, int newValue) { glUniform1i(id, newValue); }
	
		/**
		* @brief Sets uniform value to float
		*
		* Calls glUniform1f() taking the id associated to the shader variable and changing
		* it to the new value set by the second parameter given.
		*
		* @param GLint id
		* @param GLfloat newValue
		* @return void
		*/
	void SetFloat(GLint id, GLfloat newValue) { glUniform1f(id, newValue); }
	
		/**
		* @brief Selects uniform value to be changed
		*
		* Calls glUniform2f() taking the id associated to the shader variable and changing
		* the value according to the GLfloat parameters specified.
		*
		* @param GLint id
		* @param GLfloat v0
		* @param GLfloat v1
		* @return void
		*/
	void SetFloat2(GLint id, GLfloat v0, GLfloat v1) { glUniform2f(id, v0, v1); }
	
		/**
		* @brief Selects uniform value to be changed
		*
		* Calls glUniform3f() taking the id associated to the shader variable and changing
		* the value according to the GLfloat parameters specified.
		*
		* @param GLint id
		* @param GLfloat v0
		* @param GLfloat v1
		* @param GLfloat v2
		* @return void
		*/
	void SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(id, v0, v1, v2);	}
	
		/**
		* @brief Selects uniform value to be changed
		*
		* Calls glUniform4f() taking the id associated to the shader variable and changing
		* the value according to the GLfloat parameters specified.
		*
		* @param GLint id
		* @param GLfloat v0
		* @param GLfloat v1
		* @param GLfloat v2
		* @param GLfloat v3
		* @return void
		*/
	void SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glUniform4f(id, v0, v1, v2, v3); }
	
		/**
		* @brief Selects uniform value to be changed
		*
		* Calls glUniformMatrix4fv() taking the id associated to the shader variable and passing a 4x4 matrix
		* of size 16 bytes (4 floats).
		*
		* @param GLint id
		* @param GLsizei count
		* @param GLboolean transpose
		* @param const GLfloat* value
		* @return void
		*/
	void SetMatrix4(GLint id, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4fv(id, count, transpose, value); }

		/**
		* @brief Turns shader on
		*
		* Calls glUseProgram() turning the shader on.
		*
		* @return void
		*/
	void TurnOn() {	glUseProgram(m_shaderProgramId); }

		/**
		* @brief Turns shader off
		*
		* Calls glUseProgram(0) turning the shader off.
		*
		* @return void
		*/
	void TurnOff() { glUseProgram(0); }
	
		/**
		* @brief Destroys any linked shaders
		*
		* Detatches and deletes the shader information. Sets the m_vertexShaderId,
		* m_fragmentShaderId and m_shaderProgramId to 0.
		*
		* @return void
		*/
	void Destroy();

protected:
	/// Stores vertex shader information
	GLuint m_vertexShaderId;

	/// Stores fragment shader information
	GLuint m_fragmentShaderId;

	/// Stores the program information 
	GLuint m_shaderProgramId;
};

inline ShaderSource ParseShaders(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // stack allocated array that will store vertex and fragment strings
	ShaderType type = ShaderType::NONE; // set to none (-1) by default
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

