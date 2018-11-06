#include "Affordance.h"
#include "..\..\Physics\PhysicsEngine.h"

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

const void Affordance::InitBaseAffordances(const AffordanceData& affordanceData, std::vector<CollisionBody*>& collisionBodies)
{
	AffordanceData::const_iterator itr = affordanceData.begin();

	while (itr != affordanceData.end())
	{
		// Iterate through all collision bodies
		for (size_t i = 0; i < collisionBodies.size(); i++)
		{
			// Compare the collision body name to the affordance base value name (eg. table == table)
			if (collisionBodies.at(i)->m_modelName == itr->first)
			{
				// Pass the value to the collision body from the map read in from script
				collisionBodies.at(i)->m_affordance->m_sitOn = itr->second.at(0).second;
				collisionBodies.at(i)->m_affordance->m_standOn = itr->second.at(1).second;
				collisionBodies.at(i)->m_affordance->m_kick = itr->second.at(2).second;
			}
		}
		itr++;
	}
}

