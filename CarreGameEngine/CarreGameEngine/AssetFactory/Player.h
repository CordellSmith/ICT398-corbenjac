#pragma once

#include <iostream>

#include "Model.h"
#include "../Physics/PhysicsEngine.h"

	/**
	* @class Player
	* @brief Player class data and functionality
	*
	* The player class represents any character in the game that you can control.
	* It requires its own special class because it may contain attributes that other assets
	* or objects may not such as health or statistics.
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	* @author Ben Ward
	* @version 02 - made it suitable for a first person camera.
	* @date 11/09/2018
	*/
class Player 
{
public:
		/**
		* @brief Constructor
		*
		* This constructor takes the a string as the key to reference the player and sets it to the 
		* name member variable. It initializes the model, sets the current move speed to 0, current
		* turn speed to 0, move speed to 30 and turn speed to 0.1.
		*
		* @return null
		*/
	Player(std::string assetName);

		/**
		* @brief Destructor
		*
		* This the empty default destructor.
		*
		* @return null
		*/
	~Player() { }

		/**
		* @brief Load object from file path
		*
		* This function takes a file path as a string parameter to load a file.
		*
		* @param std::string filePath
		* @return void
		*/
	void LoadFromFilePath(std::string filePath);

		/**
		* @brief Destroys the player object
		*
		* This function deallocates any memory that the player class has used. Used
		* for memory management.
		*
		* @return const void
		*/
	const void Destroy();

		/**
		* @brief Moves the player forward
		*
		* Takes the delta time as a parameter (glfwGetTime()) and the forward vector and uses it to calculate the
		* speed at which the player will move. It then adjusts the position of the player
		* in a forward direction for movement.
		*
		* @return void
		*/
	void MoveForward(float time, glm::vec3& forward);

		/**
		* @brief Moves the player backward
		*
		* Takes the delta time as a parameter (glfwGetTime()) and the forward vector and uses it to calculate the
		* speed at which the player will move. It then adjusts the position of the player
		* in a backward direction for movement.
		*
		* @return void
		*/
	void MoveBackward(float time, glm::vec3& forward);

		/**
		* @brief Moves the player left
		*
		* Takes the delta time as a parameter (glfwGetTime()), as well as the vector 
		* representing the current left of the player and uses it to calculate the
		* speed and direction at which the player will move. It then adjusts the position of the player
		* in a right direction for movement.
		*
		* @return void
		*/
	void StrafeLeft(float time, glm::vec3& left);

		/**
		* @brief Moves the player left
		*
		* Takes the delta time as a parameter (glfwGetTime()), as well as the vector
		* representing the current left of the player and uses it to calculate the
		* speed and direction at which the player will move. It then adjusts the position of the player
		* in a right direction for movement.
		*
		* @return void
		*/
	void StrafeRight(float time, glm::vec3& left);

		/**
		* @brief Turns the player clockwise
		*
		* Takes the delta time as a parameter (glfwGetTime()) and uses it to calculate the
		* speed at which the player will turn. It then adjusts the rotation of the player
		* in a clockwise direction for movement.
		*
		* @return void
		*/
	void TurnClock(float time);

		/**
		* @brief Turns the player anti-clockwise
		*
		* Takes the delta time as a parameter (glfwGetTime()) and uses it to calculate the
		* speed at which the player will turn. It then adjusts the rotation of the player
		* in a anti-clockwise direction for movement.
		*
		* @return void
		*/
	void TurnAntiClock(float time);
	
		/**
		* @brief Gets the file path of the player
		*
		* Returns the file path that the players data was read in from as a string.
		*
		* @return const std::string& 
		*/
	const std::string& GetFilePath() const { return m_filePath; }

		/**
		* @brief Sets the file path of the player
		*
		* Sets the file path to which the data was read in from to the string parameter
		* parsed in.
		*
		* @param const std::string& filePath
		* @return void
		*/
	void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

		/**
		* @brief Sets the camera object
		*
		* Parses the parameter given Camera* object to the players model. This is to
		* be able to access the view and projection matrices when rendering.
		*
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera * camera) { m_playerModel->SetCamera(camera); }

		/**
		* @brief Gets the position
		*
		* Returns the position of the player in a 3 component glm::vec3.
		*
		* @return glm::vec3
		*/
	glm::vec3 GetPosition() const { return m_playerModel->GetPosition(); }

		/**
		* @brief Sets the position
		*
		* Sets the position of the player to the 3 component vector parsed in.
		*
		* @param glm::vec3 position
		* @return void
		*/
	void SetPosition(glm::vec3 position) { m_playerModel->SetPosition(position); }

		/**
		* @brief Gets the rotation
		*
		* Returns the rotation of the player in a 3 component glm::vec3.
		*
		* @return glm::vec3
		*/
	glm::vec3 GetRotation() const { return m_playerModel->GetRotation(); }

		/**
		* @brief Sets the rotation
		*
		* Sets the rotation of the player to the 3 component vector parsed in.
		*
		* @param glm::vec3 rotation
		* @return void
		*/
	void SetRotation(glm::vec3 rotation) { m_playerModel->SetRotation(rotation); }

		/**
		* @brief Sets the scale
		*
		* Sets the scale of the player to the 3 component vector parsed in.
		*
		* @param glm::vec3 scale
		* @return void
		*/
	void SetScale(glm::vec3 scale) { m_playerModel->SetScale(scale); }

		/**
		* @brief Gets the movement speed
		*
		* Returns the movement speed of the player as float.
		*
		* @return float
		*/
	float GetMoveSpeed() { return m_moveSpeed; }

		/**
		* @brief Gets the model of the player
		*
		* Returns a pointer to the model of the player.
		*
		* @return Model*
		*/
	Model* GetModel() { return m_playerModel; }

	void ThrowBall(float time, glm::vec3& forward);

	const void ParsePhysics(PhysicsEngine* physicsWorld, std::vector<btVector3>* collisionBodyPos)
	{ 
		m_physicsWorld = physicsWorld;
		m_collisionBodyPos = collisionBodyPos;
	}

protected:
	std::string m_filePath;
	std::string m_name;
	
	Model* m_playerModel;

	float m_currentMoveSpeed;
	float m_currentTurnSpeed;
	float m_moveSpeed = 2;
	float m_turnSpeed = 0.5;

	PhysicsEngine* m_physicsWorld;
	std::vector<btVector3>* m_collisionBodyPos;
};