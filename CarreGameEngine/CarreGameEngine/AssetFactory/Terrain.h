#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Model.h"
#include "..\headers\TextureManager.h"

	/**
	* @class Terrain
	* @brief Terrain Interface class
	*
	* This terrain class is used as a base class for other methods of terrain generation classes
	* to inherit from. It contains all the core member variables and functionalities that a terrain may
	* have.
	* 
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Terrain
{
public:
		/**
		* @brief Constructor
		*
		* This constructor sets the initial scales of the x, y, z components of the 
		* terrain to 1.0 and heightfield size to 0. It also initializes the terrains
		* model.
		*
		* @return null
		*/
	Terrain();

		/**
		* @brief Destructor
		*
		* Empty default destructor.
		*
		* @return null
		*/
	~Terrain() { }

		/**
		* @brief Constructor
		*
		* This constructor takes an x, y, z scale and sets the member variables
		* of the terrain to these parsed in.
		*
		* @return null
		*/
	Terrain(float scaleX, float scaleY, float scaleZ);

		/**
		* @brief Generates the terrain
		*
		* This virtual method is used to generate a terrain. It is overridden
		* by the inherited class.
		*
		* @param GLuint textureId
		* @param std::string textureFilePath
		* @return void
		*/
	virtual void GenerateTerrain(GLuint textureId, std::string textureFilePath) = 0;

		/**
		* @brief Sets the camera object
		*
		* Parses the parameter given Camera* object to the terrains model. This is to
		* be able to access the view and projection matrices when rendering.
		*
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera* camera) { m_terrainModel->SetCamera(camera); }
	
		/**
		* @brief Sets the terrains position
		*
		* Sets the position of the terrain using the parameter given.
		*
		* @param glm::vec3 position
		* @return void
		*/
	void SetPosition(glm::vec3 position) { m_terrainModel->SetPosition(position); }
	
		/**
		* @brief Sets the terrains scale
		*
		* Sets the scale of the terrain using the parameter given.
		*
		* @param glm::vec3 scale
		* @return void
		*/
	void SetScale(glm::vec3 scale) { m_terrainModel->SetScale(scale); }

		/**
		* @brief Gets the camera Model object
		*
		* Returns a pointer to the terrains model object.
		*
		* @return Model*
		*/
	Model* GetModel() { return m_terrainModel; }
	
protected:
	Model* m_terrainModel;
	unsigned char* m_terrainData;
	float m_scaleX, m_scaleY, m_scaleZ;
	int m_heightfieldSize;

	GLuint m_textureId;
};