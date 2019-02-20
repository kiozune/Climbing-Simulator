#ifndef _FRAMEBUFFEROBJECT_H
#define _FRAMEBUFFEROBJECT_H

#include "GL/glew.h"

class FrameBufferObject
{
private:
	GLuint m_FrameBufferObject;
	GLuint m_Texture;
	unsigned int width;
	unsigned int height;
public:
	FrameBufferObject();
	~FrameBufferObject();

	bool Init(unsigned int i_Width, unsigned int i_Height);
	void BindFrameBuffer();
	void BindTexture(GLenum texture);
	GLuint getTexture();
	GLuint getBufferObject();
};

#endif