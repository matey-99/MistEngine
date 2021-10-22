#pragma once

class Texture
{
public:
	uint32_t m_ID;
	std::string m_Path;
	std::string m_Type;
	
	Texture(std::string path, std::string type);

	static Ref<Texture> Create(std::string path, std::string type);

	void Load(std::string path);

	void Bind(uint32_t index);
	void Unbind();
};