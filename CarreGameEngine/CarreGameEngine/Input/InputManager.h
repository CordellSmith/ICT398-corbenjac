#pragma once

/// Make sure GLM is using radians instead of degrees
#define GLM_FORCE_RADIANS								

#include <GL\glew.h>
/// Used for the GLM math library
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

#include "..\Controllers\Camera.h"
#include "..\AssetFactory\Player.h"

/**
* @brief Enumerator for each key value
*
* This is a standardized enum to code for keyboard input.
*
* @return null
*/
enum InputCodes
{
	MouseButtonLeft = 0,
	MouseButtonRight = 1,
	kEscape = 27,
	Space = 32,
	Left = 37,
	Up = 38,
	Right = 39,
	Down = 40,
	a = 97,  A = 65,
	b = 98,  B = 66,
	c = 99,  C = 67,
	d = 100, D = 68,
	e = 101, E = 69,
	f = 102, F = 70,
	g = 103, G = 71,
	h = 104, H = 72,
	i = 105, I = 73,
	j = 106, J = 74,
	k = 107, K = 75,
	l = 108, L = 76,
	m = 109, M = 77,
	n = 110, N = 78,
	o = 111, O = 79,
	p = 112, P = 80,
	q = 113, Q = 81,
	r = 114, R = 82,
	s = 115, S = 83,
	t = 116, T = 84,
	u = 117, U = 85,
	v = 118, V = 86,
	w = 119, W = 87,
	x = 120, X = 88,
	y = 121, Y = 89,
	z = 122, Z = 90,
};

	/*
	* @class InputManager
	* @brief Handles user input
	* 
	* This manager takes care of any input from the user and sent to the camera.
	*
	* @version 01
	* @date 29/03/2018
	* @version 02
	* @date 31/05/2018
	* @author Ben Ward
	* @version 03 - made it suitable for a first person camera.
	* @date 11/09/2018
	*/
class InputManager
{
public:
		/**
		* @brief Enumerator for each key value
		*
		* This takes in a code and depending on the value moves the camera or switches
		* to wireframe mode.
		*
		* @param InputCodes code
		* @return void
		*/
	void KeyPressed(InputCodes code);

		/**
		* @brief Sets the camera
		*
		* Sets the camera to the parameter Camera given.
		*
		* @return void
		*/
	void SetCamera(Camera* camera) { m_camera = camera; }

		/**
		* @brief Gets the camera
		*
		* Returns the current camera object.
		*
		* @return Camera*
		*/
	Camera* GetCamera() { return m_camera; }
		
	void MouseMove(float mouseX, float mouseY);

	void WheelScrolled(double offsetz);

	void SetPlayer(Player* player) { m_player = player; }
		
protected:
	/// Camera object
	Camera* m_camera;
	Player* m_player;
	bool m_wireframe = false;
};