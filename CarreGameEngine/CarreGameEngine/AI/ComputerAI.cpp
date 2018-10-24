/*
* Implementation of ComputerAI.h file
* Author - Jack Matters
*/

// Includes
#include "ComputerAI.h"
#include "AllStatesFSM.h"

// Default constructor
ComputerAI::ComputerAI()
{
	this->m_computerAIFSM = new StateMachine<ComputerAI>(this);

	// Possible change
	this->m_computerAIFSM->SetCurrState(&m_startState::GetInstance());
	this->m_computerAIFSM->SetGlobalState(&m_globalState::GetInstance());

	// Read from script
	this->m_currPos = glm::vec3(0, 0, 0);
	this->m_currVel = glm::vec3(0, 0, 0);
	this->m_health = 100;
	this->m_maxHealth = 100;
	this->m_isDead = false;

	//MakeWaypoints();
}

// Default constructor
ComputerAI::ComputerAI(glm::vec3 pos)
{
	this->m_computerAIFSM = new StateMachine<ComputerAI>(this);

	// Possible change
	this->m_computerAIFSM->SetCurrState(&m_startState::GetInstance());
	this->m_computerAIFSM->SetGlobalState(&m_globalState::GetInstance());

	// Read from script
	this->m_currPos = glm::vec3(pos.x, pos.y, pos.z);
	this->m_currVel = glm::vec3(0, 0, 0);
	this->m_health = 100;
	this->m_maxHealth = 100;
	this->m_isDead = false;

	//MakeWaypoints();
}

// De-constructor
ComputerAI::~ComputerAI()
{
	delete m_computerAIFSM;
}

// Add health
void ComputerAI::AddHealth(int health)
{
	this->m_health += health;

	// Don't go over max health
	if (this->m_health > this->m_maxHealth)
		this->m_health = this->m_maxHealth;
}

// Decrease health
void ComputerAI::DecreaseHealth(int health)
{
	this->m_health -= health;

	// Check to see if dead
	if (this->m_health < 0)
	{
		this->m_health = 0;
		m_isDead = true;
	}
}

// Return health
int ComputerAI::GetHealth()
{
	return this->m_health;
}

// Return if dead or not
bool ComputerAI::IsDead()
{
	return m_isDead == true;
}

// Update the FSM
void ComputerAI::Update()
{
	m_computerAIFSM->Update();
}

// Set current velocity
void ComputerAI::SetVelocity(glm::vec3 vel)
{
	this->m_currVel = vel;
}

// Get current velocity
glm::vec3 ComputerAI::GetVelocity()
{
	return m_currVel;
}

// Set current position
void ComputerAI::SetPosition(glm::vec3 pos)
{
	this->m_currPos = pos;
}

// Return current position
glm::vec3 ComputerAI::GetPosition()
{
	return m_currPos;
}

// Move to a location
bool ComputerAI::MoveTo(ComputerAI* compAI, glm::vec3 targetPos)
{
	//glm::vec3 targetPos(1000, 1000);
	glm::vec3 currVel = compAI->GetVelocity();
	glm::vec3 currPos = compAI->GetPosition();

	// Calcute heading from this position to target position
	glm::vec3 toTarget2 = targetPos - currPos;
	glm::vec3 toTarget = glm::normalize(toTarget2);
	if ((toTarget2.x < 10 && toTarget2.x > -10) && (toTarget2.z < 10 && toTarget2.z > -10))
	{
		//std::cout << "here 0" << std::endl;
		compAI->SetVelocity(glm::vec3(0, 0, 0));
		return true;
	}

	// Calculate new velocity and new position
	currVel = toTarget * glm::length(currVel);
	compAI->SetVelocity(currVel);
	glm::vec3 displacement = currVel * 0.05f;
	glm::vec3 newPos = currPos + displacement;

	// Calculate real target position
	glm::vec3 realTargetPos = targetPos - (toTarget * 0.001f);

	// Calculate the direction from newPos to realTargetPos
	glm::vec3 toRealTarget = realTargetPos - newPos;
	glm::vec3 toRealTarget2 = glm::normalize(toRealTarget);
	if (toRealTarget2.x == 0 && toRealTarget2.z == 0)
	{
		//std::cout << "here 1" << std::endl;
		currPos = realTargetPos;
		compAI->SetPosition(currPos);
		compAI->SetVelocity(glm::vec3(0, 0, 0));
		return true;
	}
	
	//std::cout << toRealTarget2 << " : " << toTarget << std::endl;

	//// Check to see whether newPos has passed the realTargetPos
	//float dp = toRealTarget2.Dot(toRealTarget2, toTarget);
	//if (dp < 0.0)
	//{
	//	std::cout << "here 2" << std::endl;
	//	currPos = realTargetPos;
	//	compAI->SetPosition(currPos);
	//	compAI->SetVelocity(glm::vec3(0, 0));
	//	return true;
	//}

	// newPos has not yet passed realTargetPos
	currPos = newPos;
	compAI->SetPosition(currPos);
	return false;
}

std::vector<glm::vec3> ComputerAI::MakeWaypoints()
{
	glm::vec3 waypoint;

	//srand(time(NULL));

	//for (int i = 0; i < 5; i++)
	//{
	//	waypoint = glm::vec3(rand() % 2000 + -1000, rand() % 2000 + -1000);
	//	this->m_waypoints.push_back(waypoint);

	//	//std::cout << waypoint << std::endl;
	//}

	waypoint = glm::vec3(5387, 0, 4262);
	this->m_waypoints.push_back(waypoint);
	waypoint = glm::vec3(8018, 0, 4262);
	this->m_waypoints.push_back(waypoint);
	waypoint = glm::vec3(8018, 0, 7155);
	this->m_waypoints.push_back(waypoint);
	waypoint = glm::vec3(5387, 0, 7155);
	this->m_waypoints.push_back(waypoint);
	return this->m_waypoints;
}