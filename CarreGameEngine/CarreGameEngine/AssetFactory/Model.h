#pragma once

#include <iostream>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>					
#include <assimp/Importer.hpp>							// Assimp library used for loading
#include <assimp/scene.h>								// Guided by https://learnopengl.com/Model-Loading/Assimp
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "..\Controllers\Camera.h"
#include "..\Renderer\Shader.h"
#include "..\Texture\TextureManager.h"
#include "..\AI\ComputerAI.h"

	/**
	* @class Model
	* @brief Model class to store model data
	*
	* The model class is used to read in and store model data that is read in using
	* the assimp import asset library. It contains all the functionality to load in a model
	* of many different file types and store the information in various data structures
	* to be accessed later.
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Model
{
public:
		/**
		* @brief Constructor
		*
		* This is the default constuctor that sets the scale value of the model to 1,
		* initializes the models shader as well as setting the AI of the model to NULL.
		*
		* @return null
		*/
	Model();

		/**
		* @brief Destructor
		*
		* This is the default destructor that calls the Destroy() function to free up
		* any resources used by the model, incorporating memory management.
		*
		* @return null
		*/
	~Model() { Destroy(); }

		/**
		* @brief Loads the model data
		*
		* This function loads the model data taking in a relative string file path. The function then
		* uses the assimp import asset library to read the file and with other helper functions, store
		* the data in the correct member variable data structures.
		*
		* @param std::string filePath
		* @return void
		*/
	void LoadModel(std::string filePath);

		/**
		* @brief Processes the node of an aiScene	
		*
		* Assimp reads the loaded file as a scene that contains many nodes. The nodes are stored in a linked
		* list data structure. This function processes the specific node that is being examined.
		*
		* @param aiNode* node
		* @param const aiScene* scene
		* @return void
		*/
	void ProcessNode(aiNode* node, const aiScene* scene);

		/**
		* @brief Processes the mesh
		*
		* For each node that is contained in a scene, if it contains a mesh, then this function processes it.
		* It reads the vertex data in the aiNode object and stores them in the correct member data structures.
		*
		* @param aiMesh* mesh
		* @param const aiScene* scene
		* @return void
		*/
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		/**
		* @brief Loads the material textures
		*
		* Reads in the materials of the aiMesh and stores them in a temporary Texture data
		* structure that is then returned.
		*
		* @param aiMaterial* mat
		* @param aiTextureType type
		* @param std::string typeName
		* @return std::vector<Texture>
		*/
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		/**
		* @brief Destroys the model
		*
		* Deallocates any memory used by the model object.
		*
		* @return void
		*/
	void Destroy();

		/**
		* @brief Gets the models position
		*
		* Returns the position of the model as a glm::vec3.
		*
		* @return glm::vec3&
		*/
	glm::vec3& GetPosition() { return m_position; }
	
		/**
		* @brief Sets the models position
		*
		* Sets the position of the model using the parameter given.
		*
		* @param glm::vec3 position
		* @return void
		*/
	void SetPosition(glm::vec3 position);

		/**
		* @brief Gets the models rotation
		*
		* Returns the rotation of the model as a glm::vec3.
		*
		* @return glm::vec3&
		*/
	glm::vec3& GetRotation() { return m_rotation; }
	
		/**
		* @brief Sets the models rotation
		*
		* Sets the rotation of the model using the parameter given.
		*
		* @param glm::vec3 rotation
		* @return void
		*/
	void SetRotation(glm::vec3 rotation);

		/**
		* @brief Gets the models scale
		*
		* Returns the scale of the model as a glm::vec3.
		*
		* @return glm::vec3&
		*/
	glm::vec3& GetScale() { return m_scale; }
	
		/**
		* @brief Sets the models scale
		*
		* Sets the rotation of the model using the parameter given.
		*
		* @param glm::vec3 scale
		* @return void
		*/
	void SetScale(glm::vec3 scale);

		/**
		* @brief Gets the camera object
		*
		* Returns a pointer to the camera object to access the view and projection matrices.
		*
		* @return Camera*
		*/
	Camera* GetCamera() { return m_camera; }

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return const void
		*/
	const void SetCamera(Camera* camera) { m_camera = camera; }

		/**
		* @brief Gets the mesh batch
		*
		* Returns the vector of meshes of the model by reference.
		*
		* @return std::vector<Mesh>&
		*/
	std::vector<Mesh>& GetMeshBatch() { return m_meshBatch; }

		/**
		* @brief Gets the textures
		*
		* Returns the vector of textures of the model by reference.
		*
		* @return std::vector<Texture>&
		*/
	std::vector<Texture>& GetTextures() { return m_texturesLoaded; }

		/**
		* @brief Gets the shader
		*
		* Returns the shader object of the model.
		*
		* @return Shader*
		*/
	Shader* GetShader() { return m_shader; }

		/**
		* @brief Sets the shader
		*
		* Sets the shader member variable to the parameter shader parsed in.
		*
		* @param Shader* shader
		* @return void
		*/
	void SetShader(Shader* shader) { m_shader = shader; }

		/**
		* @brief Sets the AI of the model
		*
		* Sets the ComputerAI member variable of the model to tha parameter 
		* ComputerAI parsed in.
		*
		* @param ComputerAI* compAI
		* @return void
		*/
	void SetAI(ComputerAI* compAI) { m_compAI = compAI; }

		/**
		* @brief Gets the AI of the model
		*
		* Returns the ComputerAI member variable of the model as a pointer.
		*
		* @return ComputerAI*
		*/
	ComputerAI* GetAI() { return m_compAI; }

	unsigned int VAO;

	void CalculateMinMaxDimensions(glm::vec3 vertex);
	void ScaleDimensions();

protected:
	std::vector<Mesh> m_meshBatch;
	std::vector<Texture> m_texturesLoaded;
	std::string m_directory;

	Shader* m_shader;
	Camera* m_camera;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	
	ComputerAI* m_compAI;

	// Min and max for X, Y, Z dimensions of the model (used for bounding boxes)
	// Order is (min, max)
	glm::vec2 m_Xdim;
	glm::vec2 m_Ydim;
	glm::vec2 m_Zdim;
	bool m_firstVertex = true;
};


