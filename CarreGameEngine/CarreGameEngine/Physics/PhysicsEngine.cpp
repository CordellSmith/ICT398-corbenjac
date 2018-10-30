/*
* Implementation of PhysicsEngine.h file
* Author - Jack Matters
* Note - Bullet Physics Engine documentation http://bulletphysics.org/wordpress/?page_id=9
*/

//Includes
#include "PhysicsEngine.h"
#include <iostream>

// Number of data types being stored for each object in m_currState
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
	m_epsilon = .85;

	// Initialize time step, and related data
	delta_t = 0.02;
	game_time = 0.0;
	prev_game_time = 0.0;
	physics_lag_time = 0.0;

	// Debug draw shader init
	m_debugShader = new Shader();

	// Camera object for MVP matrix
	m_camera = new Camera();
}

// De-constructor (not implemented)
PhysicsEngine::~PhysicsEngine()
{};

// Create a static rigid body
void PhysicsEngine::CreateStaticRigidBody(glm::vec3 &pos, glm::vec3& dimensions, CollisionBody* colBody, std::string objType)
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
		btScalar(dimensions.x),
		btScalar(dimensions.y),
		btScalar(dimensions.z))
	);

	// Set shape to object
	colObject->setCollisionShape(boxShape);

	// Search objects created
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

		// Mass of object
		m_mass = 50000;

		// Size of object
		glm::vec3 size = { 100, 100, 100 };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);
		objPhysicsData->combinedCG = glm::vec3(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);

		// Calc relative positions of pointmass data
		CalcPointMassRelativePositions(pointMassData, objPhysicsData);

		// Calc second moment of mass (inertia)
		CalcObjectSecondMoment(objPhysicsData, size);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Set the index for the type of rigid body that is being created
	colObject->setUserIndex(BOX);
	colObject->setUserPointer(colBody);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;
	objRigidBodyData->currPos = pos;
	m_objectRigidBodyData.push_back(objRigidBodyData);
}

// Create a bounding box for camera or player controlled object
void PhysicsEngine::CreatePlayerControlledRigidBody(glm::vec3 &playerObj)
{
	// Does object data already exist
	bool objExists = false;

	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

	// Create box shape and add to shape array
	btCollisionShape* camShape = new btCapsuleShape(100, 200);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set origin of body
	btVector3 temp = btVector3(playerObj.x, playerObj.y, playerObj.z);
	startTransform.setOrigin(temp);

	// Set shape to object
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

	// Set contact point info for object
	//colObject->setContactProcessingThreshold(BT_LARGE_FLOAT);
	//colObject->setCcdMotionThreshold(1e-7);
	//colObject->setCcdSweptSphereRadius(15);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = "player";
	objRigidBodyData->currPos = playerObj;
	m_objectRigidBodyData.push_back(objRigidBodyData);

	// Set new player object coordinates
	m_playerObject = playerObj;
}

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(glm::vec3 &pos, glm::vec3& dimensions, CollisionBody* colBody, std::string objType)
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
		btScalar(dimensions.x),
		btScalar(dimensions.y),
		btScalar(dimensions.z))
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
		m_mass = 50000;

		// Size of object
		glm::vec3 size = { 100, 100, 100 };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);
		objPhysicsData->combinedCG = glm::vec3(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);

		// Calc relative positions of pointmass data
		CalcPointMassRelativePositions(pointMassData, objPhysicsData);

		// Calc second moment of mass (inertia)
		CalcObjectSecondMoment(objPhysicsData, size);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Set the index for the type of rigid body that is being created
	colObject->setUserIndex(BOX);
	colObject->setUserPointer(colBody);
	
	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;
	objRigidBodyData->currPos = pos;

	if (objType == "ball")
		objRigidBodyData->currLinearVel.x = 0.05;

	m_objectRigidBodyData.push_back(objRigidBodyData);
}

// Create a dynamic rigid body
void PhysicsEngine::AddSphere(float radius, glm::vec3 &startPos, CollisionBody* colBody, glm::vec3& startVel, std::string objType)
{
	// Does object data already exist
	bool objExists = false;
	
	// Create box shape and add to shape array
	btCollisionObject* colObject = new btCollisionObject();
	btCollisionShape* sphereShape = new btSphereShape(radius);

	// Set origin of body
	btVector3 temp = btVector3(startPos.x, startPos.y, startPos.z);
	colObject->getWorldTransform().setOrigin(temp);

	// Set shape to object
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

	// Set the index for the type of rigid body that is being created
	colObject->setUserPointer(colBody);

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;
	objRigidBodyData->currPos = startPos;
	objRigidBodyData->currLinearVel = startVel;
	m_objectRigidBodyData.push_back(objRigidBodyData);
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<CollisionBody*>& collisionBodies, glm::vec3& playerObj)
{
	// Time step
	game_time += 1.0;
	physics_lag_time += (game_time - prev_game_time);

	// Ray from player to player look direction
	btVector3 btFrom(playerObj.x, playerObj.y, playerObj.z);
	btVector3 btTo(playerObj.z, -3000.0f, playerObj.z);

	// Get closest result
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	// Get data from closest object
	m_collisionWorld->rayTest(btFrom, btTo, res); 
	
	// Number of collisions detected
	int numManifolds = 0;

	// Collision objects that are colliding
	btCollisionObject* objA;
	btCollisionObject* objB;

	// Data of colliding rigid bodies
	// rb - rigidBody, pd - physicsData
	ObjectRigidBodyData* rbA = 0;
	ObjectRigidBodyData* rbB = 0; 
	ObjectTypePhysicsData* pdA = 0;
	ObjectTypePhysicsData* pdB = 0;

	// Collision point data
	btVector3 ptA = { 0, 0, 0 };
	btVector3 ptB = { 0, 0, 0 };

	// Simple step simulation
	while (physics_lag_time > delta_t)
	{
		// Reset data for next calculations
		ptA = { 0, 0, 0 };
		ptB = { 0, 0, 0 };
		rbA = 0;
		rbB = 0;
		pdA = 0;
		pdB = 0;

		// Counters
		int i,j,k;

		// Check all objects for collision detection, and get number of collisions
		m_collisionWorld->performDiscreteCollisionDetection();
		numManifolds = m_collisionWorld->getDispatcher()->getNumManifolds();

		// Get info for each colliding pair
		for (i = 0; i < numManifolds; i++)
		{
			// Get the two colliding objects
			btPersistentManifold* contactManifold = m_collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
			objA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			objB = const_cast<btCollisionObject*>(contactManifold->getBody1());
			contactManifold->refreshContactPoints(objA->getWorldTransform(), objB->getWorldTransform());

			// Get number of contact points on colliding bodies
			int numContacts = contactManifold->getNumContacts();

			// If no contact points found, continue simulation
			if (numContacts < 1)
				continue;

			// Get data for colliding objects
			for (j = 0; j < m_collisionWorld->getNumCollisionObjects(); j++)
			{
				// Current object
				btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];

				// If current object equals colliding object A, set rbA and pdA for object
				if (obj == objA)
				{
					rbA = m_objectRigidBodyData[j];
					for (k = 0; k < m_ObjectTypePhysicsData.size(); k++)
					{
						if (m_ObjectTypePhysicsData[k]->objType == m_objectRigidBodyData[j]->objType)
							pdA = m_ObjectTypePhysicsData[k];
					}
				}

				// If current object equals colliding object B, set rbB and pdB for object
				if (obj == objB)
				{
					rbB = m_objectRigidBodyData[j];
					for (k = 0; k < m_ObjectTypePhysicsData.size(); k++)
					{
						if (m_ObjectTypePhysicsData[k]->objType == m_objectRigidBodyData[j]->objType)
							pdB = m_ObjectTypePhysicsData[k];
					}
				}
			}

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

<<<<<<< HEAD
			//if (pdA->objType == "lecTheatre" || pdB->objType == "lecTheatre")
			//{
			//	if (pdA->objType == "lecTheatre")
			//	{
			//		glm::vec3 centerBToCol = rbB->currPos - glm::vec3(ptB.getX(), ptB.getY(), ptB.getZ());

			//		m_normal = centerBToCol;
			//		m_normal = Normalize(m_normal);

			//		// Calculate linear impulse
			//		btScalar tempImpulse;

			//		tempImpulse = DotProduct(rbA->currLinearVel - rbB->currLinearVel, m_normal);
			//		tempImpulse *= -(1 + m_epsilon) * pdA->totalMass * pdB->totalMass;
			//		tempImpulse /= (pdA->totalMass + pdB->totalMass);
			//		m_impulse = tempImpulse * m_normal;

			//		rbB->currLinearVel = rbB->currLinearVel - (m_impulse / pdB->totalMass);
			//		//rbB->currLinearVel *= 2;
			//		rbB->currPos -= m_impulse / pdA->totalMass;
			//	}
			//	else
			//	{
			//		glm::vec3 centerAToCol = glm::vec3(rbA->currPos.x, rbA->currPos.y, rbA->currPos.z) - glm::vec3(ptA.getX(), ptA.getY(), ptA.getZ());
			//		//glm::vec3 centerBToCol = rbB->currPos - glm::vec3(ptB.getX(), ptB.getY(), ptB.getZ());

			//		m_normal = centerAToCol;
			//		m_normal = Normalize(m_normal);

			//		// Calculate linear impulse
			//		btScalar tempImpulse;

			//		tempImpulse = DotProduct(rbA->currLinearVel - rbB->currLinearVel, m_normal);
			//		tempImpulse *= -(1 + m_epsilon) * pdA->totalMass * pdB->totalMass;
			//		tempImpulse /= (pdA->totalMass + pdB->totalMass);
			//		m_impulse = tempImpulse * m_normal;

			//		rbA->currLinearVel = rbA->currLinearVel + (m_impulse / pdA->totalMass);
			//		//rbA->currLinearVel *= 2;
			//		//rbA->currPos += rbA->currLinearVel;
			//		//rbA->currPos += m_impulse / pdA->totalMass;
			//	}
			//}
			//else 
			
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
				rbA->currPos += m_impulse / pdA->totalMass;
				rbB->currPos -= m_impulse / pdA->totalMass;
			
=======
			// Get vector for distance from object center to the average collision point
			glm::vec3 centerAToCol = glm::vec3(rbA->currPos.x, rbA->currPos.y, rbA->currPos.z) - glm::vec3(ptA.getX(), ptA.getY(), ptA.getZ());
			glm::vec3 centerBToCol = rbB->currPos - glm::vec3(ptB.getX(), ptB.getY(), ptB.getZ());

			// Calculate normal of colliding objects
			m_normal = centerAToCol - centerBToCol;
			m_normal = Normalize(m_normal);

			// Linear physics calculations below
			// Calculate linear impulse
			btScalar tempImpulse = DotProduct(rbA->currLinearVel - rbB->currLinearVel, m_normal);
			tempImpulse *= -(1 + m_epsilon) * pdA->totalMass * pdB->totalMass;
			tempImpulse /= (pdA->totalMass + pdB->totalMass);
			m_impulse = tempImpulse * m_normal;

			// Calculate object velocities after collision (eq 4.3.9 and eq 4.3.10)
			rbA->currLinearVel = rbA->currLinearVel + (m_impulse / pdA->totalMass);
			rbB->currLinearVel = rbB->currLinearVel - (m_impulse / pdB->totalMass);

			// Set new linear velocities to corresponding current values
			rbA->currPos += m_impulse / pdA->totalMass;
			rbB->currPos -= m_impulse / pdA->totalMass;

			// Angular physics calculations below
			// Calculate torque
			rbA->torque = CrossProduct(centerAToCol, pdA->totalMass * rbA->currLinearVel);
			rbB->torque = CrossProduct(centerBToCol, pdB->totalMass * rbB->currLinearVel);

			// Calculate angular acceleration
			rbA->angularAccel = rbA->torque / pdA->secondMoment;
			rbB->angularAccel = rbB->torque / pdB->secondMoment;

			// Calc new angular velocity
			rbA->prevAngularVel += rbA->currAngularVel;
			rbB->prevAngularVel += rbB->currAngularVel;
			rbA->currAngularVel += rbA->angularAccel * delta_t;
			rbB->currAngularVel += rbB->angularAccel * delta_t;

			// Calculate new angle
			rbA->angle -= rbA->currAngularVel * delta_t;
			rbB->angle += rbB->currAngularVel * delta_t;

			// Set new angular momentum to corresponding value
			//m_currState[posA * NUMSTATE + 3] = pdA->secondMoment * rbA->currAngularVel;
			//m_currState[posB * NUMSTATE + 3] = pdB->secondMoment * rbB->currAngularVel;

			// Calculate angular impulse (eq 4.3.38)
			/*btScalar numerator = DotProduct(m_normal, rbA->currLinearVel - rbB->currLinearVel) + (DotProduct(rbA->currAngularVel, CrossProduct(centerAToCol, m_normal))) + (DotProduct(rbB->currAngularVel, CrossProduct(centerBToCol, m_normal)));
			numerator *= -(1.0f + m_epsilon);
			glm::vec3 denominatorA; // = (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			glm::vec3 transA = CrossProduct(centerAToCol, m_normal);
			glm::vec3 transB = CrossProduct(centerBToCol, m_normal);
			glm::vec3 negInert = (1.0f / pdA->secondMoment);
			denominatorA = CrossProduct(centerAToCol, m_normal) * (1.0f / pdA->secondMoment) * CrossProduct(centerAToCol, m_normal) + CrossProduct(centerBToCol, m_normal) * (1.0f / pdB->secondMoment) * CrossProduct(centerBToCol, m_normal);
			denominatorA += (1.0 / pdA->totalMass) + (1.0 / pdB->totalMass);
			glm::vec3 angImpulse = numerator / denominatorA;
			angImpulse *= m_normal;*/

			// Calculate new angular impulse momentum (eq 4.3.37)
			/*glm::vec3 tempU = CrossProduct(centerAToCol, m_normal);
			glm::vec3 tempV = CrossProduct(centerAToCol, m_normal);
			rbA->angularMomentum += angImpulse * CrossProduct(centerAToCol, m_normal);
			rbB->angularMomentum -= angImpulse * CrossProduct(centerBToCol, m_normal);*/

			// Update angular momentum
			//m_currState[posA * NUMSTATE + 3] += angImpulse * CrossProduct(centerAToCol, m_normal);
			//m_currState[posB * NUMSTATE + 3] += angImpulse * CrossProduct(centerBToCol, m_normal);
		}
			
		// Calculate new deriv state for each object after collisions resolved
		/*for (i = 0; i < m_objectRigidBodyData.size(); i++)
		{
			m_derivState[4 * i] = CalcForceOnObject(i);
			m_derivState[4 * i + 1] = UpdateBodyPos(i);
			m_derivState[4 * i + 2] = calc
			m_derivState[4 * i + 3] = calcTorque();
>>>>>>> origin/temp-master
		}

		// Perform explicit euler
		//ExplicitEuler();*/

			// Update positions of all dynamic objects
		for (j = 0; j < m_collisionWorld->getNumCollisionObjects(); j++)
		{
			// Get the next object
			btCollisionObject* obj = m_collisionWorld->getCollisionObjectArray()[j];
			btTransform trans = obj->getWorldTransform();
			btVector3 tempPos = trans.getOrigin();

			// Reset forces on player object prior to next step simulation
			if (obj->getUserIndex() == CAMERA)
			{

			}

			// Check to see if player object
			if (obj->getUserIndex() == CAMERA)
			{

			}
			else
			{
				// Collision body has AI
				if (collisionBodies[j]->m_AI != NULL)
				{
					// Update AI state
					collisionBodies[j]->m_AI->Update();

					// Update the physics collision object position
					trans.getOrigin().setX(collisionBodies[j]->m_AI->GetPosition().x);
					trans.getOrigin().setY(collisionBodies[j]->m_AI->GetPosition().y);
					trans.getOrigin().setZ(collisionBodies[j]->m_AI->GetPosition().z);

					// Update the object positions for drawing
					collisionBodies[j]->m_position = collisionBodies[j]->m_AI->GetPosition();

					// Update the object rotations for drawing
					collisionBodies[j]->m_rotation = collisionBodies[j]->m_AI->GetRotation();

					// Set new position
					obj->setWorldTransform(trans);
				}
				else
				{
					// Set new position
					tempPos.setValue(
						tempPos.getX() + m_objectRigidBodyData[j]->currLinearVel.x,
						tempPos.getY() + m_objectRigidBodyData[j]->currLinearVel.y,
						tempPos.getZ() + m_objectRigidBodyData[j]->currLinearVel.z);

					// Set new position
					trans.setOrigin(tempPos);
					obj->setWorldTransform(trans);

					// Set new position
					collisionBodies[j]->m_position = glm::vec3(
						trans.getOrigin().getX(),
						trans.getOrigin().getY(),
						trans.getOrigin().getZ());
				}
			}
		}
		// Decrease time step
		physics_lag_time -= delta_t;
	}
	// Set new previous time for next time step
	prev_game_time = game_time;
}

// Create a triangle mesh shape collision object
void PhysicsEngine::TriangleMeshTest(std::vector<Mesh> &modelMesh, bool useQuantizedBvhTree, glm::vec3& dimensions, std::string objType)
{
	// Does object data already exist
	bool objExists = false;

	// Create a new collision object
	btCollisionObject* colObject = new btCollisionObject();

	// Create a triangle mesh shape
	btTriangleMesh* trimesh = new btTriangleMesh();
	for (int j = 0; j < modelMesh.size(); j++)
	{
		Mesh tempMesh = modelMesh[j];
		std::vector<Vertex3> tempMeshVertex = tempMesh.GetVertices();

		for (int i = 0; i < tempMeshVertex.size(); i+=3)
		{
			// Position for points of a triangle
			glm::vec3 p1 = tempMeshVertex[i].m_position;
			glm::vec3 p2 = tempMeshVertex[i+1].m_position;
			glm::vec3 p3 = tempMeshVertex[i+2].m_position;

			// Position for points of a triangle
			btVector3 A = btVector3(p1.x, p1.y, p1.z);
			btVector3 B = btVector3(p2.x, p2.y, p2.z);
			btVector3 C = btVector3(p3.x, p3.y, p3.z);

			// Add triangle to mesh
			trimesh->addTriangle(A, B, C);

			// Add points to debug draw array of btVector3s
			m_debugLines.push_back(A);
			m_debugLines.push_back(B);
			m_debugLines.push_back(C);
		}
	}

	// Set origin
	btTransform	trans;
	trans.setIdentity();
	btVector3 temp = btVector3(modelMesh[0].GetPosition().x, modelMesh[0].GetPosition().y, modelMesh[0].GetPosition().z);
	colObject->getWorldTransform().setOrigin(temp);

	// Set trimesh scaling
	trimesh->setScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));

	// Create mesh shape from triangle meshes
	btCollisionShape* trimeshShape = new btBvhTriangleMeshShape(trimesh, useQuantizedBvhTree);

	// Set collision shape of collision object
	colObject->setCollisionShape(trimeshShape);

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
		m_mass = 500000000;

		// Size of object
		glm::vec3 size = { 100, 100, 100 };

		// Make new PointMass data and initialize it
		std::vector<PointMass> pointMassData;
		InitializePointMass(pointMassData, m_mass, size);

		// Calc center of gravity
		CalcObjectCenterOfGravity(pointMassData, objPhysicsData);
		objPhysicsData->combinedCG = glm::vec3(dimensions.x/2.0f, dimensions.y/2.0f, dimensions.z/2.0f);

		// Add object data to vector
		m_ObjectTypePhysicsData.push_back(objPhysicsData);
	}

	// Set index of object
	colObject->setUserIndex(MESH);

	// Add object to collision world
	m_collisionWorld->addCollisionObject(colObject);

	// Add object rigid body data to vector
	ObjectRigidBodyData* objRigidBodyData = new ObjectRigidBodyData();
	objRigidBodyData->objType = objType;
<<<<<<< HEAD

	objRigidBodyData->currPos = modelMesh[0].GetPosition();
=======
	objRigidBodyData->currPos = glm::vec3(modelMesh[0].GetPosition().x, modelMesh[0].GetPosition().y, modelMesh[0].GetPosition().z);
>>>>>>> origin/temp-master
	m_objectRigidBodyData.push_back(objRigidBodyData);
}

// Return the physics world
btCollisionWorld* PhysicsEngine::GetDynamicsWorld() const
{
	return m_collisionWorld;
}

// Initialize debug draw
void PhysicsEngine::InitDebugDraw()
{
	// Create a debug shader source (vertext and fragment shader)
	ShaderSource debugShaderSource = ParseShaders("Resources/shaders/DebugDraw.shader");

	// Initialise the shader program for the physics engine
	GetDebugShader()->Initialize(debugShaderSource.VertexSource, debugShaderSource.FragmentSource);

	glGenVertexArrays(1, &VAO);
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

// Prepare debug draw for rendering
void PhysicsEngine::DebugDraw()
{
	// Enable shader
	m_debugShader->TurnOn();

	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix();
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

// Return the debug shader
Shader* PhysicsEngine::GetDebugShader() 
{ 
	return m_debugShader; 
};

// Set the camera
void PhysicsEngine::SetCamera(Camera* camera) 
{ 
	m_camera = camera; 
}

// Parse model for debug draw
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
	// Data
	glm::vec3 firstMoment;
	int i;

	// Get total mass for object
	for (i = 0; i < pointMassVect.size(); i++)
		newObject->totalMass += pointMassVect[i].mass;

	// Calculate first moment of mass
	for (i = 0; i < pointMassVect.size(); i++)
		newObject->firstMoment = newObject->firstMoment + pointMassVect[i].actualPosition * pointMassVect[i].mass;

	// Calculate center of gravity for object
	newObject->combinedCG = newObject->firstMoment / newObject->totalMass;
}

// Calculate an objects second moment of mass (innertia)
void PhysicsEngine::CalcObjectSecondMoment(ObjectTypePhysicsData* &objectData, glm::vec3 size)
{
	// Innertia for a box shape
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
	// Calculate relative distance for each point mass (distance from pointmass to center of gravity)
	for (int i = 0; i < pointMassVect.size(); i++)
		pointMassVect[i].relativePosition = pointMassVect[i].actualPosition; - objectData->combinedCG;
}

// Normalize a vec3
glm::vec3 PhysicsEngine::Normalize(glm::vec3 vec)
{
	// Length of the vec3
	btScalar length = sqrt((vec.x*vec.x) + (vec.y*vec.y) + (vec.z * vec.z));

	// Normalize the vec3
	if (length == 0)
		return glm::vec3(0);
	else
		return glm::vec3(vec.x / length, vec.y / length, vec.z / length);
}

// Calculate dot product of two vec3
btScalar PhysicsEngine::DotProduct(glm::vec3 one, glm::vec3 two)
{
	return one.x*two.x + one.y*two.y + one.z *two.z;
}

// Calculate cross product of two vec3
glm::vec3 PhysicsEngine::CrossProduct(glm::vec3 first, glm::vec3 second)
{
	return glm::vec3(first.y*second.z - first.z*second.y, first.z*second.x - first.x*second.z, first.x*second.y - first.y*second.x);
}

// Explicit Euler function - Calculate change over time for each object state
void PhysicsEngine::ExplicitEuler()
{
	// Number of states to update
	int size = m_currState.size();

	// Update each state
	for (int i = 0; i < size; i++)
	{
		m_prevState[i] = m_currState[i];
		m_currState[i] += delta_t * m_derivState[i];
	}
}