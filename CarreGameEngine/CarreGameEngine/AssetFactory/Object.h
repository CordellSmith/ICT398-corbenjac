#pragma once

#include "IGameAsset.h"
#include "Model.h"

	/*
	* @class Object
	* @brief Object asset (static/dynamic)
	*
	* This class is used to store all the data about an Object. It inherits from
	* the base class IGameAsset and allows for initialisation and drawing of
	* the object.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	* @version 02
	* @date 31/05/2018
	*/
class Object : public IGameAsset
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	Object() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Object() { Destroy(); }

		/**
		* @brief Parameter constructor
		*
		* Takes the parameter string asset name and creates a new
		* Object with that name identifier. Initialises the model of the
		* obejct as well.
		*
		* @param std::string assetName
		* @return null
		*/
	Object(std::string assetName);

		/**
		* @brief Loads an object from file
		*
		* Takes the parameter string file path and loads the NPC data into the
		* corresponding model member variable.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

	virtual const void Destroy();

		/**
		* @brief Gets the file path
		*
		* Returns the file path containing the object information.
		*
		* @return std::string
		*/
	virtual const std::string& GetFilePath() const { return m_filePath; }

		/**
		* @brief Sets the file path
		*
		* Sets the file path containing the object information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

	virtual void SetCamera(Camera* camera) { m_model->SetCamera(camera); }

	virtual void SetPosition(glm::vec3 position) { m_model->SetPosition(position); }
	
	virtual void SetScale(glm::vec3 scale) { m_model->SetScale(scale); }

	virtual void SetRotation(glm::vec3 rotation) { m_model->SetRotation(rotation); }
	
	virtual const glm::vec3 GetPosition() { return m_model->GetPosition(); }

	virtual const glm::vec3 GetRotation() { return m_model->GetRotation(); }
		
	virtual Model* GetModel() { return m_model; }

	virtual void AddTexutre(GLuint textureId, std::string textureFilePath);
	
	void SetModel(Model* model) { m_model = model; }

	void SetAI(ComputerAI* compAI) { m_model->SetAI(compAI); }

	ComputerAI* GetAI() { return m_model->GetAI(); }

		/**
		* @brief Scales the dimensions
		*
		* Virtual function to scale the dimensions of the model for
		* physics bounding box creation.
		*
		* @return void
		*/
	virtual void ScaleDimensions() { m_model->ScaleDimensions(); }

protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	Model* m_model;
};