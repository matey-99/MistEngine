#include "StaticMeshComponent.h"

#include <glad/glad.h>

#include "Material/MaterialManager.h"

StaticMeshComponent::StaticMeshComponent() : StaticMeshComponent("../../res/models/defaults/default_cube.obj")
{
}

StaticMeshComponent::StaticMeshComponent(std::string path) : m_Path(path)
{
	LoadMesh(path);

	std::string defaultMaterialPath = "../../res/materials/Default.mat";
	for (Mesh mesh : m_Meshes)
	{
		m_MaterialsPaths.push_back(defaultMaterialPath);
		m_Materials.push_back(MaterialManager::GetInstance()->LoadMaterial(defaultMaterialPath));
	}
}

StaticMeshComponent::StaticMeshComponent(std::string path, std::vector<std::string> materialsPaths) : m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialManager::GetInstance()->LoadMaterial(path));

}

void StaticMeshComponent::Draw()
{
	if (!m_MultipleMaterials && m_Materials.at(0))
	{
		m_Materials.at(0)->Use();

		for (auto mesh : m_Meshes)
		{
			mesh.Draw();
		}

		return;
	}

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
			m_Materials.at(i)->Use();

		m_Meshes.at(i).Draw();
	}
}

void StaticMeshComponent::Begin()
{

}

void StaticMeshComponent::Update()
{

}

uint32_t StaticMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh.vertices.size();
	}

	return vertices;
}

void StaticMeshComponent::LoadMesh(std::string path)
{
	m_Meshes.clear();
	m_LoadedTextures.clear();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Loading model failed: " << importer.GetErrorString() << std::endl;
		return;
	}
	m_Path = path;
	m_Directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	//std::string defaultMaterialPath = "../../res/materials/Default.mat";
	//for (Mesh mesh : m_Meshes)
	//{
	//	m_MaterialsPaths.push_back(defaultMaterialPath);
	//	m_Materials.push_back(MaterialManager::GetInstance()->LoadMaterial(defaultMaterialPath));
	//}
}

void StaticMeshComponent::LoadMaterial(std::string path)
{
	m_MaterialsPaths.push_back(path);
	m_Materials.push_back(MaterialManager::GetInstance()->LoadMaterial(path));
}

void StaticMeshComponent::ChangeMesh(std::string path)
{
	m_Materials.clear();
	m_MaterialsPaths.clear();

	LoadMesh(path);
}

void StaticMeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = MaterialManager::GetInstance()->LoadMaterial(path);
}

void StaticMeshComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh StaticMeshComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
	//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//}

	return Mesh(vertices, indices);
}

//std::vector<Texture> StaticMeshComponent::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//	std::vector<Texture> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		bool skip = false;
//		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
//		{
//			if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
//			{
//				textures.push_back(m_LoadedTextures[j]);
//				skip = true;
//				break;
//			}
//		}
//
//		if (!skip)
//		{
//			Texture texture;
//			std::string path = m_Directory + '/' + str.C_Str();
//			texture.id = TextureFromFile(str.C_Str(), m_Directory);
//			texture.type = typeName;
//			texture.path = str.C_Str();
//
//			textures.push_back(texture);
//			m_LoadedTextures.push_back(texture);
//		}
//	}
//
//	return textures;
//}
//
//unsigned int StaticMeshComponent::TextureFromFile(const char* path, const std::string& directory)
//{
//	std::string filename = std::string(path);
//	filename = directory + '/' + filename;
//
//	unsigned int textureId;
//	glGenTextures(1, &textureId);
//
//	stbi_set_flip_vertically_on_load(true);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		glBindTexture(GL_TEXTURE_2D, textureId);
//
//		if (nrComponents == 1)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
//		else if (nrComponents == 3)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		else if (nrComponents == 4)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Failed to load texture from: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureId;
//}