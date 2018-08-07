#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
}