#include "ShaderLibrary.h"

Ref<ShaderLibrary> ShaderLibrary::s_Instance{};
std::mutex ShaderLibrary::s_Mutex;

ShaderLibrary::ShaderLibrary()
{
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Standard", CreateRef<Shader>("Standard", "res/shaders/Material/Standard.vert", "res/shaders/Material/Standard.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Shiny", CreateRef<Shader>("Shiny", "res/shaders/Material/Shiny.vert", "res/shaders/Material/Shiny.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Refraction", CreateRef<Shader>("Refraction", "res/shaders/Material/Refraction.vert", "res/shaders/Material/Refraction.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Viewport", CreateRef<Shader>("Viewport", "res/shaders/PostProcessing/Viewport.vert", "res/shaders/PostProcessing/Viewport.frag")));

	m_SkyboxShaders.insert(std::make_pair<std::string, Ref<Shader>>("Skybox", CreateRef<Shader>("Skybox", "res/shaders/Skybox/Skybox.vert", "res/shaders/Skybox/Skybox.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("EquirectangularToCubemap", 
		CreateRef<Shader>("EquirectangularToCubemap", "res/shaders/Calculation/EquirectangularToCubemap.vert", "res/shaders/Calculation/EquirectangularToCubemap.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Irradiance", CreateRef<Shader>("Irradiance", "res/shaders/Calculation/Irradiance.vert", "res/shaders/Calculation/Irradiance.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Prefilter", CreateRef<Shader>("Prefilter", "res/shaders/Calculation/Prefilter.vert", "res/shaders/Calculation/Prefilter.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("BRDF", CreateRef<Shader>("BRDF", "res/shaders/Calculation/BRDF.vert", "res/shaders/Calculation/BRDF.frag")));
}

Ref<ShaderLibrary> ShaderLibrary::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<ShaderLibrary>();

	return s_Instance;
}

Ref<Shader> ShaderLibrary::GetShader(ShaderType type, std::string name)
{
	switch (type)
	{
	case ShaderType::MATERIAL:
		return m_MaterialShaders.find(name)->second;
	case ShaderType::POST_PROCESSING:
		return m_PostProcessingShaders.find(name)->second;
	case ShaderType::SKYBOX:
		return m_SkyboxShaders.find(name)->second;
	case ShaderType::CALCULATION:
		return m_CalculationShaders.find(name)->second;
	}

	return Ref<Shader>();
}

std::vector<Ref<Shader>> ShaderLibrary::GetAllMaterialShaders()
{
	std::vector<Ref<Shader>> result = std::vector<Ref<Shader>>();
	for (auto shader : m_MaterialShaders)
	{
		result.push_back(shader.second);
	}

	return result;
}
