#include "Camera.h"

glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_nearPlane = near;
	m_farPlane = far;

	m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

	return m_projectionMatrix;
}

void Camera::PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch)
{
	m_position = glm::vec3(positionX, positionY, positionZ);
	m_yaw = yaw;
	m_pitch = pitch;
}

glm::mat4 Camera::GetRotationMatrix()
{
	glm::mat4 rotationMatrix(0.5f);

	// Add the Pitch rotation along the x and y axis
	rotationMatrix = glm::rotate(rotationMatrix, glm::degrees(m_pitch), glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, glm::degrees(m_yaw), glm::vec3(0, 1, 0));

	return rotationMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	// Return a view matrix by multiplying our rotation matrix by the inverse of a translation matrix
	return GetRotationMatrix() * glm::inverse(glm::translate(glm::mat4(), m_position));
}

glm::vec3 Camera::GetView()
{
	// Looking down the negative z-axis.
	glm::vec4 viewVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 0, -1, 1);

	return glm::vec3(viewVector);
}

glm::vec3 Camera::GetUp()
{
	glm::vec4 upVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 1, 0, 1);

	return glm::vec3(upVector);
}

void Camera::MoveCamera(float speed)
{
	// Get our normalized view vector
	glm::vec3 viewVector = GetView();

	m_position.x += viewVector.x * speed;
	m_position.z += viewVector.z * speed;
}

void Camera::Zoom(float yoffset)
{
	// multiplication factor changes how fast scrolling in and out occurs
	int mf = 4;

	if (glm::degrees(m_fov) >= 1.0 && glm::degrees(m_fov) <= 61.0)
	{
		float change = glm::degrees(m_fov);
		change -= yoffset * mf;
		m_fov = glm::radians(change);
	}
	if (glm::degrees(m_fov) <= 10.0)
		m_fov = glm::radians(10.0);
	if (glm::degrees(m_fov) >= 61.0)
		m_fov = glm::radians(60.0);

	m_distanceFromPlayer -= yoffset;
	m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::ChangePitch(float yoffest)
{
	// multiplication factor changes how fast pitch up and down occurs
	float mf = 0.01;
	float pitchChange = yoffest;
	m_pitch += pitchChange * mf;

	if (m_pitch > glm::radians(80.0))
		m_pitch = glm::radians(80.0);
	if (m_pitch < glm::radians(-80.0))
		m_pitch = glm::radians(-80.0);
}

void Camera::ChangeYaw(float xoffset)
{
	// multiplication factor changes how fast pitch up and down occurs
	float mf = 0.1;
	float angleChange = xoffset * mf;
	m_angleAroundPlayer -= angleChange;

	float horizontalDistance = CalculateHorizontalDistance();
	float verticalDistance = CalculateVerticalDistance();

	CalculateCameraPosition(horizontalDistance, verticalDistance);
}

float Camera::CalculateHorizontalDistance()
{
	return (float)m_distanceFromPlayer * glm::cos(m_pitch);
}

float Camera::CalculateVerticalDistance()
{
	return (float)m_distanceFromPlayer * glm::sin(m_pitch);
}

void Camera::CalculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	float theta = m_playerRotation.y + m_angleAroundPlayer;
	float xoffset = horizontalDistance * glm::sin(glm::radians(theta));
	float zoffset = horizontalDistance * glm::cos(glm::radians(theta));

	m_position.x = m_playerPosition.x - xoffset;
	m_position.y = m_playerPosition.y + verticalDistance;
	m_position.z = m_playerPosition.z - zoffset;

	m_yaw = glm::radians(180 - (m_playerRotation.y + m_angleAroundPlayer));	
}

void Camera::ParsePlayerInfo(glm::vec3& position, glm::vec3& rotation)
{
	m_playerPosition = position;
	m_playerRotation = rotation;
}
