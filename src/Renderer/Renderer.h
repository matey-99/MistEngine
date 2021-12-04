#pragma once

#include <mutex>
#include <glm/glm.hpp>
#include <iostream>
#include <glad/glad.h>

#define CHECK_OPENGL_ERRORS()	while (GLenum error = glGetError()) \
									std::cout << "OpenGL Error: " << error << std::endl;

class Framebuffer;
class Scene;
class DirectionalLight;
class PointLight;
class SpotLight;

enum class ViewMode
{
	Lit, Wireframe, SceneDepth
};

class Renderer
{
public:
	uint32_t m_DepthMap;
	uint32_t m_DepthCubemap;
private:
	static Ref<Renderer> s_Instance;
	static std::mutex s_Mutex;

	Ref<Framebuffer> m_MainSceneFramebuffer;
	Ref<Framebuffer> m_PostProcessingFramebuffer;

	Ref<Framebuffer> m_DirectionalLightShadowMapFramebuffer;
	Ref<Framebuffer> m_PointLightShadowMapFramebuffer;
	Ref<Framebuffer> m_SpotLightShadowMapFramebuffer;

	uint32_t m_DepthMapFBO;

	uint32_t m_PostProcessingVAO;
	uint32_t m_PostProcessingVBO;

	bool m_PostProcessing;

	float m_Gamma;
	float m_Exposure;

public:
	Renderer();
	~Renderer();

	Renderer(Renderer& other) = delete;
	void operator=(const Renderer&) = delete;

	static Ref<Renderer> GetInstance();

	void InitializeMainSceneFramebuffer();
	void InitializePostProcessingFramebuffer();
	void InitializeShadowMapFramebuffers();

	void InitializePostProcessing();

	void RenderScene(Ref<Scene> scene, ViewMode viewMode);
	void AddPostProcessingEffects();

	void RenderShadowMap(Scene* scene, DirectionalLight* source);
	void RenderShadowMap(Scene* scene, PointLight* source);
	void RenderShadowMap(Scene* scene, SpotLight* source);

	void RenderQuad();

	inline Ref<Framebuffer> GetMainSceneFramebuffer() const { return m_MainSceneFramebuffer; }
	inline Ref<Framebuffer> GetPostProcessingFramebuffer() const { return m_PostProcessingFramebuffer; }
	inline Ref<Framebuffer> GetDirectionalLightShadowMapFramebuffer() const { return m_DirectionalLightShadowMapFramebuffer; }
	inline Ref<Framebuffer> GetPointLightShadowMapFramebuffer() const { return m_PointLightShadowMapFramebuffer; }
	inline Ref<Framebuffer> GetSpotLightShadowMapFramebuffer() const { return m_SpotLightShadowMapFramebuffer; }

	inline bool IsPostProcessing() const { return m_PostProcessing; }

	friend class RendererSettingsPanel;
};