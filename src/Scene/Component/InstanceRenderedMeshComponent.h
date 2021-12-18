#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class InstanceRenderedMeshComponent : public RenderComponent
{
private:
	std::string m_Path;
	std::vector<Mesh> m_Meshes;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	bool m_MultipleMaterials;

	uint32_t m_Amount = 10;
	float m_Radius = 150.0f;
	float m_Offset = 25.0f;
	std::vector<glm::mat4> m_ModelMatrices;

	uint32_t m_ModelMatricesBuffer;

public:
	InstanceRenderedMeshComponent(Entity* owner);
	InstanceRenderedMeshComponent(Entity* owner, std::string path);
	InstanceRenderedMeshComponent(Entity* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<Mesh> GetMeshes() const { return m_Meshes; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }
};