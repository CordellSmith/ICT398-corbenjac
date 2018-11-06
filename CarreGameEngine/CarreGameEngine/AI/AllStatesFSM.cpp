/*
* Implementation of AllStatesFSM.h file
* Author - Jack Matters
*/

// Includes
#include <iostream>
#include "../AI/AllStatesFSM.h"
#include "..\Physics\PhysicsEngine.h"

/*****************************************Class Separator******************************************/

void MoveState::Enter(ComputerAI* compAI)
{
	std::cout << "Entering 'Move' state!" << std::endl;
}

void MoveState::Execute(ComputerAI* compAI)
{
	// If energy low, change state
	if (compAI->GetEnergy() < 30.0f)
	{
		compAI->GetFSM()->ChangeState(&m_tiredState::GetInstance());
	}

	// If no velocity, set to walking and pick a waypoint
	glm::vec3 tempVel = compAI->GetVelocity();
	if (compAI->GetVelocity() == glm::vec3(0))
	{
		compAI->SetVelocity(glm::vec3(50, 0, 0));

		//srand(time(NULL));
		int pos = rand() % compAI->GetWaypoints().size();
		//std::cout << pos << std::endl;
		compAI->SetMoveToTarget(compAI->GetWaypoints()[pos]);
		//std::cout << currTargetPos << std::endl;
	}

	compAI->MoveTo(compAI, compAI->GetMoveToTarget());
}

void MoveState::Exit(ComputerAI* compAI)
{
	std::cout << "Exiting 'Move' state!" << std::endl;
}

/*****************************************Class Separator******************************************/

void StartState::Execute(ComputerAI* compAI)
{
	std::cout << "Entering 'Start' state!" << std::endl;
	compAI->GetFSM()->ChangeState(&m_moveState::GetInstance());
}

/*****************************************Class Separator******************************************/

void EmotionsState::Enter(ComputerAI* compAI) {
}

void EmotionsState::Execute(ComputerAI* compAI) {
	
	glm::vec3 tempVel = compAI->GetVelocity();
	if (tempVel.x == 0 && tempVel.z == 0){
		compAI->SetVelocity(glm::vec3(50, 0, 0));
		int pos = rand() % compAI->GetWaypoints().size();
		compAI->SetMoveToTarget(compAI->GetWaypoints()[pos]);
	}

	//if happy or fearful move faster
	if (compAI->GetEmotionalState().isFear() || compAI->GetEmotionalState().isJoy()) {
		compAI->SetVelocity(glm::vec3(75, 0, 0));
	}
	//if amgry/sad/anticipating, move slower
	if (compAI->GetEmotionalState().isAnger() || compAI->GetEmotionalState().isSad() || compAI->GetEmotionalState().isAnticipation()) {
		compAI->SetVelocity(glm::vec3(25, 0, 0));
	}
	//if surprised have a 25% chance to change directions
	if (compAI->GetEmotionalState().isSurprise() && (rand() % 100 > 75)) { 
		int pos = rand() % compAI->GetWaypoints().size();
		compAI->SetMoveToTarget(compAI->GetWaypoints()[pos]);
	}

	compAI->MoveTo(compAI, compAI->GetMoveToTarget());
}

void EmotionsState::Exit(ComputerAI* compAI) {

}

/*****************************************Class Separator******************************************/

void TiredState::Enter(ComputerAI* compAI)
{
	std::cout << "Entering 'Tired' state!" << std::endl;

	glm::vec3 targetPos = compAI->LocateObject();
	compAI->SetMoveToTarget(compAI->GetFocusObj()->m_position);
	compAI->SetHasArrived(false);
}

void TiredState::Execute(ComputerAI* compAI)
{
	if (compAI->HasArrived())
	{
		// Reduce affordance of the object
		compAI->GetFocusObj()->m_affordance->SetSitOn(compAI->GetFocusObj()->m_affordance->GetSitOn() - 50);
		compAI->GetFocusObj()->m_affordance->SetStandOn(compAI->GetFocusObj()->m_affordance->GetStandOn() - 50);

		glm::vec3 temp = compAI->GetPosition();
		compAI->SetPosition(glm::vec3(temp.x, compAI->GetMoveToTarget().y + 225, temp.z));

		compAI->SetEnergy(compAI->GetEnergy() + 0.008);
	}
	else
	{
		// Move towards object
		compAI->MoveTo(compAI, compAI->GetMoveToTarget());
	}

	if (float newEnergy = compAI->GetEnergy() > 99)
	{
		// Reset the affordance of the object
		compAI->GetFocusObj()->m_affordance->SetSitOn(compAI->GetFocusObj()->m_affordance->GetSitOn() + 50);
		compAI->GetFocusObj()->m_affordance->SetStandOn(compAI->GetFocusObj()->m_affordance->GetStandOn() + 50);

		compAI->SetHasArrived(false);
		compAI->GetFSM()->ChangeState(&m_moveState::GetInstance());
	}
}