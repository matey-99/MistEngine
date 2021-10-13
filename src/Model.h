#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Material.h"
#include "ShaderLibrary.h"
#include "Component.h"

class Model : public Component
{
private:
	std::string m_Path;
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	Ref<Material> m_Material;
	std::string m_MaterialPath;

public:
	Model(const char* path);
	Model(const char* path, std::string materialPath);
	void Draw(Ref<Shader> shader);

	void LoadModel(std::string path);
	void LoadMaterial(std::string path);

	virtual void Begin() override;
	virtual void Update() override;

	inline std::string GetPath() const { return m_Path; }
	inline Ref<Material> GetMaterial() const { return m_Material; }
	inline std::string GetMaterialPath() const { return m_MaterialPath; }

	inline void SetMaterial(Ref<Material> material) { m_Material = material; }

private:
	
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};