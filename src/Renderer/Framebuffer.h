#pragma once

#include <glm/glm.hpp>

class Framebuffer
{
private:
	uint32_t m_ID = 0;
	unsigned int m_ColorAttachment = 0;
	unsigned int m_DepthAttachment = 0;
	unsigned int m_Width;
	unsigned int m_Height;

public:
	Framebuffer(unsigned int width, unsigned int height);
	~Framebuffer();

	void Bind();
	void Unbind();

	void Resize(unsigned int width, unsigned int height);

	inline unsigned int GetColorAttachment() const { return m_ColorAttachment; }
	inline unsigned int GetDepthAttachment() const { return m_DepthAttachment; }
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
};