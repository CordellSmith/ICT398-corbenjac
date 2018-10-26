#include "GameWorld.h"

// Helper function
glm::vec3 BttoGlm(const btVector3& btVector)
{
	return glm::vec3(btVector.getX(), btVector.getY(), btVector.getZ());
}

void GameWorld::Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);

	// Each new entity, asset needs its own shader (terrain or anything with multiple meshes especially)
	ShaderSource mainShader = ParseShaders("Resources/shaders/Default.shader");
	ShaderSource terrainShader = ParseShaders("Resources/shaders/Terrain.shader");

	// Prepare player
	m_player = player;
	m_player->SetCamera(m_camera);
	m_glRenderer.Prepare(m_player->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);

	// Pass player info to camera
	m_camera->ParsePlayerInfo(m_player->GetPosition(), m_player->GetRotation());
	float horizontalDistance = m_camera->CalculateHorizontalDistance();
	float verticalDistance = m_camera->CalculateVerticalDistance();
	m_camera->CalculateCameraPosition(horizontalDistance, verticalDistance);
	
	// Prepare terrains
	for each (Bruteforce* terrain in m_terrains)
	{
		terrain->SetCamera(m_camera);
		m_glRenderer.Prepare(terrain->GetModel(), terrainShader.VertexSource, terrainShader.FragmentSource);
	}
	
	// Prepare assets
	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->SetCamera(m_camera);
		m_glRenderer.Prepare(itr->second->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);
	}
}

void GameWorld::Update()
{
	// Camera
	m_camera->ParsePlayerInfo(m_player->GetPosition(), m_player->GetRotation());
	float horizontalDistance = m_camera->CalculateHorizontalDistance();
	float verticalDistance = m_camera->CalculateVerticalDistance();
	m_camera->CalculateCameraPosition(horizontalDistance, verticalDistance);

	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Render player
	//m_glRenderer.Render(m_player->GetModel());

	/// Debug draw
	//m_physicsWorld->DebugDraw();

	// Update all physics body locations *** All asset rendering is done through here for now because I dont want to have to call asset render twice ***
	UpdatePhysics();
}

void GameWorld::Destroy()
{
	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Destroy();
	}

	// Delete all heightmap BruteForce
	for (int i = 0; i < m_terrains.size()-1; i++)
		delete m_terrains[i];
}

void GameWorld::SetPhysicsWorld(PhysicsEngine* physicsEngine, std::vector<CollisionBody*>& collisionBodies)
{
	m_physicsWorld = physicsEngine;
	m_collisionBodies = &collisionBodies;
}

// Update all physics
void GameWorld::UpdatePhysics()
{
	// Update physicsWorld
	// TODO: Make this better (Jack)
	
	btVector3 bt_playerPos(m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	
	// Set updated camera location
	//m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z));
		
	m_physicsWorld->Simulate(*m_collisionBodies, bt_playerPos);
	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	ComputerAI* compAI;

	// Loop through all the rigid bodies to update their position
	for (size_t i = 0; i < m_collisionBodies->size(); i++)
	{
		glm::vec3 updPosition = BttoGlm(m_collisionBodies->at(i)->m_position);
		glm::vec3 updRotation = BttoGlm(m_collisionBodies->at(i)->m_rotation);

		// Search through map using find. If found, update that objects position and rotation
		m_gameAssets.find(m_collisionBodies->at(i)->m_modelName)->second->GetModel()->SetPosition(updPosition);
		m_gameAssets.find(m_collisionBodies->at(i)->m_modelName)->second->GetModel()->SetRotation(updRotation);
		// Search through map using find. If found, render the object
		m_glRenderer.Render(m_gameAssets.find(m_collisionBodies->at(i)->m_modelName)->second->GetModel());

		//if (m_collisionBodies->at(i)->m_name == "AI 1")
		//	printf("AI 1 <X %d><Y %d><Z %d>\n", updPosition.x, updPosition.y, updPosition.z);
		//if (m_collisionBodies->at(i)->m_name == "AI 2")																			 
		//	printf("AI 2 <X %d><Y %d><Z %d>\n", updPosition.x, updPosition.y, updPosition.z);
		//if (m_collisionBodies->at(i)->m_name == "AI 3")																		
		//	printf("AI 3 <X %d><Y %d><Z %d>\n", updPosition.x, updPosition.y, updPosition.z);
	}

	// Ray casting
	glm::vec3 camDirection = m_camera->GetView() * 10000.0f;
	btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(
		m_camera->GetPosition().x, 
		m_camera->GetPosition().y, 
		m_camera->GetPosition().z), 
		btVector3(camDirection.x, 
			camDirection.y, 
			camDirection.z));
	m_physicsWorld->GetDynamicsWorld()->rayTest(btVector3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z), btVector3(camDirection.x, camDirection.y, camDirection.z), rayCallback);

	if (rayCallback.hasHit())
	{
		//std::cout << rayCallback.m_collisionObject->getCollisionShape()->getName() << std::endl;
	}

	m_player->SetPosition(glm::vec3(bt_playerPos.getX(), bt_playerPos.getY(), bt_playerPos.getZ()));
	//std::cout << m_player->GetPosition().x << " " << m_player->GetPosition().y << " " << m_player->GetPosition().z << std::endl;
}