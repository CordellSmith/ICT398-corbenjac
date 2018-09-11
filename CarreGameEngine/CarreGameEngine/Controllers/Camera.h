#pragma once

/// Define PI for usage in positioning the camera's rotations
#define PI 3.1415926	
#define GLM_FORCE_RADIANS								

#include <iostream>
#include <fstream>
/// Used for the GLM math library
#include <GLM\glm.hpp>									
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

	/**
	* @class Camera
	* @brief Basic camera class
	*
	* This class contains the camera object that will allow us to move around and view the
	* world.
	*
	* @version 01
	* @date 29/03/2018
	*
	* @version 02
	* @date 31/05/2018
	*
	* @author Ben Ward
	* @version 03 - made it suitable for a first person camera.
	* @date 11/09/2018
	*/
class Camera
{
public:
	/**
	* @brief Default constructor
	*
	* Constructor sets the default origin of the camera at 0,0,0.
	*
	* @return null
	*/
	Camera() { 
		m_position = glm::vec3(0.0, 0.0, 0.0); 
		m_distanceFromPlayer = 450.0;
		m_angleAroundPlayer = 0.0;
	}

	/**
	* @brief Constructor to set position and distance from player.
	*
	* Sets cmaera position and the distance from player to a specified value.
	*
	* @param float distanceFromPlayer
	*
	* @return null
	*/
	Camera(float distanceFromPlayer) {
		m_position = glm::vec3(0.0, 0.0, 0.0);
		m_distanceFromPlayer = distanceFromPlayer;
		m_angleAroundPlayer = 0.0;
	}

	/**
	* @brief Default constructor
	*
	* This sets the perspective matrix that is built from the field of fiew, aspect ratio
	* and near/far planes
	*
	* @param float fov
	* @param float aspectRatio
	* @param float nearPlane
	* @param float farPlane
	* @return glm::mat4
	*/
	glm::mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

	/**
	* @brief Sets up the camera
	*
	* This sets the Position and the yaw and pitch rotations (in radians).
	*
	* @param float positionX
	* @param float positionY
	* @param float positionZ
	* @param float yaw
	* @param float pitch
	* @return void
	*/
	void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);
	
	/**
	* @brief Gets the rotation matrix
	*
	* This function builds and returns a rotation matrix from the yaw and pitch rotations.
	*
	* @return glm::mat4
	*/
	glm::mat4 GetRotationMatrix();

	/**
	* @brief Gets the projection matrix
	*
	* This function returns the current projection matrix.
	*
	* @return glm::mat4
	*/
	glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }

	/**
	* @brief Gets the view matrix
	*
	* This function returns the current view matrix.
	*
	* @return glm::mat4
	*/
	glm::mat4 GetViewMatrix();

	/**
	* @brief Sets camera position
	*
	* This function sets the camera's position to the parameter given vec3.
	*
	* @return void
	*/
	void SetPosition(glm::vec3 position) { m_position = position; }

	/**
	* @brief Gets the camera position
	*
	* This function returns the cameras position in a vec3.
	*
	* @return glm::vec3
	*/
	glm::vec3& GetPosition() { return m_position; }

	/**
	* @brief Gets the camera view
	*
	* This function returns the cameras forwards vector.
	*
	* @return glm::vec3
	*/
	glm::vec3 GetView();

	/**
	* @brief Gets the camera position
	*
	* This function returns the camera's up vector (the direction pointing up)
	* from the rotation matrix.
	*
	* @return glm::vec3
	*/
	glm::vec3 GetUp();

	/**
	* @brief Gets the camera yaw
	*
	* This function returns the yaw of the camera.
	*
	* @return float
	*/
	float& GetYaw() { return m_yaw; }

	/**
	* @brief Sets the yaw
	*
	* This function sets the yaw of the camera to the float parameter given.
	*
	* @param float yaw
	* @return void
	*/
	void SetYaw(float yaw) { m_yaw = yaw; }

	/**
	* @brief Gets the camera pitch
	*
	* This function returns the pitch of the camera.
	*
	* @return float
	*/
	float& GetPitch() { return m_pitch; }

	/**
	* @brief Sets the camera pitch
	*
	* This function sets the pitch of the camera to the float parameter given.
	*
	* @param float pitch
	* @return void
	*/
	void SetPitch(float pitch) { m_pitch = pitch; }

	/**
	* @brief Sets the camera movement speed
	*
	* This function sets the movement speed of the camera to the double parameter given.
	*
	* @param double speed
	* @return void
	*/
	void SetSpeed(double speed) { m_speed = (float)speed; }

	/**
	* @brief Gets the camera speed
	*
	* This function returns the speed of the camera.
	*
	* @return double
	*/
	double GetSpeed() { return m_speed; }

	/**
	* @brief Sets the camera rotation speed
	*
	* This function sets the rotation speed of the camera to the double parameter given.
	*
	* @param double speed
	* @return void
	*/
	void SetRotationSpeed(double speed) { m_rotationSpeed = (float)speed; }

	/**
	* @brief Gets the camera rotation speed
	*
	* This function returns the rotation speed of the camera.
	*
	* @return double
	*/
	double GetRotationSpeed() { return m_rotationSpeed; }

	/**
	* @brief Sets the view of the camera according to the mouse movement
	*
	* This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame.
	*
	* @param float mouseX
	* @param float mouseY
	* @return void
	*/
	void SetViewByMouse(float mouseX, float mouseY);

	/**
	* @brief Moves the camera
	*
	* This function will move the camera forward or backwards depending on the speed (negative for backwards).
	*
	* @param float speed
	* @return void
	*/
	void MoveCamera(float speed);

	float& GetFov() { return m_fov; }
	void SetFov(float fov) { m_fov = fov; }
	float& GetNearPlane() { return m_nearPlane; }
	float& GetFarPlane() { return m_farPlane; }

	void Zoom(float yoffset);
	void ChangePitch(float yoffset);
	void ChangeYaw(float xoffset);
	float CalculateHorizontalDistance();
	float CalculateVerticalDistance();
	void CalculateCameraPosition(float horizontalDistance, float verticalDistance);

	void ParsePlayerInfo(glm::vec3& position, glm::vec3& rotation);
	
protected:
	/// The camera's projection matrix
	glm::mat4 m_projectionMatrix;
	/// The camera's position
	glm::vec3 m_position;	

	/// The camera's speed that is used with a time slice
	float m_speed = 50.0;
	/// The rotation speed for the camera
	float m_rotationSpeed = 2;						
	/// The speed for the mouse rotations
	double m_mouseSpeed = 0.001;
	/// The horizontal rotation angle (in radians) y-axis
	float m_yaw = 0;		
	/// The vertical rotation angle (in radians) x-axis
	float m_pitch = 0;
	/// The fov
	float m_fov;
	/// aspect ratio of the screen
	float m_aspectRatio;
	/// minimum rendering distance
	float m_nearPlane;
	/// maximum rendering distance
	float m_farPlane;
	
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerRotation;
	glm::vec3 m_playerScale;
	float m_distanceFromPlayer;
	float m_angleAroundPlayer;
};