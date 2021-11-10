#pragma once

enum class TextureRange
{
	LDR, HDR
};

class Texture
{
public:
	uint32_t m_ID;
	std::string m_Path;
	std::string m_Type;
	
	Texture(std::string path, std::string type, TextureRange range);

	static Ref<Texture> Create(std::string path, std::string type, TextureRange range = TextureRange::LDR);

	void Load(std::string path);
	void LoadHDR(std::string path);

	void Bind(uint32_t index);
	void Unbind();
};