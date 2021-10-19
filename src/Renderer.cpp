#include "Renderer.h"
#include "Framebuffer.h"
#include "Scene.h"

#include <glad/glad.h>

Ref<Renderer> Renderer::s_Instance{};
std::mutex Renderer::s_Mutex;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

Ref<Renderer> Renderer::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<Renderer>();

	return s_Instance;
}

void Renderer::CreateMainSceneFramebuffer()
{
	m_MainSceneFramebuffer = CreateRef<Framebuffer>(1920, 1080);
}

void Renderer::RenderMainScene(Ref<Scene> scene)
{
	m_MainSceneFramebuffer->Bind();

	glClearColor(scene->GetBackgroundColor()->x, scene->GetBackgroundColor()->y, scene->GetBackgroundColor()->z, scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Draw();

	m_MainSceneFramebuffer->Unbind();
}
