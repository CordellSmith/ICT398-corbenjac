#pragma once

#include "Terrain.h"

	/**
	* @class Bruteforce
	* @brief Bruteforce terrain generation class
	*
	* This class inherits from the base Terrain class. It procedurally generates a terrain
	* using the bruteforce method.
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Bruteforce : public Terrain
{
public:
		/**
		* @brief Constructor
		*
		* Parameterised constructor that takes the desired scale of the terrain to be generated.
		* This is how large you want it to be in the X, Y and Z directions.
		*
		* @param float scaleX
		* @param float scaleY
		* @param float scaleZ
		* @return null
		*/
	Bruteforce(float scaleX, float scaleY, float scaleZ);

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Bruteforce() { }

		/**
		* @brief Generates the terrain
		*
		* Takes the heightfield data of the terrain and generates a mesh for it. The two parameters
		* are the textures to be assigned to the terrain for rendering.
		*
		* @param GLuint textureId
		* @param std::string textureFilePath
		* @return void
		*/
	virtual void GenerateTerrain(GLuint textureId, std::string textureFilePath) override;

		/**
		* @brief Loads the heightfield data
		*
		* Takes the relative path to the file that is the .raw image that is to be used
		* for the heightfield data. It also takes the images size which should be a power
		* of 2 (eg. 128x128 or 32x32). Returns true if created successfully, false if not.
		*
		* @param std::string fileName
		* @param const int size
		* @return bool
		*/
	bool LoadHeightfield(std::string fileName, const int size);

		/**
		* @brief Adds texture to mesh
		*
		* Creates a temporary texture and returns it.
		*
		* @param GLuint textureId
		* @param std::string filePath
		* @return Texture
		*/
	Texture AddTexture(GLuint textureId, std::string filePath);

		/**
		* @brief Checks if inbounds
		*
		* Checks if the parameter positions given are within that of the terrain.
		* Returns true if it is, false if not.
		*
		* @param int xpos
		* @param int ypos
		* @return bool
		*/
	bool Inbounds(int xpos, int zpos);

		/**
		* @brief Gets the height of the terrain
		*
		* Returns the height value y of the terrain at the corresponding parameters 
		* given x and z values.
		*
		* @param int xpos
		* @param int ypos
		* @return float
		*/
	float GetHeight(int xpos, int zpos);

		/**
		* @brief Gets the colour at a specific height
		*
		* Returns the height value y of the terrain at the corresponding parameters
		* given x and z values to be used in determining the colour of the vertex.
		*
		* @param int xpos
		* @param int ypos
		* @return unsigned char
		*/
	unsigned char GetHeightColour(int xpos, int zpos);

		/**
		* @brief Gets the average height
		*
		* Returns the y value of the x and z position parsed in as parameters. This
		* function is used for terrain collision.
		*
		* @param int xpos
		* @param int ypos
		* @return float
		*/
	float GetAverageHeight(int xpos, int zpos);

protected:
};