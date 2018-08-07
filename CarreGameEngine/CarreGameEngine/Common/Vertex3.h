#pragma once

#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

struct Vertex3
{
	glm::vec3 m_position; // The x,y,z position
	glm::vec2 m_texCoords; // The uv coordinates
	glm::vec3 m_normal; // The normal coordinates
	glm::vec4 m_colour; // Colour r,g,b,a
	//glm::vec3 m_tangent; // unused
	//glm::vec3 m_biTangent; // unused
};