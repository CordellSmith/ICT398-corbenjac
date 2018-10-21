#include "GameControlEngine.h"

#include "GL/glew.h"

const int GameControlEngine::RunEngine()
{
	Initialize();

	GameLoop();

	Destroy();

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
	glEnable(GL_CULL_FACE);

	// Initialize gameworld
	m_gameWorld = new GameWorld();

	// Initialize camera perspective and position
	m_camera->SetPerspective(glm::radians(m_camera->GetFov()), ScreenWidth / (float)ScreenHeight, m_camera->GetNearPlane(), m_camera->GetFarPlane());
	m_camera->PositionCamera(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z, m_camera->GetYaw(), glm::radians(m_camera->GetPitch()));

	// Create new player
	m_player = new Player("Player");

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
	m_physicsWorld->SetCamera(m_camera);

	/*
		When creating .raw files in Gimp. Make sure the file is Grey-scale when creating and when exporting, 
		make sure to select raw image format (.data) and Planar (RRR GGG BBB). Then you can just rename the .data 
		extension to .raw and its all good to go!
	*/

	/********************Loading of all models at once*******************/
	// Create asset
	IGameAsset* modelAsset = NULL;

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

			// Not sure why this is here
			if ((*itModels).first != "lecTheatre")
			{
				modelAsset->AddTexutre(TextureManager::Instance().GetTextureID((*itModels).second.texFilePath), (*itModels).second.texFilePath);
			}
			modelAsset->SetScale(glm::vec3(assetScaleXYZ[0], assetScaleXYZ[1], assetScaleXYZ[2]));
			modelAsset->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));
			/// CSmith 
			///	20/10/18 Dimensions of models calculated here for bouding box
			modelAsset->CalculateDimensions();

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

		if ((*itModels).first == "player")
		{
			for (int k = 0; k < (*itModels).second.modelPositions.size(); k++)
			{
				// Get positions
				for (int j = 0; j < (*itModels).second.modelPositions[k].size(); j++)
				{
					assetPosXYZ[j] = (*itModels).second.modelPositions[k][j];
				}
			}

			// Initialize player model
			m_player->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));
		}
		
		// Increment iterator
		itModels++;
	}

	/********************Loading of all models at once*******************/
	m_windowManager->GetInputManager()->SetPlayer(m_player);

	/// 21/10/18 CSmith Affordance Script Read-in
	ScriptManager::Instance().LoadAffordanceTable(m_affordanceTable);

	/********************AI Testing*******************/
	/*ComputerAI* p = new ComputerAI();
	for (int i = 0; i < 1000; i++)
	p->Update();
	getchar();*/
	/********************AI Testing*******************/

	// Physics engine initialization
	InitializePhysics();

	// Initialize the game world, pass in terrain, assets and physics engine *** Can be reworked *** 
	m_gameWorld->Init(m_player, m_assetFactory->GetAssets());
	m_gameWorld->SetTerrains(m_terrains);
	m_gameWorld->SetAI(m_allAI);
	m_gameWorld->SetPhysicsWorld(m_physicsWorld, m_collisionBodies);
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

void GameControlEngine::InitializePhysics()
{
	// Iterate throgh objects map and add all objects to the collision body list
	std::multimap<std::string, IGameAsset*>::const_iterator itr;
	for (itr = m_assetFactory->GetAssets().begin(); itr != m_assetFactory->GetAssets().end(); itr++)
	{
		btVector3 objRigidBodyPosition;

		if (itr->second->GetAssetName() == "player")
		{
			// Create camera capsule shape to collide with objects
			btVector3 bt_cameraPos(m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
			m_physicsWorld->CreatePlayerControlledRigidBody(bt_cameraPos);
			m_collisionBodies.push_back(new CollisionBody("player", bt_cameraPos));		
			continue;
		}
		
		if (itr->second->GetAssetName() == "lecTheatre")
		{
			// Convert from glm::vec3 to Bullets btVector3
			objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);

			std::cout << "Physics Init " << itr->second->GetAssetName() << ": " << itr->second->GetModel()->GetMeshBatch().size() << " and " << itr->second->GetModel()->GetMeshBatch().size() << std::endl;

			/// 15/10/18 CSmith Debug Draw
			/// 16/10/18		Debug Draw almost working
			///					Mesh collider with LBLT is working
			// Important: used to get model matrix for debug draw lines
			m_physicsWorld->ParseModel(itr->second->GetModel());
			// Static Triangle mesh of LBLT is created here!
			// Debug draw is also used here
			m_physicsWorld->TriangleMeshTest(itr->second->GetModel()->GetMeshBatch(), true, false);
			m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));
			// This has to be called after the mesh data is passed in
			//m_physicsWorld->InitDebugDraw();
			continue;
		}

		if (itr->second->GetAssetName() == "ball")
		{
			// Have to convert from glm::vec3 to Bullets btVector3
			objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);

			m_physicsWorld->AddSphere(110.0, objRigidBodyPosition);
			// Add to our array of collision bodies
			//m_collisionBodyPos.push_back(objRigidBodyPosition);
			m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));
			continue;
		}

		if (itr->second->GetAssetName() == "chair")
		{
			// Creates 5 chairs (testing)
			for (int i = 0; i < 5; i++)
			{
				objRigidBodyPosition = btVector3(
					itr->second->GetPosition().x + 100 * i, 
					itr->second->GetPosition().y + 100 * i, 
					itr->second->GetPosition().z + 100 * i
				);
				m_physicsWorld->CreateDynamicRigidBody(objRigidBodyPosition, itr->second->GetDimensons());
				m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));
			}
			continue;
		}
		
		// Adjust rotation for table, orientation is not correct
		if (itr->first == "table")
		{
			itr->second->SetRotation(glm::vec3(0.0, 20.0, 45.0));
		}

		/// CSmith	
		///			03/10/18 -- Start
		///			09/10/18 -- Only generating box shape rigid objects, removed name specific code
		///			20/10/18 -- CreateDynamicRigidBody() now takes the models dimensions to create a more accurate size bounding box
		objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);
		m_physicsWorld->CreateDynamicRigidBody(objRigidBodyPosition, itr->second->GetDimensons());
		m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));
	}

	// Parse physics data to player
	m_player->ParsePhysics(*m_physicsWorld, m_collisionBodies);

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