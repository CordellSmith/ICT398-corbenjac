#include "..\headers\GameControlEngine.h"

#include "GL/glew.h"

int randPositions[20];
int randRotations[20];

const int GameControlEngine::RunEngine()
{
	Initialize();

	GameLoop();

	// NOTE: return 0 automatically calls ~GameControlEngine() which calls Destroy()
	// This results in a crash on the second run through when trying to access/delete data already deleted
	//Destroy();

	return 0;
}

void GameControlEngine::Initialize()
{
	// Initialize from script
	ScriptManager::Instance().LoadWindowInitLua(ScreenWidth, ScreenHeight, screenTitle, fullScreen);
	ScriptManager::Instance().LoadCamInitLua(
		m_camera->GetPosition(), 
		m_camera->GetYaw(), 
		m_camera->GetPitch(), 
		m_camera->GetFov(), 
		m_camera->GetNearPlane(), 
		m_camera->GetFarPlane());
	ScriptManager::Instance().LoadModelsInitLua(m_allModelsData, m_modelsData);
	ScriptManager::Instance().LoadHeightmapsInitLua(m_allHeightmapsData, m_heightmapsData);

	// Exit if error creating window
	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, screenTitle, fullScreen) != 0)
		exit(-1);

	// Create viewport
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize gameworld
	m_gameWorld = new GameWorld();

	// Initialize camera perspective and position
	m_camera->SetPerspective(glm::radians(m_camera->GetFov()), ScreenWidth / (float)ScreenHeight, m_camera->GetNearPlane(), m_camera->GetFarPlane());
	m_camera->PositionCamera(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z, m_camera->GetYaw(), glm::radians(m_camera->GetPitch()));

	// Create new player
	player = new Player("Player");

	/**********************************Loading of all heightfields at once**************************************/
	// Get iterator to start of heightfields map
	std::unordered_map<std::string, HeightmapsData>::iterator itHeightfields = m_allHeightmapsData.begin();
	
	// BruteForce variable
	Bruteforce* bfHeightfield;

	// Initialize all heightfields in map
	while (itHeightfields != m_allHeightmapsData.end())
	{
		// Create new BruteForce
		bfHeightfield = new Bruteforce((*itHeightfields).second.modelScales[0], (*itHeightfields).second.modelScales[1], (*itHeightfields).second.modelScales[2]);
		
		// Initialize data from map and push to terrains vector
		bfHeightfield->LoadHeightfield((*itHeightfields).second.filePath, (*itHeightfields).second.fileSize);
		bfHeightfield->GenerateTerrain(TextureManager::Instance().GetTextureID((*itHeightfields).second.texFilePath), (*itHeightfields).second.texFilePath);
		bfHeightfield->SetPosition(glm::vec3((*itHeightfields).second.modelPositions[0], (*itHeightfields).second.modelPositions[1], (*itHeightfields).second.modelPositions[2]));
		m_terrains.push_back(bfHeightfield);
		
		// Move camera to be on top of terrain 
		if ((*itHeightfields).first == "terrain")
		{
			//m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, bfHeightfield->GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z), m_camera->GetPosition().z));
		}

		// Increment iterator
		itHeightfields++;
	}
	/**********************************Loading of all heightfields at once**************************************/
		
	// Pass camera into gameworld
	m_gameWorld->SetCamera(m_camera);

	// Initialize asset factory
	m_assetFactory = new GameAssetFactory();

	// Initialize physics engine
	m_physicsWorld = new PhysicsEngine();

	/*
		When creating .raw files in Gimp. Make sure the file is Grey-scale when creating and when exporting, 
		make sure to select raw image format (.data) and Planar (RRR GGG BBB). Then you can just rename the .data 
		extension to .raw and its all good to go!
	*/

	/********************Loading of all models at once*******************/
	// Create asset
	IGameAsset* modelAsset;

	//std::vector<ComputerAI*> m_allAI;
	ComputerAI* modelAI;
	
	// Asset xyz scale and pos
	float assetScaleXYZ[3];
	float assetPosXYZ[3];

	// Get iterator to start of models map
	std::unordered_map<std::string, ModelsData>::iterator itModels = m_allModelsData.begin();

	// Loop through map until all models created
	while (itModels != m_allModelsData.end())
	{
		// For each different type of model that isn't the player model
		if ((*itModels).first != "player")
		{
			// For each model of same type
			for (int k = 0; k < (*itModels).second.modelPositions.size(); k++)
			{
				// Get scales
				for (int j = 0; j < (*itModels).second.modelScales[k].size(); j++)
				{
					assetScaleXYZ[j] = (*itModels).second.modelScales[k][j];
				}

				// Get positions
				for (int j = 0; j < (*itModels).second.modelPositions[k].size(); j++)
				{
					assetPosXYZ[j] = (*itModels).second.modelPositions[k][j];
				}

				// Create name asset data and add to asset map
				modelAsset = m_assetFactory->CreateAsset(ASS_OBJECT, (*itModels).first);
				modelAsset->LoadFromFilePath((*itModels).second.filePath);
				modelAsset->AddTexutre(TextureManager::Instance().GetTextureID((*itModels).second.texFilePath), (*itModels).second.texFilePath);
				modelAsset->SetScale(glm::vec3(assetScaleXYZ[0], assetScaleXYZ[1], assetScaleXYZ[2]));
				modelAsset->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));

				// If AI model, make AI for it
				if ((*itModels).second.isAI[k])
				{
					// Create new computerAI and push to vector storing them
					modelAI = new ComputerAI(glm::vec3((*itModels).second.modelPositions[k][0], (*itModels).second.modelPositions[k][1], (*itModels).second.modelPositions[k][2]));
					m_allAI.push_back(modelAI);
					modelAsset->SetAI(modelAI);
					std::cout << "Model loaded" << std::endl;
				}

				m_assetFactory->AddAsset(modelAsset);
			}
		}
		// Player model
		else if ((*itModels).first == "player")
		{
			for (int k = 0; k < (*itModels).second.modelPositions.size(); k++)
			{
				// Get scales
				for (int j = 0; j < (*itModels).second.modelScales[k].size(); j++)
				{
					assetScaleXYZ[j] = (*itModels).second.modelScales[k][j];
				}

				// Get positions
				for (int j = 0; j < (*itModels).second.modelPositions[k].size(); j++)
				{
					assetPosXYZ[j] = (*itModels).second.modelPositions[k][j];
				}
			}
			// Initialize player model
			player->LoadFromFilePath((*itModels).second.filePath);
			player->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));
			player->SetScale(glm::vec3(assetScaleXYZ[0], assetScaleXYZ[1], assetScaleXYZ[2]));
		}
		// Increment iterator
		itModels++;
	}
	/********************Loading of all models at once*******************/

	m_windowManager->GetInputManager()->SetPlayer(player);


	/********************AI Testing*******************/
	/*ComputerAI* p = new ComputerAI();
	for (int i = 0; i < 1000; i++)
	p->Update();
	getchar();*/
	/********************AI Testing*******************/

	for (int i = 0; i < 15; i++)
	{
		int randNum1 = rand() % (6000 - 0 + 1) + 0;
		int randNum2 = rand() % (360 - 1 + 1) + 1;

		randPositions[i] = randNum1;
		randRotations[i] = randNum2;
	}

	// Physics engine initialization
	InitializePhysics();

	// Initialize the game world, pass in terrain, assets and physics engine *** Can be reworked *** 
	m_gameWorld->SetTerrains(m_terrains);
	m_gameWorld->Init(player, m_assetFactory->GetAssets());
	m_gameWorld->SetAI(m_allAI);
	m_gameWorld->SetPhysicsWorld(m_physicsWorld, m_collisionBodyPos);
}

void GameControlEngine::GameLoop()
{
	while (m_windowManager->ProcessInput(true))
	{
		// Use our TimeManager singleton to calculate our framerate every frame
		TimeManager::Instance().CalculateFrameRate(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update the game world
		m_gameWorld->Update();

		// Swap buffers
		m_windowManager->SwapTheBuffers();
	}
}

float RandomPos()
{
	return rand() % (4000 - 0 + 1) + 0;
}

void GameControlEngine::InitializePhysics()
{
	// Create camera rigid body to collide with objects
	glm::vec3 camerPos(player->GetPosition());
	btVector3 bt_cameraPos(camerPos.x, camerPos.y, camerPos.z);
	m_physicsWorld->CreatePlayerControlledRigidBody(bt_cameraPos);
	m_collisionBodyPos.push_back(bt_cameraPos);

	// Add player (taxi) to the rigid bodies
	glm::vec3 playerPos(m_camera->GetPosition());
	btVector3 bt_playerPos(playerPos.x, playerPos.y, playerPos.z);
	m_collisionBodyPos.push_back(bt_playerPos);

	int i = 1;
	int numOfRocks = 10;
	int numOfKnights = 20;
	// Loop through map and add all assets to the collision body list
	std::multimap<std::string, IGameAsset*>::const_iterator itr;
	for (itr = m_assetFactory->GetAssets().begin(); itr != m_assetFactory->GetAssets().end(); itr++)
	{
		btVector3 randomPos;
		float tempX, tempY, tempZ;

		if (itr->second->GetAssetName() == "knight")
		{
			for (int j = 0; j < numOfKnights; j++)
			{
				tempX = itr->second->GetPosition().x + RandomPos();
				tempZ = itr->second->GetPosition().z + RandomPos();
				tempY = m_terrains[0]->GetAverageHeight(tempX, tempZ) + 100;

				randomPos = btVector3(tempX, tempY, tempZ);
				
				m_physicsWorld->CreateStaticRigidBody(randomPos, "knight");
				m_collisionBodyPos.push_back(randomPos);
			}
		}
		
		if (itr->second->GetAssetName() == "rock")
		{
			
			for (int j = 0; j < numOfRocks; j++)
			{
				tempX = itr->second->GetPosition().x + RandomPos();
				tempZ = itr->second->GetPosition().z + RandomPos();
				tempY = m_terrains[0]->GetAverageHeight(tempX, tempZ) + 100;

				randomPos = btVector3(tempX, tempY, tempZ);

				m_physicsWorld->CreateStaticRigidBody(randomPos, "rock");
				m_collisionBodyPos.push_back(randomPos);
			}
		}
	}

	//  *** Can this be changed to the terrain mesh? *** 
	// Create static rigid body (floor)
	//m_physicsWorld->CreateStaticRigidBody();
	//m_collisionBodyPos->push_back(btVector3(0.0, 0.0, 0.0));

	// Activate all rigid body objects
	m_physicsWorld->ActivateAllObjects();
}

void GameControlEngine::Destroy()
{
	// Destroy game world
	m_gameWorld->Destroy();

	// Delete window
	if (m_windowManager)
	{
		m_windowManager->Destroy();

		delete m_windowManager;
		m_windowManager = nullptr;
	}

	// Delete camera
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}