#pragma once

#include <string>

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

class Affordance
{
public:
	Affordance(std::string name);
	Affordance(std::string name, float sitOn, float standOn, float kick);
	~Affordance() { }

	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }
protected:
	/// Affordances and corresponding value
	float m_sitOn, m_standOn, m_kick;
	std::string m_name;
};
#endif // !AFFORDANCE_H


