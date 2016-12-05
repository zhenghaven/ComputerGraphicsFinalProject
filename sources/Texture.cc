#include "Texture.h"

#define cimg_display 0
#include <CImg.h>

#include <GLFW/glfw3.h>


Texture::Texture(const std::string & filePath) :
	m_textureID(0)
{
	LoadTexture(filePath, m_textureID);
}

bool Texture::IsValid() const
{
	return m_textureID != 0;
}

void Texture::BindTexture() const
{
	if(IsValid())
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
}

void Texture::UnBindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::LoadTexture(const std::string & filePath, GLuint & outTextureID)
{
	cimg_library::CImg<unsigned char> image;
	image.load(filePath.c_str());
	const int width = image.width();
	const int height = image.height();
	image.permute_axes("cxyz");
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	outTextureID = texture_id;
}

Texture::Texture() :
	m_textureID(0)
{

}
