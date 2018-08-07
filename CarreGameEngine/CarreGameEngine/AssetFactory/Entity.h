#pragma once

#include <iostream>

#include "..\AssetFactory\Model.h"

	/**
	* @class Entity
	* @brief Entity class
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Entity
{
public:
	Entity() { }
	Entity(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Entity() { }

	void IncreasePosition(glm::vec3 vector);
	void IncreaseRotation(glm::vec3 rotate);
	void IncreaseScale(glm::vec3 scale);
	
	Model* GetModel() { return m_model; }
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetRotation() { return m_rotation; }
	glm::vec3 GetScale() { return m_scale; }

protected:
	Model* m_model;
	glm::vec3 m_position;

	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};