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
	
	// Asset xyz position, scale and rotation
	float assetScaleXYZ[3];
	float assetPosXYZ[3];
	
	// Get iterator to start of models map
	std::multimap<std::string, ModelsData>::iterator itModels = m_allModelsData.begin();

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

			
			if ((*itModels).first != "person")
			{
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
			}
			else
			{
				// Creates NPC object for person
				modelAsset = m_assetFactory->CreateAsset(ASS_NPC, (*itModels).first);
				modelAsset->LoadFromFilePath((*itModels).second.filePath);
				modelAsset->AddTexutre(TextureManager::Instance().GetTextureID((*itModels).second.texFilePath), (*itModels).second.texFilePath);
				modelAsset->SetScale(glm::vec3(assetScaleXYZ[0], assetScaleXYZ[1], assetScaleXYZ[2]));
				modelAsset->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));
			}

			/// CSmith 
			///	20/10/18 Dimensions of models calculated here for bouding box
			modelAsset->CalculateDimensions();

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
	m_affordanceTable = new AffordanceData();
	ScriptManager::Instance().LoadAffordanceTable(*m_affordanceTable);

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
	m_gameWorld->SetAI(m_agents);
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
	// Iterator for the models data (this is to retrieve unique names from the script e.g. chair1, table1...)
	std::multimap<std::string, ModelsData>::iterator modelsItr = m_allModelsData.begin();

	for (itr = m_assetFactory->GetAssets().begin(); itr != m_assetFactory->GetAssets().end(); itr++, modelsItr++)
	{
		btVector3 objRigidBodyPosition, objRigidBodyRotation;

		objRigidBodyPosition = GlmtoBt(itr->second->GetPosition());
		objRigidBodyRotation = GlmtoBt(itr->second->GetRotation());

		// Create UNIQUE NAME
		std::string uniqueName = modelsItr->second.objectName;

		Affordance* affordance = new Affordance(uniqueName);
		CollisionBody* colBody = new CollisionBody(modelsItr->second.objectName, itr->second->GetAssetName(), objRigidBodyPosition, objRigidBodyRotation, affordance);

		if (itr->second->GetAssetName() == "player")
		{
			// Create camera capsule to collide with objects
			m_physicsWorld->CreatePlayerControlledRigidBody(objRigidBodyPosition);
			m_collisionBodies.push_back(colBody);
			continue;
		}
		
		if (itr->second->GetAssetName() == "lecTheatre")
		{			
			/// 15/10/18 CSmith Debug Draw
			/// 16/10/18		Debug Draw almost working
			///					Mesh collider with LBLT is working
			///					Static Triangle mesh of LBLT is created here!
			///					Important: used to get model matrix for debug draw lines
			m_physicsWorld->ParseModel(itr->second->GetModel());
			// Debug draw is also used here
			m_physicsWorld->TriangleMeshTest(itr->second->GetModel()->GetMeshBatch(), true, false);
			m_collisionBodies.push_back(colBody);
			// This has to be called after the mesh data is passed in
			m_physicsWorld->InitDebugDraw();
			continue;
		}

		if (itr->second->GetAssetName() == "ball")
		{
			m_physicsWorld->AddSphere(110.0, objRigidBodyPosition, colBody);
			m_collisionBodies.push_back(colBody);
			continue;
		}

		if (itr->second->GetAssetName() == "person")
		{
			// Create a new AI with position
			ComputerAI* AI = new ComputerAI(itr->second->GetPosition());
			// Give it to person
			itr->second->SetAI(AI);

			// Create a collision body object with AI
			CollisionBody* colBody = new CollisionBody(uniqueName, itr->second->GetAssetName(), objRigidBodyPosition, objRigidBodyRotation, affordance, AI);

			// Create new dynamic rigid body
			m_physicsWorld->CreateDynamicRigidBody(objRigidBodyPosition, itr->second->GetDimensons(), colBody);

			// Add to collision bodies vector with UNIQUE NAME
			m_collisionBodies.push_back(colBody);
			// Add to all AI
			m_agents.push_back(AI);

			std::cout << uniqueName << " Loaded" << std::endl;
			continue;
		}

		/// CSmith	
		///			03/10/18 -- Start
		///			09/10/18 -- Only generating box shape rigid objects, removed name specific code
		///			20/10/18 -- CreateDynamicRigidBody() now takes the models dimensions to create a more accurate size bounding box
		m_physicsWorld->CreateDynamicRigidBody(objRigidBodyPosition, itr->second->GetDimensons(), colBody);
		m_collisionBodies.push_back(colBody);
	}

	// Initialize Affordance
	// Local variable to call initialisation of base affordances
	Affordance affordance = Affordance("base");
	affordance.InitBaseAffordances(*m_affordanceTable, m_collisionBodies);

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