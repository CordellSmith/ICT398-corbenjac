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
	std::cout << "Entering 'Move' state!" << std::endl;
}

void MoveState::Execute(ComputerAI* compAI)
{
	// If no velocity, set to walking and pick a waypoint
	glm::vec3 tempVel = compAI->GetVelocity();
	if (tempVel.x == 0 && tempVel.z == 0)
	{
		compAI->SetVelocity(glm::vec3(50, 0, 0));

		//srand(time(NULL));
		int pos = rand() % compAI->GetWaypoints().size();
		//std::cout << pos << std::endl;
		compAI->SetTargetWaypoint(pos);

		//std::cout << currTargetPos << std::endl;
	}

	compAI->MoveTo(compAI, compAI->GetTargetWaypoint());

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
}

/*****************************************Class Separator******************************************/

void StartState::Execute(ComputerAI* compAI)
{
	compAI->GetFSM()->ChangeState(&m_EmotionsState::GetInstance());
}

/*****************************************Class Separator******************************************/

void EmotionsState::Enter(ComputerAI* compAI) {

}

void EmotionsState::Execute(ComputerAI* compAI) {
	glm::vec3 tempVel = compAI->GetVelocity();
	
	if (tempVel.x == 0 && tempVel.z == 0) //choose a new waypoint
	{
		compAI->SetVelocity(glm::vec3(50, 0, 0));
		int pos = rand() % compAI->GetWaypoints().size();
		compAI->SetTargetWaypoint(pos);
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
		compAI->SetTargetWaypoint(pos);
	}

	compAI->MoveTo(compAI, compAI->GetTargetWaypoint());
}

void EmotionsState::Exit(ComputerAI* compAI) {

}