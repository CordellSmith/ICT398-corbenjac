/**
* @class State
* @brief ComputerAI class that handles all AI behaviour
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef COMPUTERAI_H
#define COMPUTERAI_H

// Includes
#include "State.h"
#include "StateMachine.h"
#include "GLM\glm.hpp"
#include <vector>
#include <time.h>


class ComputerAI
{
	public:

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*/
		ComputerAI();

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*/
		ComputerAI(glm::vec3 pos);

			/**
			* @brief De-constructor
			*
			* This is the de-constructor
			*/
		~ComputerAI();

			/**
			* @brief Increase health
			*
			* Increase the total value of health, up to maximum
			*
			* @param health - Amount to increase
			*
			* @return void
			*/
		void AddHealth(int health);

			/**
			* @brief Decrease health
			*
			* Decrease the total value of health
			*
			* @param health - Amount to decrease
			*
			* @return void
			*/
		void DecreaseHealth(int health);

			/**
			* @brief Returns health
			*
			* Return the current health point value
			*
			* @return int - m_health
			*/
		int GetHealth();

			/**
			* @brief Returns if dead
			*
			* Return m_isDead to determine if dead or not
			*
			* @return bool - True if dead, false otherwise
			*/
		bool IsDead();
		
			/**
			* @brief Update state
			*
			* Updates the current state
			*
			* @return void
			*/
		void Update();

			/**
			* @brief Set velocity
			*
			* Sets the current velocity
			*
			* @param vel - Velocity to be set
			*
			* @return void
			*/
		void SetVelocity(Vector2 vel);
		
			/**
			* @brief Return velocity
			*
			* Returns the current velocity
			*
			* @return Vector2 - m_currVel
			*/
		Vector2 GetVelocity();

			/**
			* @brief Set position
			*
			* Sets the current position
			*
			* @param pos - Position to be set
			*
			* @return void
			*/
		void SetPosition(Vector2 pos);
		
			/**
			* @brief Return position
			*
			* Returns the current position
			*
			* @return Vector2 - m_currPos
			*/
		Vector2 GetPosition();

			/**
			* @brief Move to a location
			*
			* Moves to a given location (waypoint, player, other AI)
			*
			* @param compAI - AI that is moving
			*
			* @return bool - True if at position, false otherwise
			*/
		bool MoveTo(ComputerAI* compAI, Vector2 targetPos);

			/**
			* @brief Return FSM
			*
			* Returns the FSM attached to this AI
			*
			* @return StateMachine<ComputerAI> - The FSM of the AI
			*/
		StateMachine<ComputerAI>* GetFSM() const { return m_computerAIFSM; }

		std::vector<Vector2> MakeWaypoints();

		//std::vector<Vector2> MakeWaypoints();

	private:

		/// Return an instance of the current FSM state
		StateMachine<ComputerAI>* m_computerAIFSM;

		/// Current XZ position
		Vector2 m_currPos;

		/// Current velocity
		Vector2 m_currVel;

		/// Current health points
		int m_health;

		/// Maximum health points
		int m_maxHealth;
		
		/// Dead or not
		bool m_isDead;

		/// Vector of waypoints
		std::vector<Vector2> m_waypoints;

		
};
#endif
