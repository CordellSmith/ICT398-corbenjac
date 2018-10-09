#include "GameWorld.h"

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

	m_camera->SetPosition(glm::vec3(glm::vec3(14100, 500, 10100)));
	
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

	// AI Testing
	//ComputerAI p;
	//for (int i = 0; i < 1000; i++)
	//	p.Update();
	//getchar();
	
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
	m_glRenderer.Render(m_player->GetModel());

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

void GameWorld::SetPhysicsWorld(PhysicsEngine* physicsEngine, std::vector<btVector3> collisionBodyPositions)
{
	m_physicsWorld = physicsEngine;
	m_collisionBodyPos = collisionBodyPositions;
}

// Update all physics
void GameWorld::UpdatePhysics()
{
	// Update physicsWorld
	// TODO: Make this better (Jack)
	
	btVector3 bt_playerPos(m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	
	// Set updated camera location
	//m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z));
		
	m_physicsWorld->Simulate(m_collisionBodyPos, bt_playerPos);
	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	// i is set to 1 because the camera is collision body 0
	int i = 1;

	ComputerAI* compAI;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		// Break out if collision body position vectoris greater than assets map

		// The collisionBodyPos array stores the position vector of all the objects in the scene.
		// They are updated through the Simulation above, then called again here to update and render. The map calls in an alphabetical 
		// ordering system { chair, crate, lecTheatre, player, table }
			   
		glm::vec3 updPosition = glm::vec3(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].y(), m_collisionBodyPos[i].z());
	
		compAI = itr->second->GetAI();
		if (compAI != NULL)
		{
			compAI->Update();

			Vector2 tempPos = compAI->GetPosition();
			//itr->second->SetPosition(glm::vec3(tempPos.x, temp.y, tempPos.z));
		}

		if (itr->first == "lecTheatre")
		{
			itr->second->GetModel()->SetPosition(updPosition);
			m_glRenderer.Render(itr->second->GetModel());
		}

		if (itr->first == "table")
		{
			itr->second->GetModel()->SetPosition(updPosition);
			m_glRenderer.Render(itr->second->GetModel());
		}

		if (itr->first == "chair")
		{
			itr->second->GetModel()->SetPosition(updPosition);
			m_glRenderer.Render(itr->second->GetModel());
		}

		if (itr->first == "crate")
		{
			itr->second->GetModel()->SetPosition(updPosition);
			m_glRenderer.Render(itr->second->GetModel());
		}

		if (itr->first == "ball")
		{
			itr->second->GetModel()->SetPosition(updPosition);
			m_glRenderer.Render(itr->second->GetModel());
		}
		i++;

		//std::cout << "i: " << i << " obj: " << itr->first << std::endl;
		std::cout << "collision body size: " << m_collisionBodyPos.size() << std::endl;
	}

	// Ray casting
	glm::vec3 camDirection = m_camera->GetView() * 10000.0f;
	btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z), btVector3(camDirection.x, camDirection.y, camDirection.z));
	m_physicsWorld->GetDynamicsWorld()->rayTest(btVector3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z), btVector3(camDirection.x, camDirection.y, camDirection.z), rayCallback);

	//if (rayCallback.hasHit())
	//{
	//	if (rayCallback.m_collisionObject->getUserIndex() != 5)
	//		std::cout << "HIT Object: " << rayCallback.m_collisionObject->getCollisionShape()->getShapeType() << std::endl;
	//}

	m_player->SetPosition(glm::vec3(bt_playerPos.getX(), bt_playerPos.getY(), bt_playerPos.getZ()));
	//std::cout << m_player->GetPosition().x << " " << m_player->GetPosition().y << " " << m_player->GetPosition().z << std::endl;
}