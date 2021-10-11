#pragma once

#include "Serializer.h"
#include "Material.h"

class MaterialSerializer : public Serializer<Material>
{
public:
	virtual void Serialize(Ref<Material> scene) override;
	virtual Ref<Material> Deserialize(std::string path) override;
};