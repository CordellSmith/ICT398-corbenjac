#pragma once

#include <string>
#include <stdio.h> 
#include <stdlib.h>
#include <fstream>															
#include "GLFW\glfw3.h"									
#include "WindowManager.h"								

	/*
	* @class GLFWManager
	* @brief This is our implementation of the abstract base class WindowManager
	*
	* This class uses the GLFW cross-platform library to create a window, handle input and create the OpenGL context
	* by inheriting from the interface class WindowManager.
	*
	* @version 01
	* @date 29/03/2018
	*
	* @version 02
	* @date 31/05/2018
	*/
class GLFWManager : public WindowManager
{
public:

		/**
		* @brief Default constructor
		*
		* Empty constructor.
		*
		* @return null
		*/
	GLFWManager() { }

		/**
		* @brief Default constructor
		*
		* Destructor runs Destroy() function that frees up any used memory.
		*
		* @return null
		*/
	~GLFWManager() { Destroy(); }

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
	virtual int Initialize(int width, int height, std::string strTitle, bool bFullScreen = false) override;

		/**
		* @brief Swaps the buffers
		*
		* This function swaps the backbuffer to the front and is called every frame.
		*
		* @return void
		*/
	virtual void SwapTheBuffers() override;

		/**
		* @brief Processes the input of the user
		*
		* This function processes any needed input for the application such as key
		* presses like esc to close the window.
		*
		* @param bool continueGame
		* @return bool
		*/
	virtual bool ProcessInput(bool continueGame) override;

		/**
		* @brief Destroys the window context
		*
		* This function destroys the window as well as any existing OpenGL contexts.
		*
		* @return void
		*/
	virtual void Destroy();

protected:
	/// GLFW window object that manages the window and input
	GLFWwindow* m_window;
};

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	static_cast<GLFWManager*>(glfwGetWindowUserPointer(window))->GetInputManager()->WheelScrolled(yoffset);
}