/*
* Implementation of PhysicsEngine.h file
* Author - Jack Matters
* Note - Bullet Physics Engine documentation http://bulletphysics.org/wordpress/?page_id=9
*/

//Includes
#include "PhysicsEngine.h"
#include <iostream>

// Default constructor
PhysicsEngine::PhysicsEngine()
{
	// Collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	// Use the default collision dispatcher
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// A good general purpose broadphase
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	// The default constraint solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The dynamic world
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// Set the gravity
	m_dynamicsWorld->setGravity(btVector3(0, -200, 0));

	// Initialize all objects to static
	m_isDynamic = false;

	// Initialize player object location
	m_playerObject.setZero();

	m_oldForce.setZero();
	m_newForce.setZero();

	// Debug draw shader init
	m_debugShader = new Shader();

	// Camera object for MVP matrix
	m_camera = new Camera();

	/*btIDebugDraw tempp;
	m_dynamicsWorld->setDebugDrawer(btIDebugDraw::DebugDrawModes::DBG_MAX_DEBUG_DRAW_MODE);
	m_dynamicsWorld->deb*/
}

// De-constructor (not implemented)
PhysicsEngine::~PhysicsEngine(){};

// Create a static rigid body
void PhysicsEngine::CreateStaticRigidBody(btVector3 &pos)
{
	btCollisionShape* groundShape;
	groundShape = new btBoxShape(btVector3(btScalar(10000), btScalar(200), btScalar(10000)));

	btVector3 temp = pos;
	//temp.setX(temp.getX() - 3000);
	//temp.setZ(temp.getZ() - 50);

	// Initialize transform and location
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(temp);

	// Set mass (zero for static)
	m_mass = 0.0;

	// Set dynamic objects to objects with mass that is non-zero
	m_isDynamic = (m_mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (m_isDynamic)
		groundShape->calculateLocalInertia(m_mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(PLANE);

	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);
}

// Create a bounding box for camera or player controlled object
void PhysicsEngine::CreatePlayerControlledRigidBody(btVector3 &playerObj)
{
	// Create box shape and add to shape array
	//btCollisionShape* camShape = new btBoxShape(btVector3(btScalar(30), btScalar(20), btScalar(50)));
	btCollisionShape* camShape = new btCapsuleShape(100, 200);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set origin of body
	startTransform.setOrigin(playerObj);

	// Set mass (non-zero for dynamic)
	m_mass = 1.0f;

	// Set dynamic objects to objects with mass that is non-zero
	m_isDynamic = (m_mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (m_isDynamic)
		camShape->calculateLocalInertia(m_mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, camShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(CAMERA);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setCcdMotionThreshold(1e-7);
	body->setCcdSweptSphereRadius(15);
	
	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);

	// Set new player object coordinates
	m_playerObject = playerObj;

	// Disable gravity for this object
	body->setGravity(btVector3(0.0, 0.0, 0.0));
}

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(btVector3 &pos, glm::vec3& dimensions)
{
	// Create box shape size of the dimensions of the object
	btCollisionShape* boxShape = new btBoxShape(btVector3(
		btScalar(dimensions.x / 2), 
		btScalar(dimensions.y / 2), 
		btScalar(dimensions.z / 2))
	);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();
	// Set origin of body
	startTransform.setOrigin(pos);

	// Set mass (non-zero for dynamic)
	m_mass = 10.0;

	// Set dynamic objects to objects with mass that is non-zero
	m_isDynamic = (m_mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (m_isDynamic)
		boxShape->calculateLocalInertia(m_mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, boxShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(BOX);
	
	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);
}


// Create a dynamic rigid body
btRigidBody* PhysicsEngine::AddSphere(float radius, btVector3 &startPos)
{
	// Create box shape and add to shape array
	btCollisionShape* sphereShape = new btSphereShape(radius);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();
	// Set origin of body
	startTransform.setOrigin(startPos);

	// Set mass (non-zero for dynamic)
	m_mass = 10.0;

	// Set dynamic objects to objects with mass that is non-zero
	m_isDynamic = (m_mass != 1.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (m_isDynamic)
		sphereShape->calculateLocalInertia(m_mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, sphereShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(SPHERE);

	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);

	return body;
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<CollisionBody*>& collisionBodies, btVector3& playerObj)
{
	m_dynamicsWorld->stepSimulation(1.f / 60.0f, 10);

	btVector3 btFrom(playerObj.getX(), playerObj.getY(), playerObj.getZ());
	btVector3 btTo(playerObj.getX(), -3000.0f, playerObj.getZ());
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	m_dynamicsWorld->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

	// Messing with terrain tracking

	//printf("Collision at: <%.2f>\n", res.m_hitPointWorld.getY());

	// Update positions of all dynamic objects
	for (int j = 0; j < m_dynamicsWorld->getNumCollisionObjects(); j++)
	{
		// Get the next object, and activate it
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		// Reset forces on player object prior to next step simulation
		if (body->getUserIndex() == CAMERA)
		{
			//body->clearForces();
			btVector3 tempVel = body->getLinearVelocity();
			//body->setLinearVelocity(btVector3(0,-2,0));
			body->setLinearVelocity(btVector3(0, 0, 0));
		}

		// Update rigid body positions
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
	
		// Check to see if player object
		if (body->getUserIndex() == CAMERA)
		{
			// TODO: Make this better (Jack)
			// Apply force in direction camera was moved
			m_newForce.setX((playerObj.x() - m_playerObject.x()) * 3000);
			//m_newForce.setY((playerObj.y() - m_playerObject.y()) * 3000);
			m_newForce.setZ((playerObj.z() - m_playerObject.z()) * 3000);


			/// Terrain checking needs to be fixed csmith 17/10/18
			// If floor height gets higher
			//if (res.m_hitPointWorld.getY() > m_floorHeight)
			//{
			//	std::cout << "Up" << std::endl;
			//	std::cout << res.m_hitPointWorld.getY() << std::endl;

			//	// Move player position up
			//	m_newForce.setY((playerObj.y() - m_playerObject.y()) * 3000);
			//	// New floor height is set to current ray hit value

			//	std::cout << "new force " << m_newForce.getY() << std::endl;

			//	if (m_newForce.getY() >= 6000)
			//	{
			//		m_newForce.setY(0);
			//		m_floorHeight = res.m_hitPointWorld.getY();
			//	}
			//}
			
			// If floor height gets lower
			//if (res.m_hitPointWorld.getY() < m_floorHeight)
			//{
			//	std::cout << "Down" << std::endl;
			//	std::cout << res.m_hitPointWorld.getY() << std::endl;

			//	// Move player position down
			//	m_newForce.setY((playerObj.y() - m_playerObject.y()) * (-3000));
			//	// New floor height is set to current ray hit value
			//	m_floorHeight = res.m_hitPointWorld.getY();
			//}

			//if (res.m_hitPointWorld.getY() < m_stepHeight)
			//{
			//	std::cout << "Moving Down" << std::endl;
			//	btScalar stepDown = collisionBodies[j]->m_position.getY() - 300;
			//	std::cout << "Step Down: " << stepDown << std::endl;
			//	m_newForce.setY((playerObj.y() - m_playerObject.y()) * -stepDown);
			//	m_stepHeight = res.m_hitPointWorld.getY();
			//}

			//std::cout << "Player Height: " << m_playerObject.y() << std::endl;

			// Update rigid body location for drawing
			body->applyCentralForce(m_newForce);
			m_playerObject = trans.getOrigin();
			playerObj = m_playerObject;
		}
		else
		{
			// Update object positions for drawing
			collisionBodies[j]->m_position.setX(trans.getOrigin().getX());
			collisionBodies[j]->m_position.setY(trans.getOrigin().getY());
			collisionBodies[j]->m_position.setZ(trans.getOrigin().getZ());
		}	
	}
	//std::cout << "/n/n/n/n/n" << std::endl;
}

// Testing for creating a heightfield terrain shape
void PhysicsEngine::CreateHeightfieldTerrainShape()
{
	//unsigned char *terrainData;
	int sizet = 128;

	// Open for binary read, print error if error found
	std::ifstream infile("Resources/terrain/height128.raw", std::ios::binary);
	if (!infile)
	{
		exit(0);
	}

	// Allocate memory, return false if no size = 0
	if (m_terrainData)
		delete[] m_terrainData;
	if (sizet>0)
		m_terrainData = new unsigned char[sizet*sizet];
	if (m_terrainData == NULL)
		exit(0);

	// Read in heightfield and get length of file
	infile.seekg(0, std::ios::end);
	int length = infile.tellg();

	// Read data in as a block, cast to char*, set size, and close file
	infile.seekg(0, std::ios::beg);
	infile.read(reinterpret_cast<char *>(m_terrainData), length);
	infile.close();
	this->m_size = sizet;

	// Create heightfield shape
	btCollisionShape* heightfieldShape = new btHeightfieldTerrainShape(128, 128, &m_terrainData, 1, 1, 128, 1, PHY_UCHAR, false);
	m_collisionShapes.push_back(heightfieldShape);

	// Initialize transform and location
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));

	// Set mass (non-zero for dynamic)
	m_mass = 0.0;
	m_isDynamic = (m_mass != 0.0f);

	// Set initial inertia
	btVector3 localInertia(0.0, 0.0, 0.0);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, heightfieldShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(HEIGHTFIELD);

	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);
}

void PhysicsEngine::ActivateAllObjects()
{
	// Loop through every rigid body object
	for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		// Get the next object, and activate it
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
		obj->forceActivationState(DISABLE_DEACTIVATION);
	}
}

btCollisionObject* PhysicsEngine::TriangleMeshTest(std::vector<Mesh> &modelMesh, bool useQuantizedBvhTree, bool collision)
{
	btTriangleMesh* trimesh = new btTriangleMesh();
	for (int j = 0; j < modelMesh.size(); j++)
	{
		Mesh tempMesh = modelMesh[j];
		std::vector<Vertex3> tempMeshVertex = tempMesh.GetVertices();

		for (int i = 0; i < tempMeshVertex.size(); i+=3)
		{
			glm::vec3 p1 = tempMeshVertex[i].m_position;
			glm::vec3 p2 = tempMeshVertex[i+1].m_position;
			glm::vec3 p3 = tempMeshVertex[i+2].m_position;

			btVector3 A, B, C;

			A = btVector3(p1.x, p1.y, p1.z);
			B = btVector3(p2.x, p2.y, p2.z);
			C = btVector3(p3.x, p3.y, p3.z);

			trimesh->addTriangle(A, B, C);

			// Add points to debug draw array of btVector3s
			//m_debugLines.push_back(A);
			//m_debugLines.push_back(B);
			//m_debugLines.push_back(C);
		}
	}

	btTransform	trans;
	trans.setIdentity();

	// Set origin to the position of the object (whatever object is being passed in)
	trans.setOrigin(btVector3(modelMesh[0].GetPosition().x, modelMesh[0].GetPosition().y, modelMesh[0].GetPosition().z));

	// Set trimesh scale
	trimesh->setScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));

	btCollisionShape* trimeshShape = new btBvhTriangleMeshShape(trimesh, useQuantizedBvhTree);
	m_collisionShapes.push_back(trimeshShape);

	btVector3 inertia(0, 0, 0);

	btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
	btRigidBody* body = new btRigidBody(0, motionstate, trimeshShape, inertia);

	//m_trianglemeshs.push_back(trimesh);
	//m_triangleMeshBodies.push_back(body);
	body->setUserIndex(MESH);
	//body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	m_dynamicsWorld->addRigidBody(body);
	//std::vector< btVector3* > tmp;
	//m_vertices.push_back(tmp);
	//m_vertices[m_triangleMeshBodies.size() - 1].push_back(&p0);
	//m_vertices[m_triangleMeshBodies.size() - 1].push_back(&p1);
	//m_vertices[m_triangleMeshBodies.size() - 1].push_back(&p2);

	//if (collision)
		
	//return m_triangleMeshBodies.size() - 1;
	

	return body;
}

void PhysicsEngine::InitDebugDraw()
{
	// Create a debug shader source (vertext and fragment shader)
	ShaderSource debugShaderSource = ParseShaders("Resources/shaders/DebugDraw.shader");

	// Initialise the shader program for the physics engine
	GetDebugShader()->Initialize(debugShaderSource.VertexSource, debugShaderSource.FragmentSource);

	glGenVertexArrays(1, &VAO);
	std::cout << VAO << std::endl;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_debugLines[0]) * m_debugLines.size(), &m_debugLines[0], GL_STATIC_DRAW);

	// load the vertex data info
	glVertexAttribPointer(1,  // the handle for the inPos shader attrib should be at pos 0
		3,	// there are 3 values xyz
		GL_FLOAT, // float value
		GL_FALSE, // don't need to be normalised
		4 * sizeof(float),  // how many floats to the next one (btVector3 uses 4 floats)
		(GLvoid*)&m_debugLines[0]  // where do they start as an index); // use 3 values, but add stride each time to get to the next
	);

	glBindVertexArray(0);
}

void PhysicsEngine::DebugDraw()
{
	// Enable shader
	m_debugShader->TurnOn();

	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix();
	//glm::mat4 modelMatrix = CreateTransformationMatrix(m_debugMesh->GetPosition(), m_debugMesh->GetRotation(), m_debugMesh->GetScale());
	glm::mat4 viewMatrix = CreateViewMatrix(m_camera);

	GLint modelMatrixId = m_debugShader->GetVariable("model");
	GLint viewMatrixId = m_debugShader->GetVariable("view");
	GLint projectionMatrixId = m_debugShader->GetVariable("projection");

	m_debugShader->SetMatrix4(modelMatrixId, 1, false, &m_modelMatrix[0][0]);
	m_debugShader->SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
	m_debugShader->SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Enable the position attribute
	glEnableVertexAttribArray(0);

	// Draw the lines
	glDrawArrays(GL_LINES, 0, sizeof(m_debugLines[0]) * m_debugLines.size());

	// Disable the position attribute
	glDisableVertexAttribArray(0);
	
	// Unbind the VAO
	glBindVertexArray(0);

	// Disable shader
	m_debugShader->TurnOff();
}

void PhysicsEngine::ParseModel(Model* model)
{
	// Model matrix changes
	// Store as member variable
	m_scale = glm::vec3(model->GetMeshBatch()[0].GetScale().x, model->GetMeshBatch()[0].GetScale().y, model->GetMeshBatch()[0].GetScale().z);
	m_modelMatrix = CreateTransformationMatrix(
		glm::vec3(model->GetMeshBatch()[0].GetPosition().x, model->GetMeshBatch()[0].GetPosition().y, model->GetMeshBatch()[0].GetPosition().z),
		glm::vec3(0),
		glm::vec3(m_scale.x, m_scale.y, m_scale.z));
}


// Creates all rigid bodies for all game objects
/*bool PhysicsEngine::CreateAllRigidBodies(Data &objectData)
{
	// Holds current rigid body type to create
	Data tempDataType;

	// Loop through data structure, creating rigid body for each game object
	for (int i = 0; i < objectData.length() - 1; i++)
	{
		tempDataType = objectData[i].getType();

		// Determine what rigid body type to create, then call that function
		// Return false if no rigid body type found in data structure
		switch (tempDataType)
		{
		case CAMERA: CreateCameraRigidBody(objectData[i]);
			break;
		case BOX: CreateBoxShapeRigidBody(objectData[i]);
			break;
		case SPHERE: CreateSphereShapeRigidBody(objectData[i]);
			break;
		case HEIGHTFIELD: CreateHeightFieldTerrainShape(objectData[i]);
			break;
		default: std::cout << "Error reading game object data into Physics Engine.." << std::endl;
			return false;
			break;
		}
	}

	// If all data read successfully and rigid bodies created, return true
	return true;
}*/

// Create a sphere shape rigid body for the camera
/*void PhysicsEngine::CreateCameraRigidBody(Data &objectData)
{
	// Create box shape and add to shape array
	btCollisionShape* sphereShape = new btSphereShape(objectData.getRadius());
	collisionShapes.push_back(sphereShape);

	// Initialize transform and location
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(objectData.getPos());

	// Set mass (non-zero for dynamic)
	mass = objectData.getMass();
	isDynamic = (mass != 0.0f);

	// Set initial inertia
	btVector3 localInertia(0.0, 0.0, 0.0);

	// If dynamic, calculate local inertia
	if (isDynamic)
		sphereShape->calculateLocalInertia(mass, localInertia);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, sphereShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(CAMERA);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);

	// Set starting player object/camera coordinates
	playerObject = objectData.getPos();

	// Disable gravity for this object or not
	if(objectData.applyGravity())
		body->setGravity(btVector3(0.0, 0.0, 0.0));
}*/

// Create a box shape rigid body
/*void PhysicsEngine::CreateBoxShapeRigidBody(Data &objectData)
{
	// Create box shape and add to shape array
	btCollisionShape* boxShape = new btBoxShape(btVector3(objectData.getm_scaleX(), objectData.getm_scaleY(), objectData.getm_scaleZ()));
	collisionShapes.push_back(boxShape);

	// Initialize transform and location
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(objectData.getPos());

	// Set mass (non-zero for dynamic)
	mass = objectData.getMass();
	isDynamic = (mass != 0.0f);

	// Set initial inertia
	btVector3 localInertia(0.0, 0.0, 0.0);

	// If dynamic, calculate local inertia
	if (isDynamic)
		boxShape->calculateLocalInertia(mass, localInertia);
	
	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(BOX);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}*/

// Create a sphere shape rigid body
/*void PhysicsEngine::CreateSphereShapeRigidBody(Data &objectData)
{
	// Create sphere shape and add to shape array
	btCollisionShape* sphereShape = new btSphereShape(objectData.getRadius());
	collisionShapes.push_back(sphereShape);

	// Initialize transform and location
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(objectData.getPos());

	// Set mass (non-zero for dynamic)
	mass = objectData.getMass();
	isDynamic = (mass != 0.0f);

	// Set initial inertia
	btVector3 localInertia(0.0, 0.0, 0.0);

	// If dynamic, calculate local inertia
	if (isDynamic)
		sphereShape->calculateLocalInertia(mass, localInertia);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, sphereShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(SPHERE);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}*/

// Create a heightfield terrain shape
/*void PhysicsEngine::CreateHeightFieldTerrainShape(Data &objectData)
{
	// Create heightfield terrain shape and add to shape array
	btCollisionShape* heightfieldShape = new btHeightfieldTerrainShape(objectData.getm_scaleX(), objectData.getm_scaleZ(), objectData.getData(), objectData.getHeightm_scale(), objectData.getMinHeight(), objectData.getMaxHeight(), 1, PHY_UCHAR, true);
	collisionShapes.push_back(heightfieldShape);

	// Initialize transform and location
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(objectData.getPos());

	// Set mass (non-zero for dynamic)
	mass = objectData.getMass();
	isDynamic = (mass != 0.0f);

	// Set initial inertia
	btVector3 localInertia(0.0, 0.0, 0.0);

	// If dynamic, calculate local inertia
	if (isDynamic)
		heightfieldShape->calculateLocalInertia(mass, localInertia);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, heightfieldShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(HEIGHTFIELD);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}*/



/*************************************NEW**************************************/


////Includes
//#include "PhysicsEngine.h"
//#include <iostream>
//
////#define m_numPointMassElements 10000000
//
//// Default constructor
//PhysicsEngine::PhysicsEngine()
//{
//	// Collision configuration contains default setup for memory, collision setup
//	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
//
//	// Use the default collision dispatcher
//	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
//
//	// A good general purpose broadphase
//	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
//
//	// The default constraint solver
//	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
//
//	// The dynamic world
//	m_collisionWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
//
//	// Initialize epsilon
//	m_epsilon = .5;
//
//	count = 0;
//	delta_t = 0.02;
//	game_time = 0.0;
//	prev_game_time = 0.0;
//	physics_lag_time = 0.0;
//
//	// Number of PointMass elements for an object
//	//m_numPointMassElements = 1000;
//
//	// Set the gravity
//	//m_collisionWorld->setGravity(btVector3(0, -30, 0));
//
//	// Initialize all objects to static
//	//m_isDynamic = false;
//
//	// Initialize player object location
//	//m_playerObject.setZero();
//
//	//m_oldForce.setZero();
//	//m_newForce.setZero();
//
//	//std::cout << m_collisionShapes.size() << std::endl;
//
//	/*btIDebugDraw tempp;
//	m_dynamicsWorld->setDebugDrawer(btIDebugDraw::DebugDrawModes::DBG_MAX_DEBUG_DRAW_MODE);
//	m_dynamicsWorld->deb*/
//}
//
//// De-constructor (not implemented)
//PhysicsEngine::~PhysicsEngine() {};
//
//// Create a static rigid body
//void PhysicsEngine::CreateStaticRigidBody(Vector3 &pos)
//{
//	// Create a new collision object
//	btCollisionObject* colObject = new btCollisionObject();
//
//	// Set origin of collision object
//	btVector3 temp = btVector3(pos.x, pos.y, pos.z);
//	colObject->getWorldTransform().setOrigin(temp);
//
//	// Create a box shape
//	btCollisionShape* boxShape;
//	boxShape = new btBoxShape(btVector3(btScalar(50), btScalar(50), btScalar(50)));
//
//	// Set shape to object
//	colObject->setCollisionShape(boxShape);
//
//	// Add object to collision world
//	m_collisionWorld->addCollisionObject(colObject);
//
//	//btCollisionShape* groundShape;
//	//groundShape = new btBoxShape(btVector3(btScalar(50), btScalar(50), btScalar(50)));
//	//m_collisionShapes.push_back(groundShape);
//
//	//btVector3 temp = btVector3(pos.x, pos.y, pos.z);
//	//temp.setX(temp.getX() - 3000);
//	//temp.setZ(temp.getZ() - 50);
//
//	// Initialize transform and location
//	//btTransform groundTransform;
//	//groundTransform.setIdentity();
//	//groundTransform.setOrigin(temp);
//
//	// Set mass (zero for static)
//	//m_mass = 0.0;
//
//	// Set dynamic objects to objects with mass that is non-zero
//	//m_isDynamic = (m_mass != 0.0f);
//
//	//btVector3 localInertia(0.0, 0.0, 0.0);
//
//	//if (m_isDynamic)
//	//groundShape->calculateLocalInertia(m_mass, localInertia);
//
//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
//	//btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//	//btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, groundShape, localInertia);
//	//btRigidBody* body = new btRigidBody(rbInfo);
//
//	// Set the index for the type of rigid body that is being created
//	//body->setUserIndex(PLANE);
//
//	// Add the body to the dynamic world
//	//m_collisionWorld->addRigidBody(body);
//}
//
//// Create a dynamic rigid body
//void PhysicsEngine::CreateDynamicRigidBody(Vector3 &pos, std::string objType)
//{
//	// Does object data already exist
//	bool objExists = false;
//
//	// Create a new collision object
//	btCollisionObject* colObject = new btCollisionObject();
//
//	// Set origin of collision object
//	btVector3 temp = btVector3(pos.x, pos.y, pos.z);
//	colObject->getWorldTransform().setOrigin(temp);
//
//	// Create box shape
//	btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(50), btScalar(50), btScalar(50)));
//
//	// Set shape to object
//	colObject->setCollisionShape(boxShape);
//
//	// Search vector of object types to see if object data has already been created
//	for (int i = 0; i < m_ObjectTypePhysicsData.size(); i++)
//	{
//		// If object data already exists, set flag true
//		if (objType == m_ObjectTypePhysicsData[i]->objType)
//			objExists = true;
//	}
//
//	// If object data does not exist, create it
//	if (!objExists)
//	{
//		// New object data
//		ObjectTypePhysicsData* objPhysicsData = new ObjectTypePhysicsData();
//
//		// Set type of object
//		objPhysicsData->objType = objType;
//
//		// Mass
//		m_mass = 5;
//		//if (objType == "box")
//		//m_mass = 5000;
//
//		// Size of object
//		Vector3 size = { 100, 100, 100 };
//
//		// Make new PointMass data and initialize it
//		std::vector<PointMass> pointMassData;
//		InitializePointMass(pointMassData, m_mass, size);
//
//		// Calc center of gravity
//		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);
//
//		// Calc relative positions of pointmass data
//		CalcPointMassRelativePositions(pointMassData, objPhysicsData);
//
//		// Calc second moment of mass (inertia)
//		CalcObjectSecondMoment(objPhysicsData, size);
//
//		// Add object data to vector
//		m_ObjectTypePhysicsData.push_back(objPhysicsData);
//	}
//
//	// Add object to collision world
//	m_collisionWorld->addCollisionObject(colObject);
//
//	// Add object rigid body data to vector
//	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
//	objRigidBodyData->objType = objType;
//
//	if (objType == "box2")
//		objRigidBodyData->velBeforeCol.Set(1, -.5);
//	if (objType == "box")
//		objRigidBodyData->velBeforeCol.Set(1, -0);
//	objRigidBodyData->currPos = pos;
//	m_objectRigidBodyData.push_back(objRigidBodyData);
//
//	//// Create box shape and add to shape array
//	//btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(50), btScalar(50), btScalar(50)));
//	//m_collisionShapes.push_back(boxShape);
//
//	//// Create a dynamic object
//	//btTransform startTransform;
//	//startTransform.setIdentity();
//
//	//// Set mass (non-zero for dynamic)
//	//m_mass = 100.0f;
//
//	//// Set dynamic objects to objects with mass that is non-zero
//	//m_isDynamic = (m_mass != 0.0f);
//
//	//btVector3 localInertia(0.0, 0.0, 0.0);
//
//	//if (m_isDynamic)
//	//	boxShape->calculateLocalInertia(m_mass, localInertia);
//
//	//// Set origin of body
//	//startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
//
//	////using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//	//btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	//btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, boxShape, localInertia);
//	//btRigidBody* body = new btRigidBody(rbInfo);
//
//	//// Set the index for the type of rigid body that is being created
//	//body->setUserIndex(BOX);
//	//
//	//// Add the body to the dynamic world
//	//m_collisionWorld->addRigidBody(body);
//}
//
//// Simulate the dynamic world
//void PhysicsEngine::Simulate(std::vector<Vector3> &bodyPos, std::vector<Quaternion> &bodyRot)
//{
//	// Time step
//	game_time += 0.1;
//	physics_lag_time += (game_time - prev_game_time);
//
//
//	// Collision objects that are colliding
//	btCollisionObject* objA;
//	btCollisionObject* objB;
//
//	// Data of colliding rigid bodies
//	ObjectRigidBodyData* rbA = m_objectRigidBodyData[0];
//	ObjectRigidBodyData* rbB = m_objectRigidBodyData[1];
//
//	// Data
//	btVector3 ptA = { 0, 0, 0 };
//	btVector3 ptB = { 0, 0, 0 };
//
//	while (physics_lag_time > delta_t)
//	{
//		//m_collisionWorld->stepSimulation(1.f / 60.f, 10);
//
//		//// Collision objects that are colliding
//		//btCollisionObject* objA;
//		//btCollisionObject* objB;
//
//		//// Data of colliding rigid bodies
//		//ObjectRigidBodyData* rbA = m_objectRigidBodyData[0];
//		//ObjectRigidBodyData* rbB = m_objectRigidBodyData[1];
//
//		//// Data
//		ptA = { 0, 0, 0 };
//		ptB = { 0, 0, 0 };
//
//		// Check all objects for collision detection, and get number of collisions
//		m_collisionWorld->performDiscreteCollisionDetection();
//		int numManifolds = m_collisionWorld->getDispatcher()->getNumManifolds();
//
//		for (int i = 0; i < numManifolds; i++)
//		{
//			btPersistentManifold* contactManifold = m_collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
//			objA = const_cast<btCollisionObject*>(contactManifold->getBody0());
//			objB = const_cast<btCollisionObject*>(contactManifold->getBody1());
//			contactManifold->refreshContactPoints(objA->getWorldTransform(), objB->getWorldTransform());
//
//			int numContacts = contactManifold->getNumContacts();
//
//			count++;
//			std::cout << "collision " << count << std::endl;
//			// Get data for colliding objects
//			for (int j = m_collisionWorld->getNumCollisionObjects() - 1; j >= 0; j--)
//			{
//				btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];
//
//				if (obj == objA)
//					rbA = m_objectRigidBodyData[j];
//				if (obj == objB)
//					rbB = m_objectRigidBodyData[j];
//			}
//
//
//			//For each contact point in that manifold
//			for (int j = 0; j < numContacts; j++)
//			{
//				//Get the contact information
//				btManifoldPoint& pt = contactManifold->getContactPoint(j);
//				//ptA += pt.m_localPointA;
//				//ptB += pt.m_localPointB;
//				ptA += pt.getPositionWorldOnA();
//				ptB += pt.getPositionWorldOnB();
//				double ptdist = pt.getDistance();
//				pt.m_localPointA;
//
//				//std::cout << "COLLISION" << std::endl;
//				//std::cout << ptA.getX() << " " << ptA.getY() << " " << ptA.getZ() << std::endl;
//				//std::cout << ptB.getX() << " " << ptB.getY() << " " << ptB.getZ() << std::endl;
//
//				// Get pos and vel before collision
//
//
//
//				//// Find surface normal of collision (check this)
//				//m_normal = Vector3(ptA.getX(), ptA.getY(), ptA.getZ());
//				//m_normal = m_normal.Normalized();
//
//
//				//// Calculate impulses (eq 13)
//				//Vector3 tempA = rbA->velBeforeCol;
//				//Vector3 tempB = rbB->velBeforeCol;
//				//m_impulse = m_normal;
//				//m_impulse = m_impulse * (1 + m_epsilon) * m_ObjectTypePhysicsData[0]->totalMass * m_ObjectTypePhysicsData[0]->totalMass;
//				//m_impulse = m_impulse * m_normal.Dot(m_normal, tempA - tempB);
//				//m_impulse = m_impulse / (m_ObjectTypePhysicsData[0]->totalMass + m_ObjectTypePhysicsData[0]->totalMass);
//
//				//// Get pos and vel after collision
//				//rbA->velBeforeCol = rbA->velBeforeCol + (m_impulse / m_ObjectTypePhysicsData[0]->totalMass);
//				//rbB->velBeforeCol = rbB->velBeforeCol + (m_impulse / m_ObjectTypePhysicsData[0]->totalMass);
//			}
//
//			ptA /= numContacts;
//			ptB /= numContacts;
//			Vector3 tempAA = rbA->currPos;
//			Vector3 tempBB = rbB->currPos;
//			Vector3 centerAToCol = tempAA - Vector3(ptA.getX(), ptA.getY(), ptA.getZ());
//			Vector3 centerBToCol = tempBB - Vector3(ptB.getX(), ptB.getY(), ptB.getZ());
//			// Get pos before collision
//
//
//			// Find surface normal of collision (check this)
//			Vector3 empty;// = { 1, 2, 1 };
//						  //m_normal = m_normal.Normal(Vector3(ptA.getX(), ptA.getY(), ptA.getZ()), empty, Vector3(ptB.getX(), ptB.getY(), ptB.getZ()));
//						  //m_normal = Vector3(ptA.getX(), ptA.getY(), ptA.getZ());
//						  //m_normal = Vector3(ptA.getX() - ptB.getX(), ptA.getY() - ptB.getY(), ptA.getZ() - ptB.getZ());
//						  //m_normal = m_normal.Normal(Vector3(ptA.getX(), ptA.getY(), ptA.getZ()) - rbA->currPos, Vector3(ptA.getX(), ptA.getY(), ptA.getZ()), Vector3(ptA.getX(), ptA.getY(), ptA.getZ()) - rbB->currPos);
//			m_normal = centerAToCol - centerBToCol;
//			m_normal = m_normal.Normalized();
//
//
//			// Calculate impulses (eq 13)
//			Vector3 tempA = rbA->velBeforeCol;
//			Vector3 tempB = rbB->velBeforeCol;
//			Vector3 tempNormal = m_normal;
//			Vector3 tempImpulse;
//			Vector3 tempCalc;
//			Vector3 tempCalc2;
//			Vector3 tempDivide;
//			Vector3 tempFinal;
//
//			//tempCalc = (tempA - tempB) * (1 + m_epsilon) * m_ObjectTypePhysicsData[0]->totalMass * m_ObjectTypePhysicsData[1]->totalMass;
//			////tempCalc2 = tempImpulse * tempCalc;
//			//tempImpulse = tempImpulse.Dot(tempCalc, tempNormal);
//			//tempDivide = tempImpulse / (m_ObjectTypePhysicsData[0]->totalMass + m_ObjectTypePhysicsData[1]->totalMass);
//			//tempDivide * -1;
//			//tempFinal = tempDivide * tempNormal;
//			//m_impulse = tempFinal;
//
//			tempImpulse = tempImpulse.Dot(tempA - tempB, tempNormal);
//			tempCalc = (1 + m_epsilon) * m_ObjectTypePhysicsData[0]->totalMass * m_ObjectTypePhysicsData[1]->totalMass;
//			tempCalc2 = tempImpulse * tempCalc;
//			tempDivide = tempCalc2 / (m_ObjectTypePhysicsData[0]->totalMass + m_ObjectTypePhysicsData[1]->totalMass);
//			tempDivide * -1;
//			tempFinal = tempDivide * tempNormal;
//			m_impulse = tempFinal;
//
//
//			//m_impulse = m_normal;
//			//m_impulse = m_impulse * -(1 + m_epsilon) * m_ObjectTypePhysicsData[0]->totalMass * m_ObjectTypePhysicsData[1]->totalMass;
//			//m_impulse = m_impulse * m_normal.Dot(m_normal, tempA - tempB);
//			//m_impulse = m_impulse / (m_ObjectTypePhysicsData[0]->totalMass + m_ObjectTypePhysicsData[1]->totalMass);
//			//m_impulse = m_impulse.Cross(m_impulse, m_normal);
//
//			// Get pos and vel after collision (eq 9 and eq 10)
//			rbA->velBeforeCol = rbA->velBeforeCol + (m_impulse / m_ObjectTypePhysicsData[0]->totalMass);
//			rbB->velBeforeCol = rbB->velBeforeCol - (m_impulse / m_ObjectTypePhysicsData[1]->totalMass);
//
//			//rbA->currPos.Set(1, rbA->currPos.y + 1);
//
//			//objB->getWorldTransform().setOrigin(btVector3(rbB->prevPos.x, rbB->prevPos.y, rbB->prevPos.z));
//		}
//
//		/*****************************************************************/
//		// Update positions of all objects
//		for (int j = m_collisionWorld->getNumCollisionObjects() - 1; j >= 0; j--)
//		{
//			btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];
//			btTransform temp = obj->getWorldTransform();
//			btVector3 temp2 = temp.getOrigin();
//
//
//			//std::cout << temp2.getX() << " " << temp2.getY() << " " << temp2.getZ() << std::endl;
//			//std::cout << temp2.getX() << " " << temp2.getY() << " " << temp2.getZ() << std::endl;
//			//if (j == 1)
//			//{
//			//std::cout << temp2.getX() << " " << temp2.getY() << " " << temp2.getZ() << std::endl;
//			//temp2.setY(temp2.getY() + m_objectRigidBodyData[j]->velBeforeCol.y);
//			temp2.setValue(temp2.getX() + m_objectRigidBodyData[j]->velBeforeCol.x,
//				temp2.getY() + m_objectRigidBodyData[j]->velBeforeCol.y,
//				temp2.getZ() + m_objectRigidBodyData[j]->velBeforeCol.z);
//			temp.setOrigin(temp2);
//			obj->setWorldTransform(temp);
//			//}
//
//			// Update object positions for drawing
//			bodyPos[j].Set(0, temp.getOrigin().getX());
//			bodyPos[j].Set(1, temp.getOrigin().getY());
//			bodyPos[j].Set(2, temp.getOrigin().getZ());
//
//			m_objectRigidBodyData[j]->prevPos.Set(0, m_objectRigidBodyData[j]->currPos.x);
//			m_objectRigidBodyData[j]->prevPos.Set(1, m_objectRigidBodyData[j]->currPos.y);
//			m_objectRigidBodyData[j]->prevPos.Set(2, m_objectRigidBodyData[j]->currPos.z);
//
//			m_objectRigidBodyData[j]->currPos.Set(0, temp.getOrigin().getX());
//			m_objectRigidBodyData[j]->currPos.Set(1, temp.getOrigin().getY());
//			m_objectRigidBodyData[j]->currPos.Set(2, temp.getOrigin().getZ());
//		}
//
//		// Decrease physics lag time
//		physics_lag_time -= delta_t;
//	}
//
//	prev_game_time = game_time;
//
//
//	// Update object positions and rotations for drawing
//	//	bodyPos[j].Set(0, trans.getOrigin().getX());
//	//	bodyPos[j].Set(1, trans.getOrigin().getY());
//	//	bodyPos[j].Set(2, trans.getOrigin().getZ());
//
//	//	/*Quaternion temp = { trans.getRotation().getW(),
//	//						trans.getRotation().getX(),
//	//						trans.getRotation().getY(),
//	//						trans.getRotation().getZ() };
//
//	//	temp = temp * bodyRot[j];*/
//
//	//	bodyRot[j].Set(0, trans.getRotation().getW());
//	//	bodyRot[j].Set(1, -trans.getRotation().getX());
//	//	bodyRot[j].Set(2, -trans.getRotation().getY());
//	//	bodyRot[j].Set(3, -trans.getRotation().getZ());
//
//	//	float angleRad = trans.getRotation().getAngle();
//	//	btVector3 axisOfRot = trans.getRotation().getAxis();
//	//	std::cout << bodyRot[j].w << " " << bodyRot[j].x << " " << bodyRot[j].y << " " << bodyRot[j].z << std::endl;
//	//	
//
//	/*bodyRot[j].Set(0, cos(angleRad / 2));
//	bodyRot[j].Set(1, axisOfRot.getX() * (sin(angleRad/2)));
//	bodyRot[j].Set(2, axisOfRot.getY() * (sin(angleRad / 2)));
//	bodyRot[j].Set(3, axisOfRot.getZ() * (sin(angleRad / 2)));
//
//	std::cout << bodyRot[j].w << " " << bodyRot[j].x << " " << bodyRot[j].y << " " << bodyRot[j].z << std::endl;*/
//	//trans.getRotation().getEulerZYX(bodyRot[j].x, bodyRot[j].y, bodyRot[j].z);
//	//std::cout << bodyRot[j].x << " " << bodyRot[j].y << " " << bodyRot[j].z << std::endl;
//}
//
//// Ensure all objects are activated for physics simulation
//void PhysicsEngine::ActivateAllObjects()
//{
//	// Loop through every rigid body object
//	for (int j = m_collisionWorld->getNumCollisionObjects() - 1; j >= 0; j--)
//	{
//		// Get the next object, and activate it
//		btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];
//		obj->forceActivationState(DISABLE_DEACTIVATION);
//	}
//}
//
//// Initialize the point mass data for an object
//void PhysicsEngine::InitializePointMass(std::vector<PointMass> &pointMassVect, btScalar mass, Vector3 size)
//{
//	// Data
//	float currX = 0.5;
//	float currY = 0.5;
//	float currZ = 0.5;
//	float numElements = size.x * size.y * size.z;
//
//	PointMass tempPointMass;
//
//	// Loop through all PointMass data points
//	for (int i = 0; i < numElements; i++)
//	{
//		// Set mass
//		tempPointMass.mass = mass / numElements;
//
//		// Loop through each axis
//		if (currX >= size.x)
//		{
//			currX = 0.5;
//			currY += 1;
//		}
//		if (currY >= size.y)
//		{
//			currZ += 1;
//			currY = 0.5;
//		}
//		if (currZ >= size.z)
//			currZ = 0.5;
//
//		// Set actual position of point mass
//		tempPointMass.actualPosition = Vector3(currX, currY, currZ);
//
//		// Add to vector of pointmass data
//		pointMassVect.push_back(tempPointMass);
//
//		// Increment x pos by 1
//		currX += 1;
//	}
//}
//
//// Calculate objects center of gravity and first moment of mass
//void PhysicsEngine::CalcObjectCenterOfGravity(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &newObject)
//{
//	// Counter
//	int i;
//
//	// Data
//	//ObjectPhysicsData* newObject = new ObjectPhysicsData();
//	Vector3 firstMoment;
//	//newObject->totalMass = 0;
//
//
//	// Get total mass for object
//	for (i = 0; i < pointMassVect.size(); i++)
//		newObject->totalMass += pointMassVect[i].mass;
//
//	// Calculate first moment of mass
//	for (i = 0; i < pointMassVect.size(); i++)
//	{
//		Vector3 actualPos = pointMassVect[i].actualPosition;
//		float mass = pointMassVect[i].mass;
//		newObject->firstMoment = newObject->firstMoment + actualPos * mass;
//	}
//
//	// Calculate center of gravity for object
//	firstMoment = newObject->firstMoment;
//	newObject->combinedCG = firstMoment / newObject->totalMass;
//
//	//return newObject;
//}
//
//// Calculate an objects second moment of mass about the CoG
//void PhysicsEngine::CalcObjectSecondMoment(ObjectTypePhysicsData* &objectData, Vector3 size)
//{
//	float tempX = (1.0 / 12.0)*objectData->totalMass*((size.y * size.y) + (size.z * size.z));
//	float tempY = (1.0 / 12.0)*objectData->totalMass*((size.x * size.x) + (size.z * size.z));
//	float tempZ = (1.0 / 12.0)*objectData->totalMass*((size.y * size.y) + (size.x * size.x));
//	objectData->secondMoment.Set(0, tempX);
//	objectData->secondMoment.Set(1, tempY);
//	objectData->secondMoment.Set(2, tempZ);
//}
//
//// Calculate the relative positions of each PointMass data point for an object
//void PhysicsEngine::CalcPointMassRelativePositions(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &objectData)
//{
//	for (int i = 0; i < pointMassVect.size(); i++)
//	{
//		Vector3 actualPos = pointMassVect[i].actualPosition;
//		pointMassVect[i].relativePosition = actualPos - objectData->combinedCG;
//	}
//}
//
//*/
/*************************************NEW**************************************/
