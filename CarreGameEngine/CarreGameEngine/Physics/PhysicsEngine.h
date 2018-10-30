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
*
* @date 26/10/2018
* @version 2.4	Swapped bullet physics for my coded physics, but it isn't working. Program compiles, but objects don't collide.
*
* @date 27/08/2018
* @version 2.5	Still no collision. Slowly working through it. Collisions are being detected, and object positions are being updated, but no collisions are being resolved.
*
* @date 28/10/2018
* @version 2.6	Going to try simplify my code to just do linear collision resolutions. Adding rotations in at this point isn't looking possible, and still trying to successfully implement
*				the working physics code I had on another project. Got linear physics partially implemented, but with so many collisions, the game is freezing. Need to lower the number
*				of objects being loaded to continue testing.
*
* @date 29/10/2018
* @version 2.7	Simplified code back to what it was when I first got it working. Linear collisions are working for simple shapes, but giving odd results for mesh shapes. Going to stick with
*				this and merge into master.
*
* @date 30/10/2018
* @version 3.0	Final version for submission.
*/

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

// Includes
#include <vector>
#include <cmath>
#include "btBulletDynamicsCommon.h"
#include "..\Common\Vertex3.h"
#include "..\Common\MyMath.h"
#include "..\AssetFactory\Model.h"
#include "..\..\Dependencies\GLM\include\GLM\vec3.hpp"
#include "..\AI\Affordance\Affordance.h"
#include "DebugDraw.h"

/**
* @brief Struct of point mass data
*
* Point mass data that is used for calculations of objects innertia and rotational physic
*/
struct PointMass
{
	float mass = 0;					/**< Mass of each point mass */
	glm::vec3 actualPosition;		/**< Actual position in relation to body */
	glm::vec3 relativePosition;		/**< Relative position in relation to center of gravity */
};

/**
* @brief Struct of object type physics data
*
* Data for each different type of object that is loaded in (box, sphere, etc)
*/
struct ObjectTypePhysicsData
{
	std::string objType = "";		/**< Type of object */
	float totalMass = 0;			/**< Total mass of object */
	glm::vec3 combinedCG;			/**< Center of gravity of object */
	glm::vec3 firstMoment;			/**< Used to calculate center of gravity */
	glm::vec3 secondMoment;			/**< Inertia */
};

/**
* @brief Struct of rigid body data
*
* Struct containing all rigid body data
*/
struct ObjectRigidBodyData
{
	std::string objType = "";		/**< type of object */
	glm::vec3 currLinearVel;		/**< Current linear velocity */
	glm::vec3 prevLinearVel;		/**< Previous linear velocity */
	glm::vec3 currAngularVel;		/**< Current angular velocity */
	glm::vec3 prevAngularVel;		/**< Previous angular velocity */
	glm::vec3 currForce;			/**< Current force */
	glm::vec3 prevForce;			/**< Previous force */
	glm::vec3 currPos;				/**< Current position */
	glm::vec3 prevPos;				/**< Previous position */
	Quaternion currRot;				/**< Current rotation */
	Quaternion prevRot;				/**< Previous rotation */
	glm::vec3 torque;				/**< Torque of object */
	glm::vec3 angle;				/**< Angle of object */
	glm::vec3 angularAccel;			/**< Angular acceleration */
	glm::vec3 angularMomentum;		/**< Angular momentum */
};

/**
* @brief Struct of collision body data
*
* Struct containing all collision body data
*/
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
	std::string m_name;				/**< Given name of collision body */
	std::string m_modelName;		/**< Model name of collision body */
	glm::vec3 m_position;			/**< Position of collision body */
	glm::vec3 m_rotation;			/**< Rotation of collision body */
	Affordance* m_affordance;		/**< Affordance of collision body */
	ComputerAI* m_AI;				/**< AI of collision body */
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
			* Used to create a static rigid body
			*
			* @param pos - Position to create body
			* @param dimensions - Dimensions of the bounding shape to be created
			* @param colBody - Collision body to create a user pointer on for returning of data
			* @param objType - Type of object being created
			*
			* @return void
			*/
		void CreateStaticRigidBody(glm::vec3 &pos, glm::vec3& dimensions, CollisionBody* colBody, std::string objType);

			/**
			* @brief Creates dynamic rigid body
			*
			* Used to create a dynamic rigid body
			*
			* @param pos - Position to create dynamic body
			* @param dimensions - Dimensions of the bounding shape to be created
			* @param colBody - Collision body to create a user pointer on for returning of data
			* @param objType - Type of object being created
			*
			* @return void
			*/
		void CreateDynamicRigidBody(glm::vec3 &pos, glm::vec3& dimensions, CollisionBody* colBody, std::string objType);
			
			/**
			* @brief Creates a player controlled object
			*
			* Used to create a rigid body that is player controlled
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
			* @param collisionBodies - Vector of all collision bodies to update
			* @param playerObj - Player object to update
			*
			* @return void
			*/
		void Simulate(std::vector<CollisionBody*>& collisionBodies, glm::vec3& playerObj);

			/**
			* @brief Create a sphere object
			*
			* Used to create a sphere shaped dynamic rigid body
			*
			* @param radius - Size of sphere
			* @param startPos - Starting position of object
			* @param colBody - Collision body to create a user pointer on for returning of data
			* @param startVel - Starting velocity
			* @param objType - Type of object being created
			*
			* @return void
			*/
		void AddSphere(float radius, glm::vec3 &startPos, CollisionBody* colBody, glm::vec3& startVel, std::string objType);
			
			/**
			* @brief Create a tringle mesh shape
			*
			* Used to create a dynamic rigid body for complex shapes using a triangle mesh
			*
			* @param modelMesh - Mesh of model
			* @param useQuantizedBvhTree - Use this type of mesh tree or not
			* @param dimensions - Dimensions of object
			* @param objType - Type of object being created
			*
			* @return void
			*/
		void TriangleMeshTest(std::vector<Mesh> &modelMesh, bool useQuantizedBvhTree, glm::vec3& dimensions, std::string objType);

			/**
			* @brief Return collision world
			*
			* Return the physics collision world
			*
			* @return btCollisionWorld* - m_collisionWorld
			*/
		btCollisionWorld* GetDynamicsWorld() const;

			/**
			* @brief Initialises the debug draw
			*
			* Initialize the debug draw function
			*
			* @return void
			*/
		void InitDebugDraw();

			/**
			* @brief Sets up the debug draw 
			*
			* Sets up debug draw lines for rendering
			*
			* @return void
			*/
		void DebugDraw();

			/**
			* @brief Return debug shader
			*
			* Returns the debug shader
			*
			* @return Shader* - m_debugShader
			*/
		Shader* GetDebugShader();

			/**
			* @brief Set camera
			*
			* Sets the camera to the camera in the physics world
			*
			* @param camera - Camera to set
			*
			* @return void
			*/
		void SetCamera(Camera* camera);

			/**
			* @brief Parse model
			*
			* Used to parse a model for debug draw
			*
			* @param model - Model to parse
			*
			* @return void
			*/
		void ParseModel(Model* model);

			/**
			* @brief Initialize all PointMass for an object
			*
			* Initialize all point mass data points within an object to determine the center of gravity and inertia of an object
			*
			* @param pointMass -V ector of all pointmass data for an object
			* @param mass - Mass of object
			* @param size - Size of object (XYZ components)
			*
			* @return void
			*/
		void InitializePointMass(std::vector<PointMass> &pointMassVect, btScalar mass, glm::vec3 size);

			/**
			* @brief Calculate center of gravity
			*
			* Calculate an objects center of gravity using its PointMass data
			*
			* @param pointMass - Vector of all pointmass data for an object
			* @param newObject - Object that calculations are done for
			*
			* @return void
			*/
		void CalcObjectCenterOfGravity(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &newObject);

			/**
			* @brief Calculate second moment of mass (innertia)
			*
			* Calculate the second moment of mass for an object (innertia)
			*
			* @param objectData - Object that calculations are done for
			* @param size - Size of object (XYZ components)
			*
			* @return void
			*/
		void CalcObjectSecondMoment(ObjectTypePhysicsData* &objectData, glm::vec3 size);

			/**
			* @brief Calculate PointMass relative positions
			*
			* Calculate the relative positions (distance from center of gravity) for each PointMass data point in an object
			*
			* @param pointMass - Vector of all pointmass data for an object
			* @param objectData - 		* @param objectData - Object that calculations are done for

			*
			* @return void
			*/
		void CalcPointMassRelativePositions(std::vector<PointMass> &pointMassVect, ObjectTypePhysicsData* &objectData);

			/// Vertex buffer objects
		unsigned int VAO;

			/// Vertex array objects
		unsigned int VBO;

	private:

			/**
			* @brief Normalize a vec3
			*
			* Normalize the values of a glm::vec3
			*
			* @param vec - The vec3 to normalize
			*
			* @return glm::vec3 - normalized
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
			* @return btScalar - Resulting dot product
			*/
		btScalar DotProduct(glm::vec3 one, glm::vec3 two);
		
			/**
			* @brief Cross product of two vec3
			*
			* Calculate the cross product of two glm::vec3
			*
			* @param first - The first vec3
			* @param second - The second vec3
			*
			* @return glm::vec3 - Resulting cross product
			*/
		glm::vec3 CrossProduct(glm::vec3 first, glm::vec3 second);

			/**
			* @brief Perform explicit euler
			*
			* Calculate the next state of all objects
			*
			* @return void
			*/
		void ExplicitEuler();

			/// Vector of the current state of all objects
		std::vector<glm::vec3> m_currState;

			/// Vector of the previous state of all objects
		std::vector<glm::vec3> m_prevState;

			/// Vector of the derived state of all objects (change in time)
		std::vector<glm::vec3> m_derivState;

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

			/// Physics time step
		float delta_t;

			/// Current game time
		float game_time;

			/// Game time at previous frame
		float prev_game_time;

			/// Time since last update
		float physics_lag_time;

	protected:

			/// Mass value of body
		btScalar m_mass;

			/// Holds last known player controlled object location
		glm::vec3 m_playerObject;

			/// Old force applied to player controlled object 
		glm::vec3 m_oldForce;

			/// New force applied to player controlled object
		glm::vec3 m_newForce;

			/// Vector of all debug draw lines
		std::vector<btVector3> m_debugLines;

			/// Shader for debug draw
		Shader* m_debugShader;

			/// Position and rotation matrix for debug draw lines
		glm::mat4 m_modelMatrix;
			
			/// Scale of debug draw lines
		glm::vec3 m_scale;

			/// Camera object
		Camera* m_camera;

			/// Player height controller
		btScalar m_floorHeight = 0.0f;
};

#endif