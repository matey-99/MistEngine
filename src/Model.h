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
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	Ref<Material> m_Material;

public:
	Model(const char* path, Ref<Material> material);
	void Draw(Ref<Shader> shader);

	virtual void Begin() override;
	virtual void Update() override;

	inline Ref<Material> GetMaterial() const { return m_Material; }
	inline void SetMaterial(Ref<Material> material) { m_Material = material; }

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};