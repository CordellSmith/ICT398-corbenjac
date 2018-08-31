#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>						
#include <GL\glew.h>

#include "..\Texture\TextureManager.h"
#include "..\Common\Vertex3.h"
#include "..\Renderer\Shader.h"


	/**
	* @class Mesh
	* @brief Used to store mesh data
	*
	* The mesh class is what holds all the vertex information about an objects model. There may
	* be many meshes that make up a model. Each mesh contains vertices, indices and textures that
	* are used to draw the specific model.
	* 
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Mesh
{
public:
		/**
		* @brief Constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	Mesh() { }

		/**
		* @brief Destructor
		*
		* Empty default destructor.
		*
		* @return null
		*/
	~Mesh() { }

		/**
		* @brief Constructor
		*
		* This constructor takes in vertices, indices and texture data and assigns them to
		* member variables of the mesh.
		*
		* @return null
		*/
	Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		/**
		* @brief Sets the position
		*
		* Sets the position of the mesh to the 3 component vector parsed in.
		*
		* @param glm::vec3 position
		* @return void
		*/
	void SetPosition(glm::vec3 position) { m_position = position; }

		/**
		* @brief Sets the rotation
		*
		* Sets the rotation of the mesh to the 3 component vector parsed in.
		*
		* @param glm::vec3 rotation
		* @return void
		*/
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }

		/**
		* @brief Sets the scale
		*
		* Sets the scale of the mesh to the 3 component vector parsed in.
		*
		* @param glm::vec3 scale
		* @return void
		*/
	void SetScale(glm::vec3 scale) { m_scale = scale; }

		/**
		* @brief Gets the vertices of the mesh
		*
		* Returns the vertices data of the mesh by reference.
		*
		* @return std::vector<Vertex3>&
		*/
	std::vector<Vertex3>& GetVertices() { return m_vertices; }

		/**
		* @brief Sets the vertices of the mesh
		*
		* Sets the vertices data of the mesh to the parsed in parameter value.
		*
		* @param std::vector<Vertex3> vertices
		* @return void
		*/
	void SetVertices(std::vector<Vertex3> vertices) { m_vertices = vertices; }

		/**
		* @brief Gets the indices of the mesh
		*
		* Returns the indices data of the mesh by reference.
		*
		* @return std::vector<unsigned int>&
		*/
	std::vector<unsigned int>& GetIndices() { return m_indices; }

		/**
		* @brief Sets the indices of the mesh
		*
		* Sets the indices data of the mesh to the parsed in parameter value.
		*
		* @param std::vector<unsigned int> indices
		* @return void
		*/
	void SetIndices(std::vector<unsigned int> indices) { m_indices = indices; }

		/**
		* @brief Gets the textures of the mesh
		*
		* Gets the textures data of the mesh by reference.
		*
		* @return std::vector<Texture>&
		*/
	std::vector<Texture>& GetTextures() { return m_textures; }
		
	unsigned int VAO, VBO, EBO;

protected:
	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};