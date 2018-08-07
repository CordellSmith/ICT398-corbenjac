#include "NPC.h"

NPC::NPC(std::string filePath) : m_filePath(filePath)
{
	m_assetType = ASS_NPC;
	LoadFromFilePath(filePath);
}

void NPC::LoadFromFilePath(std::string filePath)
{
	//m_model->LoadModel(filePath);
}

const void NPC::Destroy()
{
	// Needs to be implemented
	return void();
}

const std::string& NPC::GetFilePath() const
{
	return m_filePath;
}

void NPC::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
