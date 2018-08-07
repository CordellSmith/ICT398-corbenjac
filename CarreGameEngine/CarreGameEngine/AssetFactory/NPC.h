#pragma once

#include "IGameAsset.h"
#include "..\AssetFactory\Model.h"

	/*
	* @class NPC
	* @brief NPC asset
	*
	* This class is used to store all the data about an NPC. It inherits from
	* the base class IGameAsset and allows for initialisation and drawing of
	* the NPC object.
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
class NPC : public IGameAsset
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	NPC() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~NPC() { }

		/**
		* @brief Parameter constructor
		*
		* Takes the parameter string file path and assigns it to the member
		* variable m_filePath. It then parses this into the LoadFromFilePath()
		* function.
		*
		* @param std::string filePath
		* @return null
		*/
	NPC(std::string filePath);

		/**
		* @brief Loads an NPC from file
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
		* Returns the file path containing the NPC information.
		*
		* @return std::string
		*/
	virtual const std::string & GetFilePath() const;

		/**
		* @brief Sets the file path
		*
		* Sets the file path containing the NPC information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath);

	virtual void SetCamera(Camera* camera) { m_model->SetCamera(camera); }

	virtual void SetPosition(glm::vec3 position) { m_model->SetPosition(position); }

	virtual void SetScale(glm::vec3 scale) { m_model->SetScale(scale); }
	
	virtual void SetRotation(glm::vec3 rotation) { m_model->SetRotation(rotation); }

	virtual const glm::vec3 GetPosition() { return m_model->GetPosition(); }

	virtual const glm::vec3 GetRotation() { return m_model->GetRotation(); }

	virtual Model* GetModel() { return m_model; }

	virtual void AddTexutre(GLuint textureId, std::string textureFilePath) { }

	void SetAI(ComputerAI* compAI) { m_model->SetAI(compAI); }

	ComputerAI* GetAI() { return m_model->GetAI(); }
		
protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	Model* m_model;
};