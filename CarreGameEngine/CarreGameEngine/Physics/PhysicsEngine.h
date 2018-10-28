/**
*@note	Bullet Continuous Collision Detection and Physics Library
*		Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/
*
*		This software is provided 'as-is', without any express or implied warranty.
*		In no event will the authors be held liable for any damages arising from the use of this software.
*		Permission is granted to anyone to use this software for any purpose,
*		including commercial applications, and to alter it and redistribute it freely,
*		subject to the following restrictions:
*
*		1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, 
		   an acknowledgment in the product documentation would be appreciated but is not required.
*		2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
*		3. This notice may not be removed or altered from any source distribution.
*
* @class PhysicsEngine
* @brief Physics engine that handles all object collisions and physics based movement
* @author Jack Matters
*
* @date 29/04/2018
* @version 1.0	Initial start. Took ideas from a previous physics engine I worked on that used ReactPhysics3D. Also used HelloWorld example as a guide.
*				Managed to get 2 spheres (currently shown as floor planes) to collide off each other and a floor.
*
* @date 05/04/2018
* @version 1.1	Altered code to draw a square dynamic rigid body instead of a sphere for testing purposes. Started work on camera collision. 
*
* @date 06/04/2018
* @version 1.2  Tidied up camera (player object) code. Force is now applied in direction camera was moved, calculations are done, then camera is moved to updated
*				position. Still needs some work, but will do for now.
*
* @date 18/04/2018
* @version 1.3	Big gap between versions due to other units requirements. Set up functions for initializing all object rigid bodies. Will need changing.
*
* @date 19/04/2018
* @version 1.4	Added testing for heightfield terrain shape. Got it loading and working, but without a visual terrain it is not possible to see if it is
*				working correctly or not. Have commented it out until I am able to keep working on it. Also added some more doxygen comments.
*
* @date 20/04/2018
* @version 1.5	Did some research and found out that objects become inactive after a set time of inactivity. Created new function to ensure all objects are always active.
*				This fixed the camera getting stuck, as well as other objects getting stuck in the air.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*
*
* @date 15/10/2018
* @author Cordell Smith
* @version 2.1 Adding debug draw functionality for a mesh
*
* @date 16/10/2018
* @author Jack Matters
* @version 2.2	Adding all my current code for self coded physics. Currently have collision detection working, physics involved in collision partially working.
*				Commented everything out as not fully ready to implement yet.
*
* @date 24/10/2018
* @version 2.3	Swapping out bullet physics for the physics I coded. Spent many hours working on rotations to not much success. Going to do this instead so we at least have something.
*/

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

// Includes
#include <vector>
#include <fstream>	// Used for testing of heightfield terrain shape (will be removed later)
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include "..\Common\Vertex3.h"
#include "..\Common\MyMath.h"
#include "..\AssetFactory\Model.h"
#include "DebugDraw.h"
#include <cmath>
#include "../../Dependencies/GLM/include/GLM/vec3.hpp"
#include "../AI/Affordance/Affordance.h"


/*************************************NEW**************************************/
///  Struct of point mass data for an object (for determining cente of gravity and other info)
struct PointMass
{
	float mass = 0;
	glm::vec3 actualPosition;
	glm::vec3 relativePosition;
};

/// Struct containing useful data for physics sim for an object type
struct ObjectTypePhysicsData
{
	std::string objType = "";
	float totalMass = 0;
	glm::vec3 combinedCG;
	glm::vec3 firstMoment;
	glm::vec3 secondMoment; // Inertia
};

/// Struct containing useful data for physics sim for an object type
struct ObjectRigidBodyData
{
	std::string objType = "";
	glm::vec3 currLinearVel;
	glm::vec3 prevLinearVel;
	glm::vec3 currAngularVel;
	glm::vec3 prevAngularVel;
	glm::vec3 currForce;
	glm::vec3 prevForce;
	glm::vec3 currPos;
	glm::vec3 prevPos;
	Quaternion currRot;
	Quaternion prevRot;
	glm::vec3 currDerivs;
	glm::vec3 prevDerivs;
	glm::vec3 torque;
	glm::vec3 angle;
	glm::vec3 angularAccel;
	glm::vec3 angularMomentum;
};
/*************************************NEW**************************************/

struct CollisionBody {
	CollisionBody(std::string name, std::string modelName, const glm::vec3& position, const glm::vec3& rotation, Affordance* affordanceData, ComputerAI* AI = NULL)
	{ 
		m_name = name;
		m_modelName = modelName;
		m_position = position;
		m_rotation = rotation;
		m_affordance = affordanceData;
		m_AI = AI;
	};
	std::string m_name;
	std::string m_modelName;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	Affordance* m_affordance;
	ComputerAI* m_AI;
};

class PhysicsEngine
{
	public:
			/**
			* @brief Enum for the different types of rigid bodies created.
			*
			* Enum used to differentiate between the different types of rigid bodies created
			*/
		typedef enum
		{
			CAMERA = 1,			/**< Camera object rigid body */
			BOX = 2,			/**< Box shape rigid body */
			SPHERE = 3,			/**< Sphere shape rigid body */
			HEIGHTFIELD = 4,	/**< Heightfield terrain shape rigid body */
			PLANE = 5,			/**< Plane shape rigid body */
			MESH = 6			/**< Mesh collider */
		}RIGID_BODY_TYPE;

			/**
			* @brief Default constructor
			* 
			* This is the default constructor
			*
			* @return null
			*/
		PhysicsEngine();

			/**
			* @brief De-constructor
			*
			* This is the de-constructor
			*
			* @note Not implemented yet
			*
			* @return null
			*/
		~PhysicsEngine();

			/**
			* @brief Creates static rigid body
			*
			* This is a test function that is used to create a static body (as of now, it creates a floor)
			*
			* @param pos - Position to create body
			*
			* @return void
			*/
		//void CreateStaticRigidBody(btVector3 &pos);
		void CreateStaticRigidBody(glm::vec3 &pos);

			/**
			* @brief Creates dynamic rigid body
			*
			* This is a test function that creates dynamic rigid bodies for testing purposes
			*
			* @param pos - Position to create dynamic body
			* @param dimensions - Dimensions of the bounding shape to be created
			*
			* @return void
			*/
		void CreateDynamicRigidBody(glm::vec3 &pos, glm::vec3& dimensions, CollisionBody* colBody, std::string objType);
		//void CreateDynamicRigidBody(glm::vec3 &pos, std::string objType);
			/**
			* @brief Creates dynamic rigid body for a player controlled object
			*
			* This is a test function that creates a dynamic rigid body for the player controlled object (as of now, that is the camera)
			* 
			* @param playerObj - Object that is player controlled 
			*
			* @return void
			*/
		void CreatePlayerControlledRigidBody(glm::vec3 &playerObj);

			/**
			* @brief Simulate the dynamic world
			*
			* This function simulates the dynamic world by handling all physics calculations each step
			*
			* @param bodyPos - Update all rigid body positions for drawing
			* @param playerObj - Sets new player object position
			*
			* @return void
			*/
		//void Simulate(std::vector<CollisionBody*>& collisionBodies, std::vector<Quaternion> &bodyRot, glm::vec3 &playerObj);
		void Simulate(std::vector<CollisionBody*>& collisionBodies, glm::vec3& playerObj);

			/*
			* @brief Public function that calls different private functions for creation of rigid bodies
			* @param objectData - Data structure containing all game object data
			* @return True if all game object rigid bodies created, false otherwise
			*/
		//bool CreateAllRigidBodies(Data &objectData);

			/*
			* @brief 
			* @param 
			* @return 
			*/
		void AddSphere(float radius, glm::vec3 &startPos, CollisionBody* colBody, glm::vec3& startVel, std::string objType);

			/**
			* @brief Create a heightfield terrain shape
			*
			* This is a test function that creates a heightfield terrain shape
			*
			* @return void
			*/
		void CreateHeightfieldTerrainShape();

			/**
			* @brief Activates all objects
			*
			* Ensures that every rigid body remains active throughout simulation. Otherwise they can become 'stuck'
			*
			* @return void
			*/
		void ActivateAllObjects();

		void TriangleMeshTest(std::vector<Mesh> &modelMesh, bool useQuantizedBvhTree, bool collision, std::string objType);

		btDiscreteDynamicsWorld* GetDynamicsWorld() const { return m_dynamicsWorld; };

		btAlignedObjectArray<btCollisionShape*>& GetCollisionShapes() { return m_collisionShapes; };

			/**
			* @brief Initialises the debug draw
			*
			*
			*
			* @return void
			*/
		void InitDebugDraw();

			/**
			* @brief Sets up the debug draw lines to be rendered
			*
			* 
			*
			* @return void
			*/
		void DebugDraw();

		Shader* GetDebugShader() { return m_debugShader; };

		void SetCamera(Camera* camera) { m_camera = camera; }

		void ParseModel(Model* model);

		unsigned int VAO, VBO;
		
		/*************************************NEW**************************************/
		/**
		* @brief Initialize all PointMass for an object
		*
		* Initialize all point mass data points within an object to determine the Cog and inertia of an object
		*
		* @param pointMass - Pointer to the PointMass data to initialize
		* @param mass - Mass of object
		*
		* @return void
		*/
		void InitializePointMass(std::vector<PointMass> &pointMassVect, btScalar mass, glm::vec3 size);

		/**
		* @brief Calculate center of gravity
		*
		* Calculate an objects center of gravity using its PointMass data
		*
		* @param pointMass - Pointer to the PointMass data of object
		* @param numElements - Number of PointMass data points
		*
		* @return void
		*/
		void CalcObjectCenterOfGravity(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &newObject);

		/**
		* @brief Calculate second moment of mass
		*
		* Calculate the second moment of mass for an object
		*
		* @return void
		*/
		void CalcObjectSecondMoment(ObjectTypePhysicsData* &objectData, glm::vec3 size);

		/**
		* @brief Calculate PointMass relative positions
		*
		* Calculate the relative positions for each PointMass data point in an object
		*
		* @param pointMass - Pointer to the pointMass data of object
		* @param numElements - Number of PointMass data points
		*
		* @return void
		*/
		void CalcPointMassRelativePositions(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &objectData);
		/*************************************NEW**************************************/

	private:
		/*************************************NEW**************************************/
		
		/// Collision world
		btCollisionWorld* m_collisionWorld;
		
		/// Coefficient of restitution (conservation/loss of kinetic energy)
		btScalar m_epsilon;

		/// vector of different object types and physics data associated with them
		std::vector<ObjectTypePhysicsData*> m_ObjectTypePhysicsData;

		/// Vector of each rigid body being simulated
		std::vector<ObjectRigidBodyData*> m_objectRigidBodyData;


			/// Normal vector at collision point
		glm::vec3 m_normal;

			/// Linear impulse of collision
		glm::vec3 m_impulse;

		int count;

		float delta_t; // Physics time step, seconds
		float game_time; // Current game time, seconds
		float prev_game_time; // Game time at previous frame
		float physics_lag_time; // Time since last update

			/**
			* @brief Normalize a vec3
			*
			* Normalize the values of a glm::vec3
			*
			* @param vec - The vec3 to normalize
			*
			* @return glm::vec3
			*/
		glm::vec3 Normalize(glm::vec3 vec);

		/**
		* @brief Dot product of two vec3
		*
		* Calculate the dot product of two glm::vec3
		*
		* @param one - The first vec3
		* @param two - The second vec3
		*
		* @return btScalar
		*/
		btScalar DotProduct(glm::vec3 one, glm::vec3 two);

		glm::vec3 CrossProduct(glm::vec3 first, glm::vec3 second);

		std::vector<glm::vec3> m_currState;
		std::vector<glm::vec3> m_prevState;
		std::vector<glm::vec3> m_derivState;

		// linearVel
		// pos
		// angularVel
		// angularMomentum

		void UpdateBodyPos(int statePos);

		void ExplicitEuler();
		
		/*************************************NEW**************************************/

	protected:

			/// Determines if shape is dynamic or not
		bool m_isDynamic;

			/// Dynamic world
		btDiscreteDynamicsWorld* m_dynamicsWorld;

			/// Array of collision shapes
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

			/// Mass value of body
		btScalar m_mass;

			/// Holds last known player controlled object location
		glm::vec3 m_playerObject;

			/// Old force applied to player controlled object (don't think this is needed)
		//glm::vec3 m_oldForce;

			/// New force applied to player controlled object (don't think this is needed)
		glm::vec3 m_newForce;

			/*
			* @brief Creates a rigid body for the camera
			* @param objectData - Data structure containing all data for rigid body creation
			*/
		//void CreateCameraRigidBody(Data &objectData);

			/*
			* @brief Creates a box shape rigid body
			* @param objectData - Data structure containing all data for rigid body creation
			*/
		//void CreateBoxShapeRigidBody(Data &objectData);

			/*
			* @brief Creates a sphere shape rigid body
			* @param objectData - Data structure containing all data for rigid body creation
			*/
		//void CreateSphereShapeRigidBody(Data &objectData);

			/*
			* @brief Creates a heightfield terrain shape
			* @param objectData - Data structure containing all data for rigid body creation
			*/
		//void CreateHeightFieldTerrainShape(Data &objectData);

			/// Size of heightfield data (used for testing)
		int m_size;

			/// Holds all heightfield data (used for testing)
		unsigned char *m_terrainData;

			/// Debug draw
		std::vector<btVector3> m_debugLines;

		Shader* m_debugShader;

		// Used to alter the scale, position, rotation of the debug draw (lines)
		glm::mat4 m_modelMatrix;
		glm::vec3 m_scale;

		Camera* m_camera;

			/// Player height controller
		btScalar m_floorHeight = 0.0f;

		//DebugDraw d;

		//btIDebugDraw test;
};

#endif