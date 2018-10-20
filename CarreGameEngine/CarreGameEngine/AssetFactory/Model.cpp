#include "Model.h"

//#include "..\ImageDB\stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

Model::Model()
{
	SetScale(glm::vec3(1.0, 1.0, 1.0));
	m_shader = new Shader();
	m_compAI = NULL;
}

void Model::LoadModel(std::string filePath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_FlipUVs 
		| aiProcess_CalcTangentSpace
		| aiProcess_Triangulate 
		| aiProcess_JoinIdenticalVertices 
		| aiProcess_SortByPType);

	m_directory = filePath.substr(0, filePath.find_last_of('/'));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	else
		ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshBatch.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex3> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// process face data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		Vertex3 vertex;
		glm::vec3 vertexPos;
		glm::vec4 colour;
		glm::vec3 normalCoord;
		glm::vec2 texCoord;
		glm::vec3 tangent;
		glm::vec3 biTangent;

		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < 3; j++)
		{
			// vertex positions
			auto const &v = mesh->mVertices[face.mIndices[j]];
			vertexPos.x = v.x; 
			vertexPos.y = v.y; 
			vertexPos.z = v.z;
			vertex.m_position = vertexPos;

			// colours (randomised)
			colour = glm::vec4(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), 1.0f);
			vertex.m_colour = colour;

			// normals
			if (mesh->HasNormals())
			{
				auto const &n = mesh->mNormals[face.mIndices[j]];
				normalCoord.x = n.x;
				normalCoord.y = n.y;
				normalCoord.z = n.z;
				vertex.m_normal = normalCoord;
			}
			else
				vertex.m_normal = glm::vec3(0.0f, 0.0f, 0.0f);

			// texture coordinates
			if (mesh->mTextureCoords[0])
			{
				auto const &uv = mesh->mTextureCoords[0][face.mIndices[j]];
				texCoord.x = uv.x;
				texCoord.y = uv.y;
				vertex.m_texCoords = texCoord;
			}
			else
				vertex.m_texCoords = glm::vec2(0.0f, 0.0f);

			//// tangents and bitangents
			//if (mesh->HasTangentsAndBitangents())
			//{
			//	auto const &t = mesh->mTangents[face.mIndices[j]];
			//	tangent.x = t.x;
			//	tangent.y = t.y;
			//	tangent.z = t.z;
			//	vertex.m_tangent = tangent;

			//	auto const &bt = mesh->mBitangents[face.mIndices[j]];
			//	biTangent.x = bt.x;
			//	biTangent.y = bt.y;
			//	biTangent.z = bt.z;
			//	vertex.m_biTangent = biTangent;
			//}

			ReadDimensions(vertexPos);

			vertices.push_back(vertex);
			indices.push_back(face.mIndices[j]);
		}
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;

		if (std::strcmp(str.C_Str(), "$texture_dummy.bmp") == 0)
			skip = true;

		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (std::strcmp(m_texturesLoaded[j].m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.m_id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.m_type = typeName;
			texture.m_path = str.C_Str();
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

const void Model::CalculateDimensions()
{
	ScaleDimensions();

	// The dimension is calculated as the max - min of each dimensions (width, height, depth)
	m_dimensions.x = fabs(m_Xdim.y - m_Xdim.x);
	m_dimensions.y = fabs(m_Ydim.y - m_Ydim.x);
	m_dimensions.z = fabs(m_Zdim.y - m_Zdim.x);
}

const void Model::ScaleDimensions()
{
	m_Xdim *= m_scale.x;
	m_Ydim *= m_scale.y;
	m_Zdim *= m_scale.z;
}

const void Model::ReadDimensions(glm::vec3 vertexPos)
{
	// First vertex gets made the initial values of dimensions to be compared to
	if (m_firstVertex)
	{
		m_Xdim = glm::vec2(vertexPos.x, vertexPos.x);
		m_Ydim = glm::vec2(vertexPos.y, vertexPos.y);
		m_Zdim = glm::vec2(vertexPos.z, vertexPos.z);
		// After initial values are set, the rest of the models vertexs can be compared to find min, max
		m_firstVertex = false;
	}
	// Read and compare the vertex
	else
	{
		// Compare for X
		if (vertexPos.x < m_Xdim.x) {
			// x is min
			m_Xdim.x = vertexPos.x;
		}
		if (vertexPos.x > m_Xdim.y) {
			// y is max
			m_Xdim.y = vertexPos.x;
		}

		// Compare for Y
		if (vertexPos.y < m_Ydim.x) {
			m_Ydim.y = vertexPos.y;
		}
		if (vertexPos.x > m_Ydim.y) {
			m_Ydim.y = vertexPos.y;
		}

		// Compare for Z
		if (vertexPos.z < m_Zdim.x) {
			m_Zdim.x = vertexPos.z;
		}
		if (vertexPos.z > m_Zdim.y) {
			m_Zdim.y = vertexPos.z;
		}
	}
}

void Model::Destroy()
{
	// Todo: implement destroy function of all meshes of the model
	return void();
}


void Model::SetPosition(glm::vec3 position)
{
	m_position = position;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetPosition(position);
	}
}

void Model::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetRotation(rotation);
	}
}

void Model::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetScale(scale);
	}
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filePath = std::string(path);
	filePath = directory + '/' + filePath;

	return TextureManager::Instance().GetTextureID(filePath);
}