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
*
* @date 23/10/2018
* @author Cordell Smith
* @version 2.1	Emotion Engine / Personality and Traits
*/

#ifndef COMPUTERAI_H
#define COMPUTERAI_H

class ComputerAI;

// Includes
#include "State.h"
#include "StateMachine.h"
#include "GLM\glm.hpp"
#include <vector>
#include <time.h>
#include "Emotions\EmotionalState.h"

struct Trait
{
	std::string m_name;
	float energy = 0.0f, speed = 0.0f, strength = 0.0f;

	//void ApplyTrait(ComputerAI& AI)
	//{
	//	AI.GetEnergy() += energy;
	//	AI.GetSpeed() += speed;
	//	AI.GetStrength() += strength;
	//}
};

struct Personality
{
	std::vector<Trait> m_traits;

	void InitTraits(ComputerAI& AI)
	{
		if (m_traits.size() == 0)
			std::cout << "AI has no Traits" << std::endl;
		else
		{
			for (int i = 0; i < m_traits.size(); i++)
			{
				//m_traits[i].ApplyTrait(AI);
			}
		}
	}
};

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
		void SetVelocity(glm::vec3 vel);
		
			/**
			* @brief Return velocity
			*
			* Returns the current velocity
			*
			* @return glm::vec3 - m_currVel
			*/
		glm::vec3 GetVelocity();

			/**
			* @brief Set position
			*
			* Sets the current position
			*
			* @param pos - Position to be set
			*
			* @return void
			*/
		void SetPosition(glm::vec3 pos);
		
			/**
			* @brief Return position
			*
			* Returns the current position
			*
			* @return Vector2 - m_currPos
			*/
		glm::vec3 GetPosition();

			/**
			* @brief Set rotation
			*
			* Sets the current rotation
			*
			* @param pos - Rotation to be set
			*
			* @return void
			*/
		void SetRotation(glm::vec3 rot);

			/**
			* @brief Return rotation
			*
			* Returns the current position
			*
			* @return Vector3 - m_currRot
			*/
		glm::vec3 GetRotation();

			/**
			* @brief Move to a location
			*
			* Moves to a given location (waypoint, player, other AI)
			*
			* @param compAI - AI that is moving
			*
			* @return bool - True if at position, false otherwise
			*/
		bool MoveTo(ComputerAI* compAI, glm::vec3 targetPos);

			/**
			* @brief Return FSM
			*
			* Returns the FSM attached to this AI
			*
			* @return StateMachine<ComputerAI> - The FSM of the AI
			*/
		StateMachine<ComputerAI>* GetFSM() const { return m_computerAIFSM; }

		std::vector<glm::vec3> MakeWaypoints();
		std::vector<glm::vec3> GetWaypoints() { return m_waypoints; }

		void SetTargetWaypoint(int waypoint);
		glm::vec3& GetTargetWaypoint();

		float GetEnergy() { return m_energy; }

	protected:
		/// Return an instance of the current FSM state
		StateMachine<ComputerAI>* m_computerAIFSM;

		/// Current XYZ position
		glm::vec3 m_currPos;

		/// Current rotation
		glm::vec3 m_currRot;

		/// Current velocity
		glm::vec3 m_currVel;

		/// Current health points
		int m_health;

		/// Maximum health points
		int m_maxHealth;
		
		/// Dead or not
		bool m_isDead;

		/// Vector of waypoints
		std::vector<glm::vec3> m_waypoints;


		int targetWaypoint;

		/// CSmith 23/10/18 Emotion Engine / Personality and Traits
		/// Personality
		Personality m_personality;

		/// Emotions
		EmotionalState m_emotions;

		/// Energy
		float m_energy;

		/// Movement Speed
		float m_speed;

		/// Strength
		float m_strength;
};
#endif
