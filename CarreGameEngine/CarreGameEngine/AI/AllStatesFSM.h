/**
* @class AllStatesFSM
* @brief Contains all states (classes) related to the FSM
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef ALLSTATESFSM_H
#define ALLSTATESFSM_H

// Includes
#include "..\Common\Singleton.h"
#include "..\AI\State.h"
#include "..\AI\ComputerAI.h"
#include "..\Controllers\TimeManager.h"

/*****************************************Class Separator******************************************/

class ComputerAI;

/*****************************************Class Separator******************************************/

class MoveState : public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI);
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI);
};

/*****************************************Class Separator******************************************/

class GlobalState : public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI) { std::cout << "Entering Global state!" << std::endl; };
	void Execute(ComputerAI* compAI) {  };
	void Exit(ComputerAI* compAI) {};
};

/*****************************************Class Separator******************************************/

class StartState : public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI) {};
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI) {};
};

/*****************************************Class Separator******************************************/

class EmotionsState : public State <ComputerAI> {
public:
	void Enter(ComputerAI* compAI);
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI);
};

/*****************************************Class Separator******************************************/

class TiredState : public State<ComputerAI>
{
public:
	void Enter(ComputerAI* compAI);
	void Execute(ComputerAI* compAI);
	void Exit(ComputerAI* compAI) {};
};

typedef Singleton<GlobalState> m_globalState;
typedef Singleton<StartState> m_startState;
typedef Singleton<MoveState> m_moveState;
typedef Singleton<TiredState> m_tiredState;
typedef Singleton<EmotionsState> m_EmotionsState;


#endif