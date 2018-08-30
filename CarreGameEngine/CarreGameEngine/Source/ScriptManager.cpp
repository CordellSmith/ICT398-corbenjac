/*
* Implementation of TextureManager.h file
* Author - Jack Matters
*/

// Includes
#include "../Scripting/ScriptManager.h"
#include <iostream>

// Default constructor
ScriptManager::ScriptManager(){}

// De-constructor
ScriptManager::~ScriptManager(){}

// Load all scripts
void ScriptManager::LoadAllLuaScripts(){}

// Load all camera and window init variables
bool ScriptManager::LoadWindowInitLua(int &width, int &height, std::string &name, bool &fullScreen)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "Resources/scripts/WindowInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "screenWidth");
	lua_getglobal(Environment, "screenHeight");
	lua_getglobal(Environment, "screenTitle");
	lua_getglobal(Environment, "fullScreen");

	// Set values
	width = (int)lua_tonumber(Environment, 1);
	height = (int)lua_tonumber(Environment, 2);
	name = lua_tostring(Environment, 3);
	fullScreen = lua_toboolean(Environment, 4);

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}

// Load all camera and viewing matrix variables
bool ScriptManager::LoadCamInitLua(glm::vec3 &camPos, float &yaw, float &pitch, float &fov, float &near, float &far)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "Resources/scripts/CameraInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "camPosX");
	lua_getglobal(Environment, "camPosY");
	lua_getglobal(Environment, "camPosZ");
	lua_getglobal(Environment, "yaw");
	lua_getglobal(Environment, "pitch");
	lua_getglobal(Environment, "camFOV");
	lua_getglobal(Environment, "camNearPlane");
	lua_getglobal(Environment, "camFarPlane");

	// Set values
	camPos.x = (int)lua_tonumber(Environment, 1);
	camPos.y = (float)lua_tonumber(Environment, 2);
	camPos.z = (float)lua_tonumber(Environment, 3);
	yaw = (float)lua_tonumber(Environment, 4);
	pitch = (float)lua_tonumber(Environment, 5);
	fov = (float)lua_tonumber(Environment, 6);
	near = (float)lua_tonumber(Environment, 7);
	far = (float)lua_tonumber(Environment, 8);

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}

// Load all textures
bool ScriptManager::LoadTexturesInitLua()
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "Resources/scripts/TexturesInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "AllTextures");

	// File path of texture to load
	std::string filePath;

	// Push to first table
	lua_pushnil(Environment);

	// Keep reading while there is data in table
	while (lua_next(Environment, -2) != 0)
	{
		// Push to next table
		lua_pushnil(Environment);
		while (lua_next(Environment, -2) != 0)
		{
			// Push to next table
			lua_pushnil(Environment);
			while (lua_next(Environment, -2) != 0)
			{
				// Get file path and load it
				filePath = lua_tostring(Environment, -1);
				TextureManager::Instance().LoadTexture(filePath);

				// Pop out of current table
				lua_pop(Environment, 1);
			}
			// Pop out of current table
			lua_pop(Environment, 1);
		}
		// Pop out of current table
		lua_pop(Environment, 1);
	}

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}

// Load all model data
bool ScriptManager::LoadModelsInitLua(std::unordered_map<std::string, ModelsData> &allModelData, ModelsData &modelData)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "Resources/scripts/ModelsInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "AllModels");

	// File path of model to load
	std::string filePath;

	// Name of current model being read in
	std::string modelName;
	std::string texFilePath;

	// Different types of data being read in
	std::string values[9];
	values[0] = "filePath";
	values[1] = "texFilePath";
	values[2] = "scaleX";
	values[3] = "scaleY";
	values[4] = "scaleZ";
	values[5] = "posX";
	values[6] = "posY";
	values[7] = "posZ";
	values[8] = "AI";

	//temp values
	std::string temp;
	std::vector<float> tempData;
	glm::vec3 tempPos;
	glm::vec3 tempScale;
	bool isAI = false;

	// Push to first table
	lua_pushnil(Environment);

	// Keep reading while there is data in table
	while (lua_next(Environment, -2) != 0)
	{
		// Get current model name being read in
		modelName = lua_tostring(Environment, -2);
		
		// Push to next table
		lua_pushnil(Environment);
		while (lua_next(Environment, -2) != 0)
		{
			// Push to next table
			lua_pushnil(Environment);
			while (lua_next(Environment, -2) != 0)
			{
				// Load data into correct variable
				temp = lua_tostring(Environment, -2);
				if (temp.compare(values[0]) == 0)
					filePath = lua_tostring(Environment, -1);
				if (temp.compare(values[1]) == 0)
					texFilePath = lua_tostring(Environment, -1);
				if (temp.compare(values[2]) == 0)
					tempScale.x = lua_tonumber(Environment, -1);
				if (temp.compare(values[3]) == 0)
					tempScale.y = lua_tonumber(Environment, -1);
				if (temp.compare(values[4]) == 0)
					tempScale.z = lua_tonumber(Environment, -1);
				if (temp.compare(values[5]) == 0)
					tempPos.x = lua_tonumber(Environment, -1);
				if (temp.compare(values[6]) == 0)
					tempPos.y = lua_tonumber(Environment, -1);
				if (temp.compare(values[7]) == 0)
					tempPos.z = lua_tonumber(Environment, -1);
				if (temp.compare(values[8]) == 0)
					isAI = lua_toboolean(Environment, -1);

				// Pop out of current table
				lua_pop(Environment, 1);
			}	
			// Pass in filePath to modelData
			modelData.filePath = filePath;
			modelData.texFilePath = texFilePath;

			// Pass in model scales and push to modelData
			tempData.push_back(tempScale.x);
			tempData.push_back(tempScale.y);
			tempData.push_back(tempScale.z);
			modelData.modelScales.push_back(tempData);

			// Clear for next batch of data
			tempData.clear();

			// Pass in positions and push to modelData
			tempData.push_back(tempPos.x);
			tempData.push_back(tempPos.y);
			tempData.push_back(tempPos.z);
			modelData.modelPositions.push_back(tempData);

			// Clear for next batch of data
			tempData.clear();

			// Pass in if AI or not
			modelData.isAI.push_back(isAI);

			// Pop out of current table
			lua_pop(Environment, 1);
		}
		// Add to map
		allModelData[modelName] = modelData;

		// Clear vectors of any data before adding more, and reset string
		modelData.modelPositions.clear();
		modelData.modelScales.clear();
		modelData.filePath = "";
		modelData.isAI.clear();

		// Pop out of current table
		lua_pop(Environment, 1);
	}

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}

// Load all heightmap data
bool ScriptManager::LoadHeightmapsInitLua(std::unordered_map<std::string, HeightmapsData> &allHeightmapData, HeightmapsData &heightmapsData)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "Resources/scripts/TerrainsInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "AllHeightmaps");

	// File path of heightmap and texture applied
	std::string filePath;
	std::string texFilePath;
	std::string heightfieldType;

	// Different types of data being read in
	std::string values[9];
	values[0] = "filePath";
	values[1] = "texFilePath";
	values[2] = "fileSize";
	values[3] = "scaleX";
	values[4] = "scaleY";
	values[5] = "scaleZ";
	values[6] = "posX";
	values[7] = "posY";
	values[8] = "posZ";

	//temp values
	std::string temp;
	glm::vec3 tempPos;
	glm::vec3 tempScale;
	int size;

	// Push to first table
	lua_pushnil(Environment);

	// Keep reading while there is data in table
	while (lua_next(Environment, -2) != 0)
	{
		// Get type of heightfield
		heightfieldType = lua_tostring(Environment, -2);

		// Push to next table
		lua_pushnil(Environment);
		while (lua_next(Environment, -2) != 0)
		{
			// Push to next table
			lua_pushnil(Environment);
			while (lua_next(Environment, -2) != 0)
			{
				// Load data into correct variable
				temp = lua_tostring(Environment, -2);
				if (temp.compare(values[0]) == 0)
					filePath = lua_tostring(Environment, -1);
				if (temp.compare(values[1]) == 0)
					texFilePath = lua_tostring(Environment, -1);
				if (temp.compare(values[2]) == 0)
					size = lua_tonumber(Environment, -1);
				if (temp.compare(values[3]) == 0)
					tempScale.x = lua_tonumber(Environment, -1);
				if (temp.compare(values[4]) == 0)
					tempScale.y = lua_tonumber(Environment, -1);
				if (temp.compare(values[5]) == 0)
					tempScale.z = lua_tonumber(Environment, -1);
				if (temp.compare(values[6]) == 0)
					tempPos.x = lua_tonumber(Environment, -1);
				if (temp.compare(values[7]) == 0)
					tempPos.y = lua_tonumber(Environment, -1);
				if (temp.compare(values[8]) == 0)
					tempPos.z = lua_tonumber(Environment, -1);

				// Pop out of current table
				lua_pop(Environment, 1);
			}
			// Pass in filePath and texFilePath and size to heightmapsData
			heightmapsData.filePath = filePath;
			heightmapsData.texFilePath = texFilePath;
			heightmapsData.fileSize = size;

			// Pass in model scales and push to modelData
			heightmapsData.modelScales.push_back(tempScale.x);
			heightmapsData.modelScales.push_back(tempScale.y);
			heightmapsData.modelScales.push_back(tempScale.z);

			// Pass in positions and push to modelData
			heightmapsData.modelPositions.push_back(tempPos.x);
			heightmapsData.modelPositions.push_back(tempPos.y);
			heightmapsData.modelPositions.push_back(tempPos.z);

			// Add to map (should only be ONE terrain heightfield, and this is used to set cam height later)
			if(heightfieldType == "terrain")
				allHeightmapData[heightfieldType] = heightmapsData;
			else
				allHeightmapData[filePath] = heightmapsData;

			// Clear vectors of any data before adding more, and reset strings
			heightmapsData.modelPositions.clear();
			heightmapsData.modelScales.clear();
			heightmapsData.fileSize = 0;
			heightmapsData.filePath = "";
			heightmapsData.texFilePath = "";

			// Pop out of current table
			lua_pop(Environment, 1);
		}
		// Pop out of current table
		lua_pop(Environment, 1);
	}

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}