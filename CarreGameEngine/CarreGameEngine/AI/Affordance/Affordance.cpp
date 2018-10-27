#include "Affordance.h"

Affordance::Affordance(std::string name)
{
	m_name = name;
	m_sitOn = 0.0f;
	m_standOn = 0.0f;
	m_kick = 0.0f;
}

Affordance::Affordance(std::string name, float sitOn, float standOn, float kick)
{
	m_name = name;
	m_sitOn = sitOn;
	m_standOn = standOn;
	m_kick = kick;
}
