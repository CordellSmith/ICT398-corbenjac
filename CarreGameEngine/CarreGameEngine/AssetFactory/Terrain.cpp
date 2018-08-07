#include "Terrain.h"

Terrain::Terrain()
	: m_scaleX(1.0), m_scaleY(1.0), m_scaleZ(1.0), m_heightfieldSize(0)
{
	m_terrainModel = new Model();
}

Terrain::Terrain(float scaleX, float scaleY, float scaleZ) 
	: m_scaleX(scaleX), m_scaleY(scaleY), m_scaleZ(scaleZ), m_heightfieldSize(0)
{
	m_terrainModel = new Model();
}