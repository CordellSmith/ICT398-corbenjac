#pragma once

#ifndef AFFORDANCE_H
#define AFFORDANCE_H

#include <string>
#include <unordered_map>

/// Predefined class
class CollisionBody;
/// Data structure used to hold base affordance data
typedef std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> AffordanceData;

/*
* @class Affordance
* @brief Affordance object
* @author Cordell Smith
*
* Contains all information about an objects affordance and its
* actionable properties.
*
* @version 01
* @date 10/10/18
*/
class Affordance
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	Affordance(std::string name);

		/**
		* @brief Parameter constructor
		*
		* Takes the parameters and assigns them to their respected
		* member variables on creation of affordance object.
		*
		* @param std::string filePath
		* @return null
		*/
	Affordance(std::string name, float sitOn, float standOn, float kick);
	
		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Affordance() { }

		/**
		* @brief Gets the name
		*
		* Returns the name of the affordance object.
		*
		* @return std::string const
		*/
	std::string GetName() const { return m_name; }

		/**
		* @brief Sets the name
		*
		* Sets the name of the affordance object to the
		* parameter given.
		*
		* @param std::string name
		* @return void
		*/
	void SetName(std::string name) { m_name = name; }

		/**
		* @brief Initializes the base affordances
		*
		* Sets the base affordances of all the collision bodies parsed
		* in as a parameter to the affordance data parsed in as a
		* parameter.
		*
		* @param const AffordanceData& affordanceData
		* @param std::vector<CollisionBody*>& collisionBodies
		* @return const void
		*/
	const void InitBaseAffordances(const AffordanceData& affordanceData, std::vector<CollisionBody*>& collisionBodies);

		/**
		* @brief Gets the sitOn
		*
		* Gets the sitOn value of the affordance object.
		*
		* @return const float& value
		*/
	const float& GetSitOn() { return m_sitOn; }

		/**
		* @brief Sets the sitOn
		*
		* Sets the sitOn value to the parameter given.
		*
		* @param const float& value
		* @return void
		*/
	void SetSitOn(const float& value) { m_sitOn = value; }

		/**
		* @brief Gets the standOn
		*
		* Gets the standOn value of the affordance object.
		*
		* @return const float& value
		*/
	const float& GetStandOn() { return m_standOn; }

		/**
		* @brief Sets the standOn
		*
		* Sets the standOn value to the parameter given.
		*
		* @param const float& value
		* @return void
		*/
	void SetStandOn(const float& value) { m_standOn = value; }

		/**
		* @brief Gets the kick
		*
		* Gets the kick value of the affordance object.
		*
		* @return const float& value
		*/
	const float& GetKick() { return m_kick; }

		/**
		* @brief Sets the kick
		*
		* Sets the kick value to the parameter given.
		*
		* @param const float& value
		* @return void
		*/
	void SetKick(const float& value) { m_kick = value; }
	
protected:
	/// Affordances and corresponding value
	float m_sitOn, m_standOn, m_kick;
	std::string m_name;
};
#endif // !AFFORDANCE_H


