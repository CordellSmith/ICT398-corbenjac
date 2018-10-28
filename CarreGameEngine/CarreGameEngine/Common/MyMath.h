#pragma once

#include <iostream>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>
#include "btBulletDynamicsCommon.h"

#ifndef MYMATH_H
#define MYMATH

#include "..\Controllers\Camera.h"

class MyMath
{
public:
	MyMath() { }
	~MyMath() { }

protected:

};

// Convert btVector3 to glm::vec3
inline glm::vec3 BttoGlm(const btVector3& btVector)
{
	return glm::vec3(btVector.getX(), btVector.getY(), btVector.getZ());
}

// Convert glm::vec3 to btVector3
inline btVector3 GlmtoBt(const glm::vec3& glmVector)
{
	return btVector3(glmVector.x, glmVector.y, glmVector.z);
}

inline glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 modelMatrix;
	
	// Order is important, must be Translate, Rotate then Scale
	// TRANSLATE
	modelMatrix = glm::translate(modelMatrix, translation);

	// ROTATE
	modelMatrix = glm::rotate(modelMatrix, glm::degrees(rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::degrees(rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::degrees(rotation.z), glm::vec3(0, 0, 1));

	// SCALE
	modelMatrix = glm::scale(modelMatrix, scale);

	return modelMatrix;
}

inline glm::mat4 CreateViewMatrix(Camera* camera)
{
	glm::mat4 viewMatrix;

	// ROTATE
	viewMatrix = glm::rotate(viewMatrix, glm::degrees(camera->GetPitch()), glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, glm::degrees(camera->GetYaw()), glm::vec3(0, 1, 0));

	glm::vec3 negativeCameraPos = glm::vec3(-camera->GetPosition().x, -camera->GetPosition().y, -camera->GetPosition().z);

	// TRANSLATE
	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);

	return viewMatrix;
}
#endif // !MYMATH_H