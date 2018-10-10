#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

#include "Camera.h"
#include "TimeManager.h"
#include "..\Physics\PhysicsEngine.h"
#include "..\Texture\TextureManager.h"
#include "..\Scripting\ScriptManager.h"
#include "..\Common\Structs.h"
#include "..\AssetFactory\Model.h"
#include "..\AssetFactory\IGameAsset.h"
#include "..\AssetFactory\Bruteforce.h"
#include "..\AssetFactory\Player.h"
#include "..\Renderer\OpenGl.h"
#include "..\Renderer\Shader.h"
#include "..\AI\ComputerAI.h"
#include "glut.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

	/*
	* @class GameWorld
	* @brief The instance of our gameworld
	*
	* Contains all the necessary objects, terrains, characters and models that populate the
	* world that will be the view to the user.
	*
	* @note Currently not entirely implemented.
	*
	* @version 01
	* @date 29/03/2018
	*
	* @version 02
	* @date 31/05/2018
	*/
class GameWorld
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	GameWorld() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~GameWorld() { Destroy(); }

		/**
		* @brief Initialises the game world
		*
		* Sets up all the assets of the game world.
		*
		* @return void
		*/
	void Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets);

		/**
		* @brief Updates the game world
		*
		* Recieves any changes that have been made due to input, game logic or
		* physics and updates the current game instance.
		*
		* @return void
		*/
	void Update();

		/**
		* @brief Destroys the game world
		*
		* Anything that was created within this current game world context is
		* destroyed or deleted to free up memory.
		*
		* @return void
		*/
	void Destroy();

		/**
		* @brief Gets the camera object
		*
		* Returns a pointer to the camera object to access the view and projection matrices.
		*
		* @return Camera*
		*/
	Camera* GetCamera() { return m_camera; }

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera* camera) { m_camera = camera; }

		/**
		* @brief Sets the physics world properties
		*
		* Passes the initialized physics engine and populated collision bodies to the game world
		* object to be used.
		*
		* @param PhysicsEngine* physicsEngine
		* @param std::vector<btVector3> collisionBodies
		* @return void
		*/
	void SetPhysicsWorld(PhysicsEngine* physicsEngine, std::vector<CollisionBody*>& collisionBodies);

		/**
		* @brief Updates all physics
		*
		* This function updates all physics bodies, simulating them each step
		*
		* @return null
		*/
	void UpdatePhysics();

		/**
		* @brief Sets the game assets
		*
		* Sets the member data structure containing all the game assets to the map parsed in as a
		* parameter.
		*
		* @param std::multimap<std::string, IGameAsset*> gameAssets
		* @return void
		*/
	void SetGameAssets(std::multimap<std::string, IGameAsset*> gameAssets) { m_gameAssets = gameAssets; }

		/**
		* @brief Sets the terrains
		*
		* Sets the member data structure containing all the terrains to the vector data structure parsed in 
		* as a parameter.
		*
		* @param std::vector<Bruteforce*> terrains
		* @return void
		*/
	void SetTerrains(std::vector<Bruteforce*> terrains) { m_terrains = terrains; }

		/**
		* @brief Sets the AI
		*
		* Sets the member data structure containing all the AI to the vector data structure parsed in
		* as a parameter.
		*
		* @param std::vector<ComputerAI*> allAI
		* @return void
		*/
	void SetAI(std::vector<ComputerAI*> allAI) { m_allAI = allAI; }

	std::vector<CollisionBody*>& GetCollisionBodies() { return *m_collisionBodies; }

protected:
	/// Shader sources
	ShaderSource m_assimpShaderSource, m_shaderSource1, m_shaderSource2, m_testShaderSource;

	/// Camera object
	Camera* m_camera;

	/// Physics world
	PhysicsEngine* m_physicsWorld;

	/// Vector of all collision objects (static and dynamic)
	std::vector<CollisionBody*>* m_collisionBodies;

	std::multimap<std::string, IGameAsset*> m_gameAssets;

	std::vector<Bruteforce*> m_terrains;

	std::vector<ComputerAI*> m_allAI;

	Player* m_player;
	float s = 0.01;

	OpenGl m_glRenderer;
};