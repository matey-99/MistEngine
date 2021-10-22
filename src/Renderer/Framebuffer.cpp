#include "Framebuffer.h"

#include <glad/glad.h>

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
{
	Resize(width, height);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteRenderbuffers(1, &m_DepthAttachment);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;

	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteRenderbuffers(1, &m_DepthAttachment);
	}

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	glGenTextures(1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glGenRenderbuffers(1, &m_DepthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is incomplete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
