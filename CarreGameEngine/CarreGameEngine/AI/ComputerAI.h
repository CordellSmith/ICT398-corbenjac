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

class ComputerAI;

#ifndef COMPUTERAI_H
#define COMPUTERAI_H

// Includes
#include "State.h"
#include "StateMachine.h"
#include "GLM\glm.hpp"
#include <vector>
#include <time.h>

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

// Will need to be changed to a class
struct Emotions
{
	// range [1-100]
	float m_joy = 1.0f;
	float m_sad = 1.0f;
	float m_anger = 1.0f;
	float m_fear = 1.0f;
	float m_trust = 1.0f;
	float m_disgust = 1.0f;

	bool isJoy, isSad;
	bool isAnger, isFear;
	bool isTrust, isDisgusted;

	/*
		Emotions values change
		Rules should determine what state the AI is in (read from script)
			Change state
			eg. If m_joy > 70 && m_sad < 10
					change state -> happy;

			Happy state -> AI moves around much faster ie. AIspeed * 2
	*/
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

		float GetEnergy() { return m_energy; }

	protected:
		/// Return an instance of the current FSM state
		StateMachine<ComputerAI>* m_computerAIFSM;

		/// Current XYZ position
		glm::vec3 m_currPos;

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

		/// CSmith 23/10/18 Emotion Engine / Personality and Traits
		/// Personality
		Personality m_personality;

		/// Emotions
		Emotions m_emotions;

		/// Energy
		float m_energy;

		/// Movement Speed
		float m_speed;

		/// Strength
		float m_strength;
};
#endif
