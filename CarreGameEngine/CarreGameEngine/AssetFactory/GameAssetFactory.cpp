#include "GameAssetFactory.h"
#include "Object.h"
#include "NPC.h"

IGameAsset* GameAssetFactory::CreateAsset(ASS_TYPE type, std::string assetName)
{
	switch (type)
	{
	case ASS_OBJECT:
		return new Object(assetName);
		break;
	case ASS_NPC:
		return new NPC(assetName);
		break;
	default:
		return nullptr;
		break;
	}
}

const void GameAssetFactory::AddAsset(IGameAsset* assetToAdd)
{
	std::pair<std::string, IGameAsset*> tempAsset = std::pair<std::string, IGameAsset*>(assetToAdd->GetAssetName(), assetToAdd);
	m_assets.insert(tempAsset);
}

