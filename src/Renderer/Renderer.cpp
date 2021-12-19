#include "Renderer.h"
#include "Framebuffer.h"
#include "Scene/Scene.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Mesh.h"
#include "Scene/Component/Light/Light.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"

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

	FramebufferTextureConfig thresholdTextureConfig;
	thresholdTextureConfig.Target = GL_TEXTURE_2D;
	thresholdTextureConfig.InternalFormat = GL_RGBA32F;
	thresholdTextureConfig.Format = GL_RGBA;
	thresholdTextureConfig.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	thresholdTextureConfig.MagFilter = GL_LINEAR_MIPMAP_LINEAR;

	FramebufferRenderbufferConfig thresholdRenderbufferConfig;
	thresholdRenderbufferConfig.InternalFormat = GL_DEPTH_STENCIL;

	FramebufferConfig thresholdConfig;
	thresholdConfig.Width = 1920;
	thresholdConfig.Height = 1080;
	thresholdConfig.Textures.push_back(thresholdTextureConfig);
	thresholdConfig.Renderbuffers.push_back(thresholdRenderbufferConfig);

	m_ThresholdFramebuffer = Framebuffer::Create(thresholdConfig);
}

void Renderer::InitializeShadowMapFramebuffers()
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

	m_DirectionalLightShadowMapFramebuffer = Framebuffer::Create(config);

	// POINT LIGHT
	glGenFramebuffers(1, &m_PointLightShadowMapFramebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// SPOT LIGHT
	glGenFramebuffers(1, &m_SpotLightShadowMapFramebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Renderer::RenderScene(Ref<Scene> scene)
{
	scene->PreRender();

	m_MainSceneFramebuffer->Bind();

	glClearColor(scene->GetBackgroundColor()->x, scene->GetBackgroundColor()->y, scene->GetBackgroundColor()->z, scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->Render();

	m_MainSceneFramebuffer->Unbind();
}

void Renderer::AddPostProcessingEffects()
{
	m_ThresholdFramebuffer->Bind();

	auto thresholdShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Threshold");
	thresholdShader->Use();
	thresholdShader->SetInt("u_Screen", 0);

	glBindVertexArray(m_PostProcessingVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_MainSceneFramebuffer->GetColorAttachment());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	m_ThresholdFramebuffer->Unbind();

	m_PostProcessingFramebuffer->Bind();

	glBindTexture(GL_TEXTURE_2D, m_ThresholdFramebuffer->GetColorAttachment());
	glGenerateMipmap(GL_TEXTURE_2D);

	auto postProcessingShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "PostProcessing");
	postProcessingShader->Use();
	postProcessingShader->SetInt("u_Screen", 0);
	postProcessingShader->SetInt("u_ThresholdScreen", 1);
	postProcessingShader->SetFloat("u_Gamma", m_Gamma);
	postProcessingShader->SetFloat("u_Exposure", m_Exposure);

	glBindVertexArray(m_PostProcessingVAO);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_MainSceneFramebuffer->GetColorAttachment());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ThresholdFramebuffer->GetColorAttachment());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	m_PostProcessingFramebuffer->Unbind();

}

void Renderer::RenderShadowMap(Scene* scene, DirectionalLight* source)
{
	m_DirectionalLightShadowMapFramebuffer->Bind();

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();
	depthShader->SetMat4("u_LightSpace", source->GetLightSpace());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}


	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthInstanced");
	depthIstancedShader->Use();
	depthIstancedShader->SetMat4("u_LightSpace", source->GetLightSpace());
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	m_DirectionalLightShadowMapFramebuffer->Unbind();
}

void Renderer::RenderShadowMap(Scene* scene, PointLight* source)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPoint");
	depthShader->Use();
	for (int i = 0; i < 6; i++)
		depthShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthShader->SetFloat("u_FarPlane", source->GetFarPlane());
	depthShader->SetVec3("u_LightPos", source->GetOwner()->GetWorldPosition());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}
	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPointInstanced");
	depthIstancedShader->Use();
	for (int i = 0; i < 6; i++)
		depthIstancedShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthIstancedShader->SetFloat("u_FarPlane", source->GetFarPlane());
	depthIstancedShader->SetVec3("u_LightPos", source->GetOwner()->GetWorldPosition());
	
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderShadowMap(Scene* scene, SpotLight* source)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();
	depthShader->SetMat4("u_LightSpace", source->GetLightSpace());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}
	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthInstanced");
	depthIstancedShader->Use();
	depthIstancedShader->SetMat4("u_LightSpace", source->GetLightSpace());
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
