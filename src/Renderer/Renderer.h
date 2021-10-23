#pragma once

#include <mutex>

class Framebuffer;
class Scene;

enum class FramebufferType
{
	MAIN_SCENE, POST_PROCESSING
};

class Renderer
{
private:
	static Ref<Renderer> s_Instance;
	static std::mutex s_Mutex;

	Ref<Framebuffer> m_MainSceneFramebuffer;
	Ref<Framebuffer> m_PostProcessingFramebuffer;

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

	void CreateFramebuffer(FramebufferType type);

	void InitializePostProcessing();

	void RenderMainScene(Ref<Scene> scene);
	void AddPostProcessingEffects();

	inline Ref<Framebuffer> GetMainSceneFramebuffer() const { return m_MainSceneFramebuffer; }
	inline Ref<Framebuffer> GetPostProcessingFramebuffer() const { return m_PostProcessingFramebuffer; }
	inline bool IsPostProcessing() const { return m_PostProcessing; }

	friend class RendererSettingsPanel;
};