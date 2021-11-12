#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/Component.h"

class StaticMeshComponent : public Component
{
private:
	std::string m_Path;
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	bool m_MultipleMaterials;

public:
	StaticMeshComponent();
	StaticMeshComponent(std::string path);
	StaticMeshComponent(std::string path, std::vector<std::string> materialsPath);
	void Draw();

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	virtual void Begin() override;
	virtual void Update() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }

private:
	
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	//unsigned int TextureFromFile(const char* path, const std::string& directory);
};