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
	m_dynamicsWorld->setGravity(btVector3(0, 0, 0));

	// Initialize all objects to static
	m_isDynamic = false;

	// Initialize player object location
	m_playerObject.setZero();

	m_newForce.setZero();
}

// De-constructor (not implemented)
PhysicsEngine::~PhysicsEngine(){};

// Create a static rigid body
void PhysicsEngine::CreateStaticRigidBody(btVector3 &pos, std::string type)
{
	btCollisionShape* groundShape;

	if (type == "knight")
		groundShape = new btBoxShape(btVector3(btScalar(80.0), btScalar(100.0), btScalar(80.0)));
	else if (type == "rock")
		groundShape = new btBoxShape(btVector3(btScalar(225.0), btScalar(100.0), btScalar(200.0)));
	else
		groundShape = new btBoxShape(btVector3(btScalar(0.0), btScalar(0.0), btScalar(0.0)));

	m_collisionShapes.push_back(groundShape);

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
	btCollisionShape* camShape = new btBoxShape(btVector3(btScalar(30), btScalar(20), btScalar(50)));
	m_collisionShapes.push_back(camShape);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set origin of body
	startTransform.setOrigin(playerObj);

	// Set mass (non-zero for dynamic)
	m_mass = 2.0f;

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

	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);

	// Set new player object coordinates
	m_playerObject = playerObj;

	// Disable gravity for this object
	//body->setGravity(btVector3(0.0, 0.0, 0.0));
}

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(btVector3 &pos)
{
	// Create box shape and add to shape array
	btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(20), btScalar(50), btScalar(20)));
	m_collisionShapes.push_back(boxShape);
	
	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set mass (non-zero for dynamic)
	m_mass = 100.0f;

	// Set dynamic objects to objects with mass that is non-zero
	m_isDynamic = (m_mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (m_isDynamic)
		boxShape->calculateLocalInertia(m_mass, localInertia);

	// Set origin of body
	startTransform.setOrigin(pos);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, boxShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(BOX);
	
	// Add the body to the dynamic world
	m_dynamicsWorld->addRigidBody(body);
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<btVector3> &bodyPos, btVector3 &playerObj)
{
	m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);


	/*****************************************************************/
	m_dynamicsWorld->performDiscreteCollisionDetection();

	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();

	std::cout << numManifolds << std::endl;
	/*****************************************************************/

	// Update positions of all dynamic objects
	for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		/**
		// Get the next object, and activate it
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		// Reset forces on player object prior to next step simulation
		if (body->getUserIndex() == CAMERA)
		{
			//body->clearForces();
			btVector3 tempVel = body->getLinearVelocity();
			//body->setLinearVelocity(btVector3(0,tempVel.getY(),0));
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
			m_newForce.setX((playerObj.x() - m_playerObject.x()) * 10000);
			//m_newForce.setY((playerObj.y() - m_playerObject.y()) * 10000);
			m_newForce.setZ((playerObj.z() - m_playerObject.z()) * 10000);

			// Update rigid body location for drawing
			body->applyCentralForce(m_newForce);
			m_playerObject = trans.getOrigin();
			playerObj = m_playerObject;
		}
		else
		{
			// Update object positions for drawing
			bodyPos[j].setX(trans.getOrigin().getX());
			bodyPos[j].setY(trans.getOrigin().getY());
			bodyPos[j].setZ(trans.getOrigin().getZ());
		}	
		*/
	}
}

// Testing for creating a heightfield terrain shape (Can delete)
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

// Activate all dynamic objects so they continue to simulate physics
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
