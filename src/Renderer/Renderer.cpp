#include "Renderer.h"
#include "Framebuffer.h"
#include "Scene/Scene.h"
#include "Scene/Component/Light/DirectionalLight.h"

#include <glad/glad.h>

Ref<Renderer> Renderer::s_Instance{};
std::mutex Renderer::s_Mutex;

Renderer::Renderer()
{
	m_Gamma = 2.2f;
	m_Exposure = 1.0f;
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

void Renderer::InitializeMainSceneFramebuffer()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_RGBA32F;
	textureConfig.Format = GL_RGBA;
	textureConfig.MinFilter = GL_LINEAR;
	textureConfig.MagFilter = GL_LINEAR;

	FramebufferRenderbufferConfig renderbufferConfig;
	renderbufferConfig.InternalFormat = GL_DEPTH_STENCIL;

	FramebufferConfig config;
	config.Width = 1920;
	config.Height = 1080;
	config.Textures.push_back(textureConfig);
	config.Renderbuffers.push_back(renderbufferConfig);

	m_MainSceneFramebuffer = Framebuffer::Create(config);
}

void Renderer::InitializePostProcessingFramebuffer()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_RGBA32F;
	textureConfig.Format = GL_RGBA;
	textureConfig.MinFilter = GL_LINEAR;
	textureConfig.MagFilter = GL_LINEAR;

	FramebufferRenderbufferConfig renderbufferConfig;
	renderbufferConfig.InternalFormat = GL_DEPTH_STENCIL;

	FramebufferConfig config;
	config.Width = 1920;
	config.Height = 1080;
	config.Textures.push_back(textureConfig);
	config.Renderbuffers.push_back(renderbufferConfig);

	m_PostProcessingFramebuffer = Framebuffer::Create(config);
}

void Renderer::InitializeShadowMapFramebuffer()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Attachment = GL_DEPTH_ATTACHMENT;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_DEPTH_COMPONENT;
	textureConfig.Format = GL_DEPTH_COMPONENT;
	textureConfig.MinFilter = GL_NEAREST;
	textureConfig.MagFilter = GL_NEAREST;
	textureConfig.WrapS = GL_REPEAT;
	textureConfig.WrapT = GL_REPEAT;
	textureConfig.Type = GL_FLOAT;
	textureConfig.Border = true;

	FramebufferConfig config;
	config.Width = 1024;
	config.Height = 1024;
	config.Textures.push_back(textureConfig);

	m_ShadowMapFramebuffer = Framebuffer::Create(config);
}

void Renderer::InitializePostProcessing()
{
	float viewportVertices[] =
	{
		-1.0f,  1.0f,    0.0f, 1.0f,
		-1.0f, -1.0f,    0.0f, 0.0f,
		 1.0f, -1.0f,    1.0f, 0.0f,

		-1.0f,  1.0f,    0.0f, 1.0f,
		 1.0f, -1.0f,    1.0f, 0.0f,
		 1.0f,  1.0f,    1.0f, 1.0f,
	};

	glGenVertexArrays(1, &m_PostProcessingVAO);
	glBindVertexArray(m_PostProcessingVAO);

	glGenBuffers(1, &m_PostProcessingVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_PostProcessingVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(viewportVertices), viewportVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::RenderScene(Ref<Scene> scene, ViewMode viewMode)
{
	glm::vec3 directionalLightDirection;
	for (auto c : scene->GetComponents<DirectionalLight>())
	{
		auto dirLight = Cast<DirectionalLight>(c);
		directionalLightDirection = dirLight->GetDirection();
	}

	directionalLightDirection *= -10.0f;
	directionalLightDirection = glm::clamp(directionalLightDirection, glm::vec3(0.0001f), glm::vec3(10000.0f));

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	glm::mat4 lightView = glm::lookAt(directionalLightDirection,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpace = lightProjection * lightView;
	scene->m_LightSpace = lightSpace;

	m_ShadowMapFramebuffer->Bind();

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	scene->Render(ViewMode::SceneDepth);
	glCullFace(GL_BACK);

	m_ShadowMapFramebuffer->Unbind();


	m_MainSceneFramebuffer->Bind();

	glClearColor(scene->GetBackgroundColor()->x, scene->GetBackgroundColor()->y, scene->GetBackgroundColor()->z, scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Render(ViewMode::Lit);

	m_MainSceneFramebuffer->Unbind();
}

void Renderer::AddPostProcessingEffects()
{
	m_PostProcessingFramebuffer->Bind();

	auto viewportShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Viewport");
	viewportShader->Use();
	viewportShader->SetInt("u_Screen", 0);
	viewportShader->SetFloat("u_Gamma", m_Gamma);
	viewportShader->SetFloat("u_Exposure", m_Exposure);

	glBindVertexArray(m_PostProcessingVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_MainSceneFramebuffer->GetColorAttachment());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	m_PostProcessingFramebuffer->Unbind();
}

void Renderer::RenderQuad()
{
	float vertices[] =
	{
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,

		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
