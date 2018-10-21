#include "Affordance.h"

Affordance::Affordance()
{
	m_sitOn = 0.0f;
	m_standOn = 0.0f;
	m_kick = 0.0f;
}

Affordance::Affordance(float sitOn, float standOn, float kick)
{
	m_sitOn = sitOn;
	m_standOn = standOn;
	m_kick = kick;
}
