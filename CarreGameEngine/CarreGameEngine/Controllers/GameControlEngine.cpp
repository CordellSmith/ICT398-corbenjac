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
	// Create camera rigid body to collide with objects
	btVector3 bt_cameraPos(m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	m_physicsWorld->CreatePlayerControlledRigidBody(bt_cameraPos);
	m_collisionBodies.push_back(new CollisionBody("player", bt_cameraPos));

	// Iterate throgh objects map and add all objects to the collision body list
	std::multimap<std::string, IGameAsset*>::const_iterator itr;
	for (itr = m_assetFactory->GetAssets().begin(); itr != m_assetFactory->GetAssets().end(); itr++)
	{
		btVector3 objRigidBodyPosition;
		//float tempX, tempY, tempZ;
		
		if (itr->second->GetAssetName() == "lecTheatre")
		{
			// Have to convert from glm::vec3 to Bullets btVector3
			objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);

			// Add static floor rigid body in physics world (size set to 1000 x 1000)
			m_physicsWorld->CreateStaticRigidBody(objRigidBodyPosition);
			// Add to our array of collision bodies
			//m_collisionBodyPos.push_back(objRigidBodyPosition);
			m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));

			float tempX = itr->second->GetPosition().x;
			float tempZ = itr->second->GetPosition().z;
			float tempY = itr->second->GetPosition().y;

			btVector3 lecPos = btVector3(tempX, tempY, tempZ);
			std::cout << "Physics Init " << itr->second->GetAssetName() << ": " << itr->second->GetModel()->GetMeshBatch().size() << " and " << itr->second->GetModel()->GetMeshBatch().size() << std::endl;

			m_physicsWorld->TriangleMeshTest(itr->second->GetModel()->GetMeshBatch(), lecPos, true, false);
			m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), lecPos));

			/// 15/10/18 CSmith Debug Draw
			// Load in lecture theatre mesh to physics debug draw
			for (int i = 0; i < itr->second->GetModel()->GetMeshBatch().size(); i++)
				m_physicsWorld->ReadInMesh(&itr->second->GetModel()->GetMeshBatch()[i]);
			// This has to be called after the mesh data is passed in
			std::cout << "Debug Mesh: " << itr->first << " loaded" << std::endl;
			m_physicsWorld->InitDebugDraw();

			continue;
		}

		if (itr->second->GetAssetName() == "ball")
		{
			// Have to convert from glm::vec3 to Bullets btVector3
			objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);

			// Add static floor rigid body in physics world (size set to 1000 x 1000)
			m_physicsWorld->AddSphere(100.0, objRigidBodyPosition);
			// Add to our array of collision bodies
			//m_collisionBodyPos.push_back(objRigidBodyPosition);
			m_collisionBodies.push_back(new CollisionBody(itr->second->GetAssetName(), objRigidBodyPosition));
			continue;
		}

		/// Cordell	03/10/18 -- Start
		///			09/10/18 -- Only generating box shape rigid objects, removed name specific code
		objRigidBodyPosition = btVector3(itr->second->GetPosition().x, itr->second->GetPosition().y, itr->second->GetPosition().z);
		// Add crates dynamic rigid body in physics world (size set to 100 x 100 x 100)
		m_physicsWorld->CreateDynamicRigidBody(objRigidBodyPosition);
		// Add to our array of collision bodies
		//m_collisionBodyPos.push_back(objRigidBodyPosition);
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


//tempModel = player->GetModel();
//std::vector<Mesh> temp6 = tempModel->GetMeshBatch();
//Mesh temp7 = temp6[0];
//std::vector<Vertex3> temp8 = temp7.GetVertices();
//int size = temp8.size();
//std::string temp0 = (*itModels).first;
//std::cout << temp0 << ": " << size << "\n\n\n\n" << std::endl;
//glm::vec3 temp4 = tempModel->GetPosition();
//
//
//void PhysicsEngine::CreateStaticRigidBody(btVector3 &pos, std::string type)
//{
//	btCollisionShape* groundShape;
//
//	if (type == "knight")
//		groundShape = new btBoxShape(btVector3(btScalar(80.0), btScalar(100.0), btScalar(80.0)));
//	else if (type == "rock")
//		groundShape = new btBoxShape(btVector3(btScalar(225.0), btScalar(100.0), btScalar(200.0)));
//	else
//		groundShape = new btBoxShape(btVector3(btScalar(0.0), btScalar(0.0), btScalar(0.0)));
//
//	m_collisionShapes.push_back(groundShape);
//
//	btVector3 temp = pos;
//	//temp.setX(temp.getX() - 3000);
//	//temp.setZ(temp.getZ() - 50);
//
//	// Initialize transform and location
//	btTransform groundTransform;
//	groundTransform.setIdentity();
//	groundTransform.setOrigin(temp);
//
//	// Set mass (zero for static)
//	m_mass = 0.0;
//
//	// Set dynamic objects to objects with mass that is non-zero
//	m_isDynamic = (m_mass != 0.0f);
//
//	btVector3 localInertia(0.0, 0.0, 0.0);
//
//	if (m_isDynamic)
//		groundShape->calculateLocalInertia(m_mass, localInertia);
//
//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, groundShape, localInertia);
//	btRigidBody* body = new btRigidBody(rbInfo);
//
//	// Set the index for the type of rigid body that is being created
//	body->setUserIndex(PLANE);
//
//	// Add the body to the dynamic world
//	m_dynamicsWorld->addRigidBody(body);
//}