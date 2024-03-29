/**
* @class ScriptManager
* @brief Script manager class that handles the loading of all scripts
* @author Jack Matters
*
* @date 14/05/2018
* @version 1.0	Initial start. Created the layout of the class, using a singleton.
*
* @date 14/05/2018
* @version 1.1	Added LoadWindowInitLua and got it all working properly.
*
* @date 17/05/2018
* @version 1.2	Finally had time to work on this again. Made LoadCamInitLua and got it all working.
*				Also started on loading of all textures at the start of program.
*
* @date 17/05/2018
* @version 1.3	Finished loading in of all textures.
*
* @date 21/05/2018
* @version 1.4	Started on loading of all models. Got loading done correctly, just need to store them.
*
* @date 22/05/2018
* @version 1.5	Finished loading of all model data (number of each model, positions, scales).
*
* @date 23/05/2018
* @version 1.6	Started loading of heightmap data. Planned to do this in ModelsInit.lua, but heightmaps
*				have extra information that is needed, so a new function was created.
*
* @date 31/05/2018
* @version 2.0	Final version for submission.
*
* @date 21/10/2018
* @author CSmith
* @version 2.1	Adding function to read in affordance script for lookup table of base affordances.
*/

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

// Includes
#include <string>
#include <map>
#include <iostream>
#include "lua.hpp"
#include <GLM\glm.hpp>
#include "..\Texture\TextureManager.h"
#include "..\Common\Structs.h"
#include "..\AI\Affordance\Affordance.h"

class ScriptManager
{
	public:
		
			/**
			* @brief Singleton instance
			*
			* This function returns a singleton instance of the script manager class so that there is only
			* one instance of a script manager.
			*
			* @return static ScriptManager&
			*/
		static ScriptManager& Instance()
		{
			static ScriptManager instance;

			return instance;
		}

			/**
			* @brief Load all lua scripts
			*
			* Public function that calls a private function for each type of script to load
			*
			* @return void
			*/
		void LoadAllLuaScripts();

			/**
			* @brief Load window initilization
			*
			* Loads all window initialisation variables in
			*
			* @param width - Screen width
			* @param height - Screen height
			* @param name - Screen name
			* @param fullScreen - True if fullscreen, false otherwise
			*
			* @return bool - True if load success, else false
			*/
		bool LoadWindowInitLua(int &width, int &height, std::string &name, bool &fullScreen);

			/**
			* @brief Load cam initilization
			*
			* Loads all camera initialisation variables in
			*
			* @param camPos - Position of camera
			* @param yaw - Camera yaw
			* @param pitch - Camera pitch
			* @param fov - Camera FOV
			* @param near - Projection matrix near plane
			* @param far - Projection matrix far plane
			*
			* @return bool - True if load success, else false
			*/
		bool LoadCamInitLua(glm::vec3 &camPos, float &yaw, float &pitch, float &fov, float &near, float &far);

			/**
			* @brief Load all textures
			*
			* Loads all textures in
			*
			* @return bool - True if load success, else false
			*/
		bool LoadTexturesInitLua();

			/**
			* @brief Load all models
			*
			* Loads all models, their xyz position, and their xyz scale
			*
			* @return bool - True if load success, else false
			*/
		bool LoadModelsInitLua(std::multimap<std::string, ModelsData> &allModelData, ModelsData &modelData);

			/**
			* @brief Load all models
			*
			* Loads all models, their xyz position, and their xyz scale
			*
			* @return bool - True if load success, else false
			*/
		bool LoadHeightmapsInitLua(std::unordered_map<std::string, HeightmapsData> &allHeightmapData, HeightmapsData &heightmapsData);

			/**
			* @brief Load all affordances
			*
			* Loads the base values of each affordance type, currently only 3:
			* sitOn, standOn, canKick of each object type
			*
			* @return bool - True if load success, else false
			*/
		bool LoadAffordanceTable(AffordanceData& m_affordanceTable);

	private:

			/**
			* @brief Default constructor
			*
			* This is the default constructor
			*
			* @return null
			*/
		ScriptManager();

			/**
			* @brief De-constructor
			*
			* This is the de-constructor
			*/
		~ScriptManager();

			/**
			 * @brief String to float converter.
			 *
			 * Does not check whether the string is a number, so ensure that it is convertable.
			 *
			 * @param num A sring representing a number.
			 *
			 * @return A float version of the string input.
			 */
		float toFloat(const std::string& num);

			/**
			 * @brief Space delimited string splitter.
			 *
			 * @param source The space delimited string to split.
			 *
			 * @return A vector of each part of the source string.
			 */
		std::vector<std::string> split(std::string& source);
};

#endif