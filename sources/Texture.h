
#pragma once
#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>


class Texture
{
public:
	Texture(const std::string & filePath);
	
	~Texture();
	
	bool IsValid() const;
	
	void BindTexture() const;
	
	void UnBindTexture() const;


protected:

	static bool LoadTexture(const std::string & filePath, GLuint & outTextureID); 

private:
	Texture();

	GLuint m_textureID;
	
};


#endif
