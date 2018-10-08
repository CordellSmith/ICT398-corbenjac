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

void Player::ThrowBall(float time, glm::vec3& forward)
{
	btVector3 camPos = btVector3(m_playerModel->GetPosition().x, m_playerModel->GetPosition().y, m_playerModel->GetPosition().z);
	//m_collisionBodyPos->push_back(btVector3(forward.x, forward.y, forward.z));
	// Get camera lookAt vector
	glm::vec3 look = m_playerModel->GetCamera()->GetView() * 100.0f;
	// Create new sphere rigid body 
	btRigidBody* sphere = m_physicsWorld->AddSphere(10.0, camPos);
	// Add to rigid bodies array
	m_physicsWorld->GetRigidBodies().push_back(sphere);
	// Add linear velocity to the sphere
	sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
	std::cout << "THROW" << std::endl;
	std::cout << "Collision Body Pos Size: " << m_collisionBodyPos->size() << std::endl;
}