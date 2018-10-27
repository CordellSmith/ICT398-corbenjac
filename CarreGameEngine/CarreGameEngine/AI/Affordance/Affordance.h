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

class Affordance
{
public:
	Affordance();
	Affordance(float sitOn, float standOn, float kick);
	~Affordance() { }

protected:
	/// Affordances and corresponding value
	float m_sitOn, m_standOn, m_kick;
};
#endif // !AFFORDANCE_H


