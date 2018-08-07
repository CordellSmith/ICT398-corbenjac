#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "IGameAsset.h"

	/*
	* @class GameAssetFactory
	* @brief Responsible for creating the assets used within the game world.
	*
	* This class is used to create any asset within the game control engine that may be
	* used in the game world. This includes objects (static and dynamic), terrain and NPCs etc.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	* @version 02
	* @date 31/05/2018
	*/
class GameAssetFactory
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	GameAssetFactory() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~GameAssetFactory() { }

		/**
		* @brief Creates and returns an asset according to type
		*
		* Using the type parameter, checks to see what type of asset to return. The file path
		* parameter is used to locate the file to load.
		* 
		* @param ASS_TYPE type
		* @param std::string filePath
		* @return IGameAsset*
		*/
	IGameAsset* CreateAsset(ASS_TYPE type, std::string assetName);

		/**
		* @brief Adds an asset to map
		*
		* Adds the pointer to a game asset that is parsed in as a parameter to the map data structure
		* that holds all the loaded assets.
		*
		* @param IGameAsset* assetToAdd
		* @return const void
		*/
	const void AddAsset(IGameAsset* assetToAdd);

		/**
		* @brief Gets the asset map
		*
		* Returns the asset map by reference.
		*
		* @return const std::multimap<std::string, IGameAsset*>&
		*/
	const std::multimap<std::string, IGameAsset*>& GetAssets() { return m_assets; }

protected:
	/// Data structure to hold assets
	std::multimap<std::string, IGameAsset*> m_assets;
};