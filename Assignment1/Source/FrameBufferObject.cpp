#include "FrameBufferObject.h"
#include <iostream>


FrameBufferObject::FrameBufferObject()
{
	glGenFramebuffers(1, &m_FrameBufferObject);
	glGenTextures(1, &m_Texture);
}


FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &m_FrameBufferObject);
	glDeleteTextures(1, &m_Texture);
}
bool FrameBufferObject::Init(unsigned int i_Width, unsigned int i_Height)
{
	//Screen size
	width = i_Width;
	height = i_Height;

	//Binding FrameBufferOBject
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
	//Binding Texture into current frame buffer

	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, i_Height, i_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture, 0);

	glDrawBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	//Checking if framebuffer is completed
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not completed" << std::endl;
		return false;
	}
	return true;
}
void FrameBufferObject::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
	//Setting viewport to screen
	glViewport(0, 0, width, height);
}
void FrameBufferObject::BindTexture(GLenum texture)
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}
GLuint FrameBufferObject::getBufferObject()
{
	return m_FrameBufferObject;
}
GLuint FrameBufferObject::getTexture()
{
	return m_Texture;
}
