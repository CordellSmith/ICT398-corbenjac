#include "Player.h"

Player::Player(std::string playerName)
{
	m_name = playerName;
	m_playerModel = new Model();

	m_currentMoveSpeed = 0;
	m_currentTurnSpeed = 0;
	m_moveSpeed = 150;
	m_turnSpeed = 0.1;
	count = 0;
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

void Player::ThrowBall(float time, Camera* cam)
{
	// Get camera position, rotation and lookAt vector
	glm::vec3 camPos = glm::vec3(m_playerModel->GetPosition().x, m_playerModel->GetPosition().y, m_playerModel->GetPosition().z);
	glm::vec3 camRot = glm::vec3(m_playerModel->GetRotation().x, m_playerModel->GetRotation().y, m_playerModel->GetRotation().z);
	glm::vec3 look = m_playerModel->GetCamera()->GetView() * 1000.0f;
	
	Affordance* affordance = new Affordance("ball");
	CollisionBody* colBody = new CollisionBody("projectile", "ball", camPos, camRot, affordance);

	// Add crates sphere shape rigid body
	//btRigidBody* sphere = m_physicsWorld->AddSphere(110.0, camPos, "ball");
	//btRigidBody* sphere = m_physicsWorld->AddSphere(110.0, camPos, colBody);
	m_physicsWorld->AddSphere(110.0, camPos, colBody, look / 10000.0f, "ball");
	//m_physicsWorld->CreateDynamicRigidBody(camPos, glm::vec3(500, 500, 500), colBody, "ball");
	// Add linear velocity to the sphere
	//sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
	count++;
	// Add to our array of collision bodies
	m_collisionBodies->push_back(colBody);
}