#include "Entity.h"

Entity::Entity(Model * model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	m_model(model), m_position(position), m_rotation(rotation), m_scale(scale)
{

}

void Entity::IncreasePosition(glm::vec3 move)
{
	m_position += move;
}

void Entity::IncreaseRotation(glm::vec3 rotate)
{
	m_rotation += rotate;
}

void Entity::IncreaseScale(glm::vec3 scale)
{
	m_scale += scale;
}

