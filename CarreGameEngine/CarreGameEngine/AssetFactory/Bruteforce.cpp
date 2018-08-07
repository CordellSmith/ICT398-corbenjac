#include "Bruteforce.h"

Bruteforce::Bruteforce(float scaleX, float scaleY, float scaleZ)
{
	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_scaleZ = scaleZ;
}

void Bruteforce::GenerateTerrain(GLuint textureId, std::string textureFilePath)
{
	Mesh tempMesh;
	float colour;
	float texLeft, texBottom, texTop;

	for (int z = 0; z < m_heightfieldSize - 1; z++)
	{
		for (int x = 0; x < m_heightfieldSize - 1; x++)
		{
			Vertex3 vertex;

			float tex00 = (float)x / (m_heightfieldSize); // U
			float tex01 = (float)z / (m_heightfieldSize); // V
			float tex10 = (float)(x + 1) / (m_heightfieldSize);
			float tex11 = (float)(z + 1) / (m_heightfieldSize);

			// Colour of triangle
			colour = (float)GetHeightColour(x, z) / 255;
			vertex.m_colour = glm::vec4(colour, colour, colour, 1.0);

			// Vertices
			vertex.m_position.x = (float)x * m_scaleX;
			vertex.m_position.y = GetHeight(x, z);
			vertex.m_position.z = (float)z * m_scaleZ;

			vertex.m_texCoords.x = tex00;
			vertex.m_texCoords.y = tex11;
			
			tempMesh.GetVertices().push_back(vertex); // 0.0, 1.0, 0.0

			vertex.m_position.x = (float)(x + 1) * m_scaleX;
			vertex.m_position.y = GetHeight(x + 1, z);
			vertex.m_position.z = (float)z * m_scaleZ;

			vertex.m_texCoords.x = tex10;
			vertex.m_texCoords.y = tex01;

			tempMesh.GetVertices().push_back(vertex); // 1.0, 1.0, 0.0

			vertex.m_position.x = (float)(x + 1) * m_scaleX;
			vertex.m_position.y = GetHeight(x + 1, z + 1);
			vertex.m_position.z = (float)(z + 1) * m_scaleZ;

			vertex.m_texCoords.x = tex10;
			vertex.m_texCoords.y = tex11;

			tempMesh.GetVertices().push_back(vertex); // 1.0, 1.0, 1.0
			
			vertex.m_position.x = (float)x * m_scaleX;
			vertex.m_position.y = GetHeight(x, z);
			vertex.m_position.z = (float)z * m_scaleZ;

			vertex.m_texCoords.x = tex00;
			vertex.m_texCoords.y = tex01;

			tempMesh.GetVertices().push_back(vertex); // 0.0, 1.0, 0.0

			vertex.m_position.x = (float)(x + 1) * m_scaleX;
			vertex.m_position.y = GetHeight(x + 1, z + 1);
			vertex.m_position.z = (float)(z + 1) * m_scaleZ;

			vertex.m_texCoords.x = tex10;
			vertex.m_texCoords.y = tex11;

			tempMesh.GetVertices().push_back(vertex); // 1.0, 1.0, 1.0

			vertex.m_position.x = (float)x * m_scaleX;
			vertex.m_position.y = GetHeight(x, z + 1);
			vertex.m_position.z = (float)(z + 1) * m_scaleZ;

			vertex.m_texCoords.x = tex00;
			vertex.m_texCoords.y = tex11;
			
			tempMesh.GetVertices().push_back(vertex); // 0.0, 1.0, 1.0
		}
	}
	//tempMesh.SetupMesh();
	tempMesh.GetTextures().push_back(AddTexture(textureId, textureFilePath));
	m_terrainModel->GetTextures().push_back(AddTexture(textureId, textureFilePath));
	m_terrainModel->GetMeshBatch().push_back(tempMesh);
	m_terrainModel->SetScale(glm::vec3(1.0, 1.0, 1.0));
}

Texture Bruteforce::AddTexture(GLuint textureId, std::string textureFilePath)
{
	Texture temp;

	temp.m_id = textureId;
	temp.m_path = textureFilePath;
	temp.m_type = "texture_diffuse";

	return temp;
}

bool Bruteforce::LoadHeightfield(std::string fileName, const int size)
{
	// Open for binary read, print error if error found
	std::ifstream infile(fileName.c_str(), std::ios::binary);
	if (!infile)
	{
		std::cout << "Cannot open file: " << fileName << std::endl;
		return false;
	}

	// Allocate memory, return false if no size = 0
	if (m_terrainData)
		delete[] m_terrainData;
	if (size > 0)
		m_terrainData = new unsigned char[size * size];

	// Read in heightfield and get length of file
	infile.seekg(0, std::ios::end);
	int length = infile.tellg();

	// Read data in as a block, cast to char*, set size, and close file
	infile.seekg(0, std::ios::beg);
	infile.read(reinterpret_cast<char*>(m_terrainData), length);
	infile.close();
	m_heightfieldSize = size;
	
	std::cout << "Heightfield Load Successful!" << std::endl;
	
	return true;
}

bool Bruteforce::Inbounds(int xpos, int zpos)
{
	if ((xpos >= 0 && xpos < m_heightfieldSize * m_scaleX) && (zpos >= 0 && zpos < m_heightfieldSize * m_scaleZ))
		return true;
	else
		return false;
}

float Bruteforce::GetHeight(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
		return ((float)(m_terrainData[(zpos * m_heightfieldSize) + xpos]) * m_scaleY);
	else
		return ((float)(m_terrainData[(zpos - 1 * m_heightfieldSize) + xpos]) * m_scaleY);
}

unsigned char Bruteforce::GetHeightColour(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return m_terrainData[zpos * m_heightfieldSize + xpos];
	}
	return 1;
}

float Bruteforce::GetAverageHeight(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return (float)(m_terrainData[((int)(zpos / m_scaleZ) * m_heightfieldSize + (int)(xpos / m_scaleX))]) * m_scaleY;
	}
	return 1;
}



