#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/Component.h"

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
	Model();
	Model(std::string path);
	Model(std::string path, std::string materialPath);
	void Draw();

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);

	virtual void Begin() override;
	virtual void Update() override;

	inline std::string GetPath() const { return m_Path; }
	inline Ref<Material> GetMaterial() const { return m_Material; }
	inline std::string GetMaterialPath() const { return m_MaterialPath; }
	uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(Ref<Material> material) { m_Material = material; }

private:
	
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	//unsigned int TextureFromFile(const char* path, const std::string& directory);
};