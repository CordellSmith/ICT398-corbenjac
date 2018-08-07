#include "Player.h"

Player::Player(std::string playerName)
{
	m_name = playerName;
	m_playerModel = new Model();

	m_currentMoveSpeed = 0;
	m_currentTurnSpeed = 0;
	m_moveSpeed = 30;
	m_turnSpeed = 0.1;
}

void Player::LoadFromFilePath(std::string filePath)
{
	m_playerModel->LoadModel(filePath);
}

const void Player::Destroy()
{
	return void();
}

void Player::MoveForward(float time)
{
	m_currentMoveSpeed = m_moveSpeed * time * 15;
	float dx = m_currentMoveSpeed * glm::sin(m_playerModel->GetRotation().y);
	float dz = m_currentMoveSpeed * glm::cos(m_playerModel->GetRotation().y);

	glm::vec3 temp = m_playerModel->GetPosition() + glm::vec3(dx, 0, dz);
	m_playerModel->SetPosition(temp);
}

void Player::MoveBackward(float time)
{
	m_currentMoveSpeed = -m_moveSpeed * time * 15;
	float dx = m_currentMoveSpeed * glm::sin(m_playerModel->GetRotation().y);
	float dz = m_currentMoveSpeed * glm::cos(m_playerModel->GetRotation().y);

	glm::vec3 temp = m_playerModel->GetPosition() + glm::vec3(dx, 0, dz);
	m_playerModel->SetPosition(temp);
}

void Player::TurnClock(float time)
{
	m_currentTurnSpeed = -m_turnSpeed * time * 10;
	m_playerModel->SetRotation(glm::vec3(0, m_playerModel->GetRotation().y + m_currentTurnSpeed, 0));
}

void Player::TurnAntiClock(float time)
{
	m_currentTurnSpeed = m_turnSpeed * time * 10;
	m_playerModel->SetRotation(glm::vec3(0, m_playerModel->GetRotation().y + m_currentTurnSpeed, 0));
}