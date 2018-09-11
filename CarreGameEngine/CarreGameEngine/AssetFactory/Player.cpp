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

void Player::MoveForward(float time, glm::vec3& forward)
{
	m_currentMoveSpeed = m_moveSpeed * time * 15;
	//float dx = m_currentMoveSpeed * glm::sin(cameraRot);
	//float dz = m_currentMoveSpeed * glm::cos(cameraRot);

	glm::vec3 temp = m_playerModel->GetPosition() + (m_currentMoveSpeed * forward);
	m_playerModel->SetPosition(temp);
}

void Player::MoveBackward(float time, glm::vec3& forward)
{
	m_currentMoveSpeed = -m_moveSpeed * time * 15;
	//float dx = m_currentMoveSpeed * glm::sin(cameraRot);
	//float dz = m_currentMoveSpeed * glm::cos(cameraRot);

	glm::vec3 temp = m_playerModel->GetPosition() + (m_currentMoveSpeed * forward);
	m_playerModel->SetPosition(temp);
}

void Player::StrafeLeft(float time, glm::vec3& left) {
	m_currentMoveSpeed = m_moveSpeed * time * 15;

	glm::vec3 temp = m_playerModel->GetPosition() + (m_currentMoveSpeed * left);
	m_playerModel->SetPosition(temp);
}

void Player::StrafeRight(float time, glm::vec3& left) {
	m_currentMoveSpeed = -m_moveSpeed * time * 15;

	glm::vec3 temp = m_playerModel->GetPosition() + (m_currentMoveSpeed * left);
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