#pragma once

#include <mutex>

class Framebuffer;
class Scene;

class Renderer
{
private:
	static Ref<Renderer> s_Instance;
	static std::mutex s_Mutex;

	Ref<Framebuffer> m_MainSceneFramebuffer;

public:
	Renderer();
	~Renderer();

	Renderer(Renderer& other) = delete;
	void operator=(const Renderer&) = delete;

	static Ref<Renderer> GetInstance();

	void CreateMainSceneFramebuffer();
	void RenderMainScene(Ref<Scene> scene);

	inline Ref<Framebuffer> GetMainSceneFramebuffer() const { return m_MainSceneFramebuffer; }
};