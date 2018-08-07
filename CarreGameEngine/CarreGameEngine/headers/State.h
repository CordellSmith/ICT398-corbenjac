/**
* @class State
* @brief A template state class
* @author Jack Matters
*
* @date 27/05/2018
* @version 1.0	Initial start. Created the layout of the class.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef STATE_H
#define STATE_H

template <class entity_type>
class State
{
	public:

			/**
			* @brief De-constructor
			*
			* This is the de-constructor
			*/
		//virtual ~State{};

			/**
			* @brief Enter the state
			*
			* Inititializes all data when entering a different state
			*
			* @param entity_type - Type of entity using the StateMachine
			*
			* @return void
			*/
		virtual void Enter(entity_type*) = 0;

			/**
			* @brief Execute the state
			*
			* Main loop of the current state. Gets called every frame
			*
			* @param entity_type - Type of entity using the StateMachine
			*
			* @return void
			*/
		virtual void Execute(entity_type*) = 0;

			/**
			* @brief Exit the state
			*
			* Frees data from this state when changing to another one
			*
			* @param entity_type - Type of entity using the StateMachine
			*
			* @return void
			*/
		virtual void Exit(entity_type*) = 0;

};

#endif