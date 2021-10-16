#pragma once

#include <glm/glm.hpp>

class Framebuffer
{
private:
	uint32_t m_ID;
	unsigned int m_ColorAttachment;
	unsigned int m_DepthAttachment;
	unsigned int m_Width;
	unsigned int m_Height;

public:
	Framebuffer(unsigned int width, unsigned int height);
	~Framebuffer();

	void Bind();
	void Unbind();

	inline unsigned int GetColorAttachment() const { return m_ColorAttachment; }
};