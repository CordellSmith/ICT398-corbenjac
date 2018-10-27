/*
* Implementation of PhysicsEngine.h file
* Author - Jack Matters
* Note - Bullet Physics Engine documentation http://bulletphysics.org/wordpress/?page_id=9
*/

//Includes
#include "PhysicsEngine.h"
#include <iostream>

#define NUMSTATE 4

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
	m_collisionWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// Initialize epsilon
	m_epsilon = .5;

	count = 0;
	delta_t = 0.02;
	game_time = 0.0;
	prev_game_time = 0.0;
	physics_lag_time = 0.0;

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
void PhysicsEngine::CreateStaticRigidBody(glm::vec3 &pos)
{
	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

	// Set origin of collision object
	btVector3 temp = btVector3(pos.x, pos.y, pos.z);
	colObject->getWorldTransform().setOrigin(temp);

	// Create a box shape
	btCollisionShape* boxShape;
	boxShape = new btBoxShape(btVector3(btScalar(50), btScalar(50), btScalar(50)));

	// Set shape to object
	colObject->setCollisionShape(boxShape);

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);
}

// Create a bounding box for camera or player controlled object
void PhysicsEngine::CreatePlayerControlledRigidBody(glm::vec3 &playerObj)
{
	// Does object data already exist
	bool objExists = false;

	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

	// Create box shape and add to shape array
	//btCollisionShape* camShape = new btBoxShape(btVector3(btScalar(30), btScalar(20), btScalar(50)));
	btCollisionShape* camShape = new btCapsuleShape(100, 200);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set origin of body
	btVector3 temp = btVector3(playerObj.x, playerObj.y, playerObj.z);
	startTransform.setOrigin(temp);

	colObject->setCollisionShape(camShape);

	// Search vector of object types to see if object data has already been created
	for (int i = 0; i < m_ObjectTypePhysicsData.size(); i++)
	{
		// If object data already exists, set flag true
		if ("player" == m_ObjectTypePhysicsData[i]->objType)
			objExists = true;
	}

	// If object data does not exist, create it
	if (!objExists)
	{
		// New object data
		ObjectTypePhysicsData* objPhysicsData = new ObjectTypePhysicsData();

		// Set type of object
		objPhysicsData->objType = "player";

		// Mass
		m_mass = 5;

		// Size of object
		glm::vec3 size = { 10, 10, 10 };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);

		// Calc relative positions of pointmass data
		CalcPointMassRelativePositions(pointMassData, objPhysicsData);

		// Calc second moment of mass (inertia)
		CalcObjectSecondMoment(objPhysicsData, size);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Add object to collision world
	colObject->setUserIndex(CAMERA);
	m_collisionWorld->addCollisionObject(colObject);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = "player";

	objRigidBodyData->currLinearVel.y = -0.5f;

	objRigidBodyData->currPos = playerObj;
	m_objectRigidBodyData.push_back(objRigidBodyData);

	/***************************************************/
	m_currState.push_back(objRigidBodyData->currLinearVel);
	m_currState.push_back(objRigidBodyData->currPos);
	m_currState.push_back(objRigidBodyData->currAngularVel);
	m_currState.push_back(objRigidBodyData->angularMomentum);

	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));

	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));

	// Set mass (non-zero for dynamic)
	//m_mass = 1.0f;

	// Set dynamic objects to objects with mass that is non-zero
	//m_isDynamic = (m_mass != 0.0f);

	//btVector3 localInertia(0.0, 0.0, 0.0);

	//if (m_isDynamic)
		//camShape->calculateLocalInertia(m_mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	//btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, camShape, localInertia);
	//btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	//body->setUserIndex(CAMERA);
	//body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	//body->setCcdMotionThreshold(1e-7);
	//body->setCcdSweptSphereRadius(15);
	
	// Add the body to the dynamic world
	//m_dynamicsWorld->addRigidBody(body);

	// Set new player object coordinates
	m_playerObject = playerObj;

	// Disable gravity for this object
	//body->setGravity(btVector3(0.0, 0.0, 0.0));
}

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(glm::vec3 &pos, glm::vec3& dimensions, std::string objType)
{
	// Does object data already exist
	bool objExists = false;

	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

	// Set origin of collision object
	btVector3 temp = btVector3(pos.x, pos.y, pos.z);
	colObject->getWorldTransform().setOrigin(temp);

	// Create box shape size of the dimensions of the object
	btCollisionShape* boxShape = new btBoxShape(btVector3(
		btScalar(dimensions.x / 2),
		btScalar(dimensions.y / 2),
		btScalar(dimensions.z / 2))
	);

	// Set shape to object
	colObject->setCollisionShape(boxShape);

	// Search vector of object types to see if object data has already been created
	for (int i = 0; i < m_ObjectTypePhysicsData.size(); i++)
	{
		// If object data already exists, set flag true
		if (objType == m_ObjectTypePhysicsData[i]->objType)
			objExists = true;
	}

	// If object data does not exist, create it
	if (!objExists)
	{
		// New object data
		ObjectTypePhysicsData* objPhysicsData = new ObjectTypePhysicsData();

		// Set type of object
		objPhysicsData->objType = objType;

		// Mass
		m_mass = 5;

		// Size of object
		glm::vec3 size = { 100, 100, 100 };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);

		// Calc relative positions of pointmass data
		CalcPointMassRelativePositions(pointMassData, objPhysicsData);

		// Calc second moment of mass (inertia)
		CalcObjectSecondMoment(objPhysicsData, size);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;
	objRigidBodyData->currPos = pos;

	objRigidBodyData->currLinearVel.y = 50.0f;

	m_objectRigidBodyData.push_back(objRigidBodyData);

	m_currState.push_back(objRigidBodyData->currLinearVel);
	m_currState.push_back(objRigidBodyData->currPos);
	m_currState.push_back(objRigidBodyData->currAngularVel);
	m_currState.push_back(objRigidBodyData->angularMomentum);

	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));

	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
}


// Create a dynamic rigid body
void PhysicsEngine::AddSphere(float radius, glm::vec3 &startPos, std::string objType, glm::vec3 &startVel)
{
	// Does object data already exist
	bool objExists = false;
	
	// Create box shape and add to shape array
	btCollisionObject* colObject = new btCollisionObject();
	btCollisionShape* sphereShape = new btSphereShape(radius);

	// Create a dynamic object
	//btTransform startTransform;
	//startTransform.setIdentity();
	// Set origin of body
	btVector3 temp = btVector3(startPos.x, startPos.y, startPos.z);
	colObject->getWorldTransform().setOrigin(temp);

	colObject->setCollisionShape(sphereShape);

	// Search vector of object types to see if object data has already been created
	for (int i = 0; i < m_ObjectTypePhysicsData.size(); i++)
	{
		// If object data already exists, set flag true
		if (objType == m_ObjectTypePhysicsData[i]->objType)
			objExists = true;
	}

	// If object data does not exist, create it
	if (!objExists)
	{
		// New object data
		ObjectTypePhysicsData* objPhysicsData = new ObjectTypePhysicsData();

		// Set type of object
		objPhysicsData->objType = objType;

		// Mass
		m_mass = 5;

		// Size of object
		glm::vec3 size = { radius, radius, radius };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);

		// Calc relative positions of pointmass data
		CalcPointMassRelativePositions(pointMassData, objPhysicsData);

		// Calc second moment of mass (inertia)
		CalcObjectSecondMoment(objPhysicsData, size);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;

	objRigidBodyData->currPos = startPos;
	objRigidBodyData->currLinearVel = startVel;
	m_objectRigidBodyData.push_back(objRigidBodyData);

	/***************************************************/
	m_currState.push_back(objRigidBodyData->currLinearVel);
	m_currState.push_back(objRigidBodyData->currPos);
	m_currState.push_back(objRigidBodyData->currAngularVel);
	m_currState.push_back(objRigidBodyData->angularMomentum);

	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));

	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<CollisionBody*>& collisionBodies, std::vector<Quaternion> &bodyRot, glm::vec3& playerObj)
{
	// Time step
	game_time += 1.0;
	physics_lag_time += (game_time - prev_game_time);

	btVector3 btFrom(playerObj.x, playerObj.y, playerObj.z);
	btVector3 btTo(playerObj.z, -3000.0f, playerObj.z);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	m_collisionWorld->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld


												  // Number of collisions detected
	int numManifolds = 0;

	// Collision objects that are colliding
	btCollisionObject* objA;
	btCollisionObject* objB;

	// Data of colliding rigid bodies
	ObjectRigidBodyData* rbA = 0; //= m_objectRigidBodyData[0];
	ObjectRigidBodyData* rbB = 0; // m_objectRigidBodyData[0];
	ObjectTypePhysicsData* pdA = 0;// = m_ObjectTypePhysicsData[0];
	ObjectTypePhysicsData* pdB = 0;// m_ObjectTypePhysicsData[0];

	// Collision point data
	btVector3 ptA = { 0, 0, 0 };
	btVector3 ptB = { 0, 0, 0 };


	while (physics_lag_time > delta_t)
	{
		// Data
		ptA = { 0, 0, 0 };
		ptB = { 0, 0, 0 };

		rbA = 0;
		rbB = 0;
		pdA = 0;
		pdB = 0;

		// Data
		int posA;
		int posB;
		int i,j,k;

		// Check all objects for collision detection, and get number of collisions
		m_collisionWorld->performDiscreteCollisionDetection();
		numManifolds = m_collisionWorld->getDispatcher()->getNumManifolds();

		// Get info for each colliding pair
		for (i = 0; i < numManifolds; i++)
		{
			//std::cout << numManifolds << std::endl;
			btPersistentManifold* contactManifold = m_collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
			objA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			objB = const_cast<btCollisionObject*>(contactManifold->getBody1());
			contactManifold->refreshContactPoints(objA->getWorldTransform(), objB->getWorldTransform());

			// Get number of contact points on colliding bodies
			int numContacts = contactManifold->getNumContacts();

			// Get data for colliding objects
			for (j = 0; j < m_collisionWorld->getNumCollisionObjects(); j++)
				//for (int j = m_collisionWorld->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];

				if (obj == objA)
				{
					rbA = m_objectRigidBodyData[j];
					for (k = 0; k < m_ObjectTypePhysicsData.size(); k++)
					{
						if (m_ObjectTypePhysicsData[k]->objType == m_objectRigidBodyData[j]->objType)
							pdA = m_ObjectTypePhysicsData[k];
					}
					posA = j;
				}
				if (obj == objB)
				{
					rbB = m_objectRigidBodyData[j];
					for (k = 0; k < m_ObjectTypePhysicsData.size(); k++)
					{
						if (m_ObjectTypePhysicsData[k]->objType == m_objectRigidBodyData[j]->objType)
							pdB = m_ObjectTypePhysicsData[k];
					}
					posB = j;
				}
			}

			//if (pdA->objType == "ball" || pdB->objType == "ball")
				std::cout << "OBJ1: " << pdA->objType << " collided with OBJ2: " << pdB->objType << std::endl;

			//For each contact point in that manifold
			for (j = 0; j < numContacts; j++)
			{
				//Get the contact information
				btManifoldPoint& pt = contactManifold->getContactPoint(j);

				// Sum up all the contact points
				ptA += pt.getPositionWorldOnA();
				ptB += pt.getPositionWorldOnB();
			}

			// Get average contact point of both colliding objects
			ptA /= numContacts;
			ptB /= numContacts;

			// Get vector for distance from object center to the average collision point
			glm::vec3 centerAToCol = glm::vec3(rbA->currPos.x, rbA->currPos.y, rbA->currPos.z) - glm::vec3(ptA.getX(), ptA.getY(), ptA.getZ());
			glm::vec3 centerBToCol = rbB->currPos - glm::vec3(ptB.getX(), ptB.getY(), ptB.getZ());

			// Calculate normal of colliding objects
			m_normal = centerAToCol - centerBToCol;
			m_normal = Normalize(m_normal);

			// Calculate linear impulse
			btScalar tempImpulse;

			tempImpulse = DotProduct(rbA->currLinearVel - rbB->currLinearVel, m_normal);
			tempImpulse *= -(1 + m_epsilon) * pdA->totalMass * pdB->totalMass;
			tempImpulse /= (pdA->totalMass + pdB->totalMass);
			m_impulse = tempImpulse * m_normal;

			//btScalar numerator = DotProduct(m_normal, rbA->currLinearVel - rbB->currLinearVel) + (DotProduct(rbA->currAngularVel, CrossProduct(centerAToCol, m_normal))) + (DotProduct(rbB->currAngularVel, CrossProduct(centerBToCol, m_normal)));
			//numerator *= -(1.0f + m_epsilon);
			//glm::vec3 denominatorA; // = (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			//glm::vec3 transA = CrossProduct(centerAToCol, m_normal);
			//glm::vec3 transB = CrossProduct(centerBToCol, m_normal);
			//glm::vec3 negInert = (1.0f / pdA->secondMoment);
			//denominatorA = CrossProduct(centerAToCol, m_normal) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + CrossProduct(centerBToCol, m_normal) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			////denominatorA = glm::vec3(transA.z, transA.y, transA.x) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + glm::vec3(transB.z, transB.y, transB.x) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			//denominatorA += (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			//glm::vec3 angImpulse = numerator / denominatorA;
			//angImpulse *= m_normal;

			// Calculate object velocities after collision
			rbA->currLinearVel = rbA->currLinearVel + (m_impulse / pdA->totalMass);
			rbB->currLinearVel = rbB->currLinearVel - (m_impulse / pdB->totalMass);

			// Set new linear velocities to corresponding current values
			m_currState[posA * NUMSTATE] += m_impulse / pdA->totalMass;
			m_currState[posB * NUMSTATE] -= m_impulse / pdA->totalMass;


			/*****************************************************************/
			/**This works**/
			// Calc torgue
			rbA->torque = CrossProduct(centerAToCol, pdA->totalMass * rbA->currLinearVel);
			rbB->torque = CrossProduct(centerBToCol, pdB->totalMass * rbB->currLinearVel);

			// Calc angular acceleration
			rbA->angularAccel = rbA->torque / pdA->secondMoment;
			rbB->angularAccel = rbB->torque / pdB->secondMoment;

			// Calc new angular velocity
			rbA->prevAngularVel += rbA->currAngularVel;
			rbB->prevAngularVel += rbB->currAngularVel;

			rbA->currAngularVel += rbA->angularAccel * delta_t;
			rbB->currAngularVel += rbB->angularAccel * delta_t;

			rbA->angle -= rbA->currAngularVel * delta_t;
			rbB->angle += rbB->currAngularVel * delta_t;

			// Set new angular momentum to corresponding value
			//m_currState[posA * NUMSTATE + 3] = pdA->secondMoment * rbA->currAngularVel;
			//m_currState[posB * NUMSTATE + 3] = pdB->secondMoment * rbB->currAngularVel;
			/**This works**/

			btScalar numerator = DotProduct(m_normal, rbA->currLinearVel - rbB->currLinearVel) + (DotProduct(rbA->currAngularVel, CrossProduct(centerAToCol, m_normal))) + (DotProduct(rbB->currAngularVel, CrossProduct(centerBToCol, m_normal)));
			numerator *= -(1.0f + m_epsilon);
			glm::vec3 denominatorA; // = (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			glm::vec3 transA = CrossProduct(centerAToCol, m_normal);
			glm::vec3 transB = CrossProduct(centerBToCol, m_normal);
			glm::vec3 negInert = (1.0f / pdA->secondMoment);
			denominatorA = CrossProduct(centerAToCol, m_normal) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + CrossProduct(centerBToCol, m_normal) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			//denominatorA = glm::vec3(transA.z, transA.y, transA.x) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + glm::vec3(transB.z, transB.y, transB.x) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			denominatorA += (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			glm::vec3 angImpulse = numerator / denominatorA;
			angImpulse *= m_normal;


			//ExplicitEuler(rbA, delta_t);
			//ExplicitEuler(rbB, delta_t);
			//rbB->angularMomentum *= -1;

			//btScalar numerator = DotProduct(m_normal, rbA->currLinearVel - rbB->currLinearVel) + (DotProduct(rbA->currAngularVel, CrossProduct(centerAToCol, m_normal))) + (DotProduct(rbB->currAngularVel, CrossProduct(centerBToCol, m_normal)));
			//numerator *= -(1.0 + m_epsilon);
			//glm::vec3 denominatorA; // = (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			//denominatorA = CrossProduct(centerAToCol, m_normal) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + CrossProduct(centerBToCol, m_normal) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			//denominatorA += (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			//glm::vec3 angImpulse = numerator / denominatorA;
			//angImpulse *= 1;
			glm::vec3 tempU = CrossProduct(centerAToCol, m_normal);
			glm::vec3 tempV = CrossProduct(centerAToCol, m_normal);
			rbA->angularMomentum += angImpulse * CrossProduct(centerAToCol, m_normal);
			rbB->angularMomentum -= angImpulse * CrossProduct(centerBToCol, m_normal);
			//std::cout << rbA->angularMomentum.x << std::endl;
			m_currState[posA * NUMSTATE + 3] += angImpulse * CrossProduct(centerAToCol, m_normal);
			m_currState[posB * NUMSTATE + 3] += angImpulse * CrossProduct(centerBToCol, m_normal);

			//rbA->angularMomentum = rbA->angularMomentum * 180.0f / (float)M_PI;
			//rbB->angularMomentum = rbB->angularMomentum * 180.0f / (float)M_PI;

			//rbA->angularMomentum = rbA->angularMomentum * (float)M_PI / 180.0f;
			//rbB->angularMomentum = rbB->angularMomentum * (float)M_PI / 180.0f;
		}


		for (i = 0; i < m_objectRigidBodyData.size(); i++)
		{
			if(m_objectRigidBodyData[i]->objType.compare("ball0") == false ||
				m_objectRigidBodyData[i]->objType.compare("ball1") == false || 
				m_objectRigidBodyData[i]->objType.compare("ball2") == false || 
				m_objectRigidBodyData[i]->objType.compare("ball3") == false || 
				m_objectRigidBodyData[i]->objType.compare("ball4") == false)
				UpdateBodyPos(i);
			//m_derivState[4 * i] = CalcForceOnObject(i);
			///m_derivState[4 * i + 1] = UpdateBodyPos(i);
			//m_derivState[4 * i + 2] = calc
			///m_derivState[4 * i + 3] = calcTorque();
		}

		ExplicitEuler();

		// Messing with terrain tracking

		//printf("Collision at: <%.2f>\n", res.m_hitPointWorld.getY());

		// Update positions of all dynamic objects
		for (j = 0; j < m_collisionWorld->getNumCollisionObjects(); j++)
		{
			// Get the next object, and activate it
			btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];
			btTransform temp = obj->getWorldTransform();
			btVector3 temp2 = temp.getOrigin();
			//btRigidBody* body = btRigidBody::upcast(obj);
			//btTransform trans;

			// Reset forces on player object prior to next step simulation
			if (obj->getUserIndex() == CAMERA)
			{
				//body->clearForces();
				//btVector3 tempVel = body->getLinearVelocity();
				//body->setLinearVelocity(btVector3(0,-2,0));
				//body->setLinearVelocity(btVector3(0, 0, 0));
			}

			// Update rigid body positions
			/*if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}*/

			// Check to see if player object
			if (obj->getUserIndex() == CAMERA)
			{
				// TODO: Make this better (Jack)
				// Apply force in direction camera was moved
				//m_newForce.x = ((playerObj.x - m_playerObject.x) * 3000);
				//m_newForce.setY((playerObj.y() - m_playerObject.y()) * 3000);
				//m_newForce.x = ((playerObj.z - m_playerObject.z) * 3000);


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
				//body->applyCentralForce(m_newForce);
				//m_playerObject = trans.getOrigin();
				playerObj = m_playerObject;
			}
			else
			{
				// Update object positions for drawing
				//collisionBodies[j]->m_position.x = (trans.getOrigin().getX());
				//collisionBodies[j]->m_position.y = (trans.getOrigin().getY());
				//collisionBodies[j]->m_position.z = (trans.getOrigin().getZ());
				//temp.setOrigin(btVector3(m_currState[j * NUMSTATE + 1].x, m_currState[j * NUMSTATE + 1].y, m_currState[j * NUMSTATE + 1].z));
				//obj->setWorldTransform(temp);

				collisionBodies[j]->m_position = m_currState[j * NUMSTATE + 1];

				/*m_objectRigidBodyData[j]->prevPos.x = m_objectRigidBodyData[j]->currPos.x;
				m_objectRigidBodyData[j]->prevPos.y = m_objectRigidBodyData[j]->currPos.y;
				m_objectRigidBodyData[j]->prevPos.z = m_objectRigidBodyData[j]->currPos.z;

				m_objectRigidBodyData[j]->currPos.x = temp.getOrigin().getX();
				m_objectRigidBodyData[j]->currPos.y = temp.getOrigin().getY();
				m_objectRigidBodyData[j]->currPos.z = temp.getOrigin().getZ();*/
			}
		}

		physics_lag_time -= delta_t;
		//std::cout << "/n/n/n/n/n" << std::endl;
	}
	prev_game_time = game_time;
}

// Testing for creating a heightfield terrain shape
void PhysicsEngine::CreateHeightfieldTerrainShape()
{
	////unsigned char *terrainData;
	//int sizet = 128;

	//// Open for binary read, print error if error found
	//std::ifstream infile("Resources/terrain/height128.raw", std::ios::binary);
	//if (!infile)
	//{
	//	exit(0);
	//}

	//// Allocate memory, return false if no size = 0
	//if (m_terrainData)
	//	delete[] m_terrainData;
	//if (sizet>0)
	//	m_terrainData = new unsigned char[sizet*sizet];
	//if (m_terrainData == NULL)
	//	exit(0);

	//// Read in heightfield and get length of file
	//infile.seekg(0, std::ios::end);
	//int length = infile.tellg();

	//// Read data in as a block, cast to char*, set size, and close file
	//infile.seekg(0, std::ios::beg);
	//infile.read(reinterpret_cast<char *>(m_terrainData), length);
	//infile.close();
	//this->m_size = sizet;

	//// Create heightfield shape
	//btCollisionShape* heightfieldShape = new btHeightfieldTerrainShape(128, 128, &m_terrainData, 1, 1, 128, 1, PHY_UCHAR, false);
	//m_collisionShapes.push_back(heightfieldShape);

	//// Initialize transform and location
	//btTransform startTransform;
	//startTransform.setIdentity();
	//startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));

	//// Set mass (non-zero for dynamic)
	//m_mass = 0.0;
	//m_isDynamic = (m_mass != 0.0f);

	//// Set initial inertia
	//btVector3 localInertia(0.0, 0.0, 0.0);

	//// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	//btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, myMotionState, heightfieldShape, localInertia);
	//btRigidBody* body = new btRigidBody(rbInfo);

	//// Set the index for the type of rigid body that is being created
	//body->setUserIndex(HEIGHTFIELD);

	//// Add the body to the dynamic world
	//m_dynamicsWorld->addRigidBody(body);
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

void PhysicsEngine::TriangleMeshTest(std::vector<Mesh> &modelMesh, bool useQuantizedBvhTree, bool collision, std::string objType)
{
	// Does object data already exist
	bool objExists = false;

	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

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

	colObject->setCollisionShape(trimeshShape);

	// Search vector of object types to see if object data has already been created
	//for (int i = 0; i < m_ObjectTypePhysicsData.size(); i++)
	//{
	//	// If object data already exists, set flag true
	//	if (objType == m_ObjectTypePhysicsData[i]->objType)
	//		objExists = true;
	//}

	//// If object data does not exist, create it
	//if (!objExists)
	//{
	//	// New object data
	//	ObjectTypePhysicsData* objPhysicsData = new ObjectTypePhysicsData();

	//	// Set type of object
	//	objPhysicsData->objType = objType;

	//	// Mass
	//	m_mass = 50000000;

	//	// Size of object
	//	glm::vec3 size = { 10, 10, 10 };

	//	// Make new PointMass data and initialize it
	//	std::vector<PointMass> pointMassData;
	//	InitializePointMass(pointMassData, m_mass, size);

	//	// Calc center of gravity
	//	CalcObjectCenterOfGravity(pointMassData, objPhysicsData);

	//	// Calc relative positions of pointmass data
	//	CalcPointMassRelativePositions(pointMassData, objPhysicsData);

	//	// Calc second moment of mass (inertia)
	//	CalcObjectSecondMoment(objPhysicsData, size);

	//	// Add object data to vector
	//	m_ObjectTypePhysicsData.push_back(objPhysicsData);
	//}

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;

	objRigidBodyData->currPos = glm::vec3(modelMesh[0].GetPosition().x, modelMesh[0].GetPosition().y, modelMesh[0].GetPosition().z);
	m_objectRigidBodyData.push_back(objRigidBodyData);

	/***************************************************/
	m_currState.push_back(objRigidBodyData->currLinearVel);
	m_currState.push_back(objRigidBodyData->currPos);
	m_currState.push_back(objRigidBodyData->currAngularVel);
	m_currState.push_back(objRigidBodyData->angularMomentum);

	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));
	m_prevState.push_back(glm::vec3(0));

	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
	m_derivState.push_back(glm::vec3(0));
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
	//glDrawArrays(GL_LINES, 0, sizeof(m_debugLines[0]) * m_debugLines.size());

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

/*************************************NEW**************************************/


// Initialize the point mass data for an object
void PhysicsEngine::InitializePointMass(std::vector<PointMass> &pointMassVect, btScalar mass, glm::vec3 size)
{
	// Data
	float currX = 0.5;
	float currY = 0.5;
	float currZ = 0.5;
	float numElements = size.x * size.y * size.z;

	PointMass tempPointMass;

	// Loop through all PointMass data points
	for (int i = 0; i < numElements; i++)
	{
		// Set mass
		tempPointMass.mass = mass / numElements;

		// Loop through each axis
		if (currX >= size.x)
		{
			currX = 0.5;
			currY += 1;
		}
		if (currY >= size.y)
		{
			currZ += 1;
			currY = 0.5;
		}
		if (currZ >= size.z)
			currZ = 0.5;

		// Set actual position of point mass
		tempPointMass.actualPosition = glm::vec3(currX, currY, currZ);

		// Add to vector of pointmass data
		pointMassVect.push_back(tempPointMass);

		// Increment x pos by 1
		currX += 1;
	}
}

// Calculate objects center of gravity and first moment of mass
void PhysicsEngine::CalcObjectCenterOfGravity(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &newObject)
{
	// Counter
	int i;

	// Data
	//ObjectPhysicsData* newObject = new ObjectPhysicsData();
	glm::vec3 firstMoment;
	//newObject->totalMass = 0;


	// Get total mass for object
	for (i = 0; i < pointMassVect.size(); i++)
		newObject->totalMass += pointMassVect[i].mass;

	// Calculate first moment of mass
	for (i = 0; i < pointMassVect.size(); i++)
	{
		glm::vec3 actualPos = pointMassVect[i].actualPosition;
		float mass = pointMassVect[i].mass;
		newObject->firstMoment = newObject->firstMoment + actualPos * mass;
	}

	// Calculate center of gravity for object
	firstMoment = newObject->firstMoment;
	newObject->combinedCG = firstMoment / newObject->totalMass;

	//return newObject;
}

// Calculate an objects second moment of mass about the CoG
void PhysicsEngine::CalcObjectSecondMoment(ObjectTypePhysicsData* &objectData, glm::vec3 size)
{
	float tempX = (1.0 / 12.0)*objectData->totalMass*((size.y * size.y) + (size.z * size.z));
	float tempY = (1.0 / 12.0)*objectData->totalMass*((size.x * size.x) + (size.z * size.z));
	float tempZ = (1.0 / 12.0)*objectData->totalMass*((size.y * size.y) + (size.x * size.x));
	objectData->secondMoment.x = tempX;
	objectData->secondMoment.y = tempY;
	objectData->secondMoment.z = tempZ;
}

// Calculate the relative positions of each PointMass data point for an object
void PhysicsEngine::CalcPointMassRelativePositions(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &objectData)
{
	for (int i = 0; i < pointMassVect.size(); i++)
	{
		glm::vec3 actualPos = pointMassVect[i].actualPosition;
		pointMassVect[i].relativePosition = actualPos - objectData->combinedCG;
	}
}

glm::vec3 PhysicsEngine::Normalize(glm::vec3 vec)
{
	btScalar length = sqrt((vec.x*vec.x) + (vec.y*vec.y) + (vec.z * vec.z));

	if (length == 0)
		return glm::vec3(0);
	else
		return glm::vec3(vec.x / length, vec.y / length, vec.z / length);
}

btScalar PhysicsEngine::DotProduct(glm::vec3 one, glm::vec3 two)
{
	return one.x*two.x + one.y*two.y + one.z *two.z;
}

glm::vec3 PhysicsEngine::CrossProduct(glm::vec3 first, glm::vec3 second)
{
	return glm::vec3(first.y*second.z - first.z*second.y, first.z*second.x - first.x*second.z, first.x*second.y - first.y*second.x);
}

void PhysicsEngine::UpdateBodyPos(int statePos)
{
	//m_prevState[statePos * NUMSTATE + 1] = m_currState[statePos * NUMSTATE + 1];
	m_currState[statePos * NUMSTATE + 1] += m_currState[statePos * NUMSTATE];
	//m_derivState[statePos * NUMSTATE + 1] = m_currState[statePos * NUMSTATE + 1] + m_currState[statePos * NUMSTATE];
	//m_currState[statePos * NUMSTATE + 1]
	//m_currState[statePos * NUMSTATE + 1] += glm::vec3(1, 2, 3);
}

void PhysicsEngine::ExplicitEuler()
{
	int size = m_currState.size();

	for (int i = 0; i < size; i++)
	{
		m_prevState[i] = m_currState[i];
		m_currState[i] += delta_t * m_derivState[i];
	}
}