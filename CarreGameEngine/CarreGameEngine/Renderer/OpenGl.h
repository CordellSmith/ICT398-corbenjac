#pragma once

#include <vector>
#include <GL\glew.h>

#include "..\Common\MyMath.h"
#include "..\AssetFactory\Model.h"
//#include "IRenderer.h" // Will make this class use IRenderer later

	/**
	* @class OpenGl
	* @brief OpenGl renderer class
	*
	* This class is the facade class that implements all the necessary functionalities
	* of the OpenGl API into the game engine. It handles creating VAOs, VBOs and EBOs for
	* model data as well as the 3D rendering and uniform calculations to be correctly
	* rendered.
	* 
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class OpenGl
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	OpenGl() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~OpenGl() { }

		/**
		* @brief Prepare
		*
		* Takes model data, vertex shader string and fragment shader string and prepares
		* the VAO, VBO and EBO attribute data. It stored the correct data in each attribute of
		* the models VAO, VBO and EBO to be used in the rendering process later on.
		*
		* @param Model* model
		* @param std::string vertShader
		* @param std::string fragShader
		* @return void
		*/
	void Prepare(Model* model, std::string vertShader, std::string fragShader);

		/**
		* @brief Render
		*
		* Takes model data that is parsed in as a parameter and accesses the necessary opneGl
		* components to render the model to the screen. This includes the textures associated
		* with the model data and its respective mesh data.
		*
		* @param Model* model
		* @return void
		*/
	void Render(Model* model);

protected:

};