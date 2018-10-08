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
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Initialize all objects to static
	m_isDynamic = false;

	// Initialize player object location
	m_playerObject.setZero();

	m_oldForce.setZero();
	m_newForce.setZero();

	m_quad = gluNewQuadric();

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
	groundShape = new btBoxShape(btVector3(btScalar(100000), btScalar(200), btScalar(100000)));

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
	//btCollisionShape* camShape = new btBoxShape(btVector3(btScalar(30), btScalar(20), btScalar(50)));
	btCollisionShape* camShape = new btCapsuleShape(100, 200);
	//btCollisionShape* camShape = new btSphereShape(10);
	m_collisionShapes.push_back(camShape);

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
void PhysicsEngine::CreateDynamicRigidBody(btVector3 &pos)
{
	// Create box shape and add to shape array
	btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(400), btScalar(400), btScalar(400)));
	m_collisionShapes.push_back(boxShape);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();
	// Set origin of body
	startTransform.setOrigin(pos);

	// Set mass (non-zero for dynamic)
	m_mass = 1.0;

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

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<btVector3> &bodyPos, btVector3 &playerObj)
{
	m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	// Update positions of all dynamic objects
	for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
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
			m_newForce.setX((playerObj.x() - m_playerObject.x()) * 20000);
			//m_newForce.setY((playerObj.y() - m_playerObject.y()) * 10000);
			m_newForce.setZ((playerObj.z() - m_playerObject.z()) * 20000);

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
	}
	//std::cout << "/n/n/n/n/n" << std::endl;
}

// Create a dynamic rigid body
btRigidBody* PhysicsEngine::AddSphere(float radius, btVector3 &startPos)
{
	// Create box shape and add to shape array
	btCollisionShape* sphereShape = new btSphereShape(radius);
	m_collisionShapes.push_back(sphereShape);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();
	// Set origin of body
	startTransform.setOrigin(startPos);

	// Set mass (non-zero for dynamic)
	m_mass = 1.0;

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

void PhysicsEngine::RenderSphere(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
		return;
	glColor3f(1.0, 0.0, 0.0);

	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);

	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
		glMultMatrixf(mat);	// translation and rotation
		gluSphere(m_quad, r, 20, 20);
	glPopMatrix();
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

btCollisionObject* PhysicsEngine::TriangleMeshTest(std::vector<Mesh> &modelMesh, btVector3 &pos, bool useQuantizedBvhTree, bool collision)
{
	btTriangleMesh* trimesh = new btTriangleMesh();
	//std::cout << modelIndices.size()  << " and " << modelMesh.size() << std::endl;
	for (int j = 0; j < modelMesh.size(); j++)
	{
		Mesh tempMesh = modelMesh[j];
		std::vector<unsigned int> tempMeshIndice = tempMesh.GetIndices();
		std::vector<Vertex3> tempMeshVertex = tempMesh.GetVertices();

		for (int i = 0; i < tempMeshIndice.size(); i += 3)
		{
			glm::vec3 x = tempMeshVertex[tempMeshIndice[i]].m_position;
			glm::vec3 y = tempMeshVertex[tempMeshIndice[i+1]].m_position;
			glm::vec3 z = tempMeshVertex[tempMeshIndice[i+2]].m_position;
			/*glm::vec3 x = modelMesh[i].m_position;
			glm::vec3 y = modelMesh[i + 1].m_position;
			glm::vec3 z = modelMesh[i + 2].m_position;*/

			btVector3 p0, p1, p2;

			p0.setX(x.x);
			p0.setY(x.y);
			p0.setZ(x.z);
			p1.setX(y.x);
			p1.setY(y.y);
			p1.setZ(y.z);
			p2.setX(z.x);
			p2.setY(z.y);
			p2.setZ(z.z);

			trimesh->addTriangle(p0, p1, p2);
		}
	}

	

	btTransform	trans;
	trans.setIdentity();
	trans.setOrigin(pos);

	trimesh->setScaling(btVector3(100, 100, 100));

	btCollisionShape* trimeshShape = new btBvhTriangleMeshShape(trimesh, useQuantizedBvhTree);
	m_collisionShapes.push_back(trimeshShape);

	btVector3 inertia(0, 0, 0);
	//trimeshShape->calculateLocalInertia(0, inertia); //gives error

	btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
	btRigidBody* body = new btRigidBody(0, motionstate, trimeshShape, inertia);



	//m_trianglemeshs.push_back(trimesh);
	//m_triangleMeshBodies.push_back(body);
	body->setUserIndex(SPHERE);
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
