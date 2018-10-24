/*
* Implementation of AllStatesFSM.h file
* Author - Jack Matters
*/

// Includes
#include <iostream>
#include "../AI/AllStatesFSM.h"

/*****************************************Class Separator******************************************/

void MoveState::Enter(ComputerAI* compAI)
{
	std::cout << "Entering 'Enter' state!" << std::endl;

	isMoving = false;
	m_waypoints = compAI->GetWaypoints();
	currTargetPos = m_waypoints[0];
}

void MoveState::Execute(ComputerAI* compAI)
{
	// If no velocity, set to walking and pick a waypoint
	glm::vec3 tempVel = compAI->GetVelocity();
	if (tempVel.x == 0 && tempVel.z == 0)
	{
		compAI->SetVelocity(glm::vec3(50, 0, 0));

		//srand(time(NULL));
		int pos = rand() % m_waypoints.size();
		//std::cout << pos << std::endl;
		currTargetPos = m_waypoints[pos];

		//std::cout << currTargetPos << std::endl;
	}

	compAI->MoveTo(compAI, currTargetPos);

	// Change to idle state if player is too close
	//if ()
	//{
	//	compAI->GetFSM()->ChangeState(&m_idleState::GetInstance());
	//}
	
	//std::cout << compAI->GetVelocity() << std::endl;
	//std::cout << compAI->GetPosition() << std::endl;
}

void MoveState::Exit(ComputerAI* compAI)
{
	std::cout << "Entering 'Exit' state!" << std::endl;
	isMoving = false;
	m_waypoints.clear();
}

/*****************************************Class Separator******************************************/

void StartState::Execute(ComputerAI* compAI)
{
	compAI->GetFSM()->ChangeState(&m_moveState::GetInstance());
}

/*****************************************Class Separator******************************************/

void IdleState::Enter(ComputerAI* compAI)
{
	compAI->SetVelocity(glm::vec3(0));
}

void IdleState::Execute(ComputerAI* compAI)
{

	// Change to move state if player moves away
	//if ()
	//{
	//	compAI->GetFSM()->ChangeState(&m_moveState::GetInstance());
	//}
}