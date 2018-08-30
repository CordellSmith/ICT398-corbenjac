#pragma once

#include <string>
#include <fstream>								
#include "..\Input\InputManager.h"

	/**
	* @class WindowManager
	* @brief Interface class to manage the creation of the window 
	* 
	* This class handles input and the creation of the OpenGL context implementing GLFW.
	*
	* @version 01
	* @date 29/03/2018
	*
	* @version 02
	* @date 31/05/2018
	*/
class WindowManager
{
public:
		/**
		* @brief Destructor
		*
		* This function is required for the inheriting class to have its destructor called.
		*
		* @return null
		*/
	virtual ~WindowManager() { }

		/**
		* @brief Initializes the window
		*
		* This function initializes the window taking in its dimensions and creating a new OpenGL
		* context.
		*
		* @param int width
		* @param int height
		* @param std::string strTitle
		* @param bool bFullScreen
		* @return int
		*/
	virtual int Initialize(int width, int height, std::string strTitle, bool bFullScreen = false) = 0;

		/**
		* @brief Swaps the buffers
		*
		* This function swaps the backbuffer to the front and is called every frame.
		*
		* @return void
		*/
	virtual void SwapTheBuffers() = 0;

		/**
		* @brief Processes the input of the user
		*
		* This function processes any needed input for the application such as key
		* presses like esc to close the window.
		*
		* @param bool continueGame
		* @return bool
		*/
	virtual bool ProcessInput(bool continueGame) = 0;

		/**
		* @brief Destroys the window context
		*
		* This function destroys the window as well as any existing OpenGL contexts.
		*
		* @return void
		*/
	virtual void Destroy() = 0;

		/**
		* @brief Returns the input manager object
		*
		* This function is a getter that retrurns a pointer to the InputManager object
		* in order to move the camera.
		*
		* @return InputManager*
		*/
	virtual InputManager* GetInputManager() { return &m_inputManager; }
	
protected:
	/// Used to control the camera movements
	InputManager m_inputManager;
};