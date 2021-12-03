#pragma once

#include <mutex>
#include <glm/glm.hpp>

class Framebuffer;
class Scene;

enum class ViewMode
{
	Lit, Wireframe, SceneDepth
};

class Renderer
{
public:
	uint32_t m_DepthMap;
private:
	static Ref<Renderer> s_Instance;
	static std::mutex s_Mutex;

	Ref<Framebuffer> m_MainSceneFramebuffer;
	Ref<Framebuffer> m_PostProcessingFramebuffer;
	Ref<Framebuffer> m_ShadowMapFramebuffer;

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
	void InitializeShadowMapFramebuffer();

	void InitializePostProcessing();

	void RenderScene(Ref<Scene> scene, ViewMode viewMode);
	void AddPostProcessingEffects();

	void RenderQuad();

	inline Ref<Framebuffer> GetMainSceneFramebuffer() const { return m_MainSceneFramebuffer; }
	inline Ref<Framebuffer> GetPostProcessingFramebuffer() const { return m_PostProcessingFramebuffer; }
	inline Ref<Framebuffer> GetShadowMapFramebuffer() const { return m_ShadowMapFramebuffer; }
	inline bool IsPostProcessing() const { return m_PostProcessing; }

	friend class RendererSettingsPanel;
};