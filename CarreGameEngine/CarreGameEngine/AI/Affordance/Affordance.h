#pragma once

	/**
	* @class Affordance
	* @brief Affordance class for AI
	*
	*
	* @author Cordell Smith
	* @version 01
	* @date 20/10/18
	*/
#ifndef AFFORDANCE_H
#define AFFORDANCE_H

#include <string>
#include <unordered_map>

class CollisionBody;

typedef std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> AffordanceData;

class Affordance
{
public:
	Affordance(std::string name);
	Affordance(std::string name, float sitOn, float standOn, float kick);
	~Affordance() { }
	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }

	const void InitBaseAffordances(const AffordanceData& affordanceData, std::vector<CollisionBody*>& collisionBodies);

protected:
	/// Affordances and corresponding value
	float m_sitOn, m_standOn, m_kick;
	std::string m_name;
};
#endif // !AFFORDANCE_H


