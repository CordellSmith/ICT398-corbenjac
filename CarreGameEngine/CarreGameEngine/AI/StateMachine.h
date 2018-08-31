/**
* @class StateMachine
* @brief A template FSM class
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class, using a template design.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

// Includes
#include <iostream>
#include "State.h"
#include "..\Common\Structs.h"

template <class entity_type>
class StateMachine
{
public:

		/**
		* @brief Non-default constructor
		*
		* The non-default constructor
		*
		* @param FSMowner - Owner of the FSM
		*/
	StateMachine(entity_type* FSMowner)
	{
		m_owner = FSMowner;
		m_prevState = NULL;
		m_currState = NULL;
		m_globalState = NULL;
	}

		/**
		* @brief Set previous state
		*
		* Sets the previous state
		*
		* @param state - The state to set
		*
		* @return void
		*/
	void SetPrevState(State<entity_type>* state) { m_prevState = state; }

		/**
		* @brief Set current state
		*
		* Sets the current state
		*
		* @param state - The state to set
		*
		* @return void
		*/
	void SetCurrState(State<entity_type>* state) { m_currState = state; }

		/**
		* @brief Set global state
		*
		* Sets the global state
		*
		* @param state - The state to set
		*
		* @return void
		*/
	void SetGlobalState(State<entity_type>* state) { m_globalState = state; }

		/**
		* @brief Updates the state
		*
		* Execute the current state
		*
		* @return void
		*/
	void Update() const
	{
		if (m_globalState)
			m_globalState->Execute(m_owner);
		if (m_currState)
			m_currState->Execute(m_owner);
	}

		/**
		* @brief Change state
		*
		* Changes previous state to current state, and current state to newState
		*
		* @param newState - New state to set
		*
		* @return void
		*/
	void ChangeState(State<entity_type>* newState)
	{
		m_prevState = m_currState;
		m_currState->Exit(m_owner);
		m_currState = newState;
		m_currState->Enter(m_owner);
	}

		/**
		* @brief Revert to previous state
		*
		* Reverts to the previous state
		*
		* @return void
		*/
	void RevertToPrevState() { m_currState = m_prevState; }

		/**
		* @brief Accessor method
		*
		* Access and return the previous state
		*
		* @return State<entity_type> - Previous state
		*/
	State<entity_type>* GetPrevState() { return m_prevState; }

		/**
		* @brief Accessor method
		*
		* Access and return the current state
		*
		* @return State<entity_type> - Current state
		*/
	State<entity_type>* GetCurrState() { return m_currState; }

		/**
		* @brief Accessor method
		*
		* Access and return the global state
		*
		* @return State<entity_type> - Global state
		*/
	State<entity_type>* GetGlobalState() { return m_globalState; }

		/**
		* @brief Compare a state to current state
		*
		* Returns true if current state's type is equal to the type of class passed as parameter
		*
		* @param state - State to compare
		*
		* @return bool - True if state types match, false otherwise
		*/
	bool IsInState(const State<entity_type>& state) const { return typeid(*m_currState) == typeid(state); }

		/**
		* @brief De-constructor
		*
		* Virtual de-constructor
		*/
	//virtual ~StateMachine() {}

private:

	/// Owner of the FSM
	entity_type* m_owner;

	/// Previous state
	State<entity_type> *m_prevState;

	/// Current state
	State<entity_type> *m_currState;

	/// Global state
	State<entity_type> *m_globalState;
};

#endif