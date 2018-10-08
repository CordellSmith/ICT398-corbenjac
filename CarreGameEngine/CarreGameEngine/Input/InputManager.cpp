#include "InputManager.h"
#include "..\Controllers\TimeManager.h"

void InputManager::KeyPressed(InputCodes code)
{
	// Validate camera
	if (m_camera == nullptr)
		return;

	switch (code)
	{
		case Space:
			m_player->ThrowBall((float)(TimeManager::Instance().DeltaTime), glm::normalize(m_camera->GetView()));
			break;
		// Move forwards
		case Up: case W: case w:
			m_player->MoveForward((float)(TimeManager::Instance().DeltaTime), glm::normalize(m_camera->GetView()));
			break;
		// Move backwards
		case Down: case S: case s:
			m_player->MoveBackward((float)(TimeManager::Instance().DeltaTime), glm::normalize(m_camera->GetView()));
			break;
		case Left: case A: case a:
			m_player->StrafeLeft((float)(TimeManager::Instance().DeltaTime), glm::normalize(glm::cross(m_camera->GetUp(), m_camera->GetView())));
			break;
		case Right: case D: case d:
			m_player->StrafeRight((float)(TimeManager::Instance().DeltaTime), glm::normalize(glm::cross(m_camera->GetUp(), m_camera->GetView())));
			break;
		case q: case Q:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 101: case E:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
	}
}

void InputManager::MouseMove(float mouseX, float mouseY)
{
	if (m_camera == nullptr)
		return;
	m_camera->ChangeYaw(mouseX);
	m_camera->ChangePitch(mouseY);
}

void InputManager::WheelScrolled(double yoffset)
{
	if (m_camera == nullptr)
		return;

	m_camera->Zoom(yoffset);
}