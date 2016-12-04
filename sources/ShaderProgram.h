
#pragma once
#ifndef SHADERPROGRAM_HEADER
#define SHADERPROGRAM_HEADER

#include <string>
#include <GL/glew.h>

class ShaderProgram
{

public:
	ShaderProgram(const std::string & vertexShaderStr, const std::string & fragmentShaderStr);
	
	~ShaderProgram();
	
	bool Use() const;
	
	const bool IsValid() const;
	
	const std::string GetErrorMessage() const;
	
	bool SetUniformIfExistMatrix4fv(const char * uniformName, const GLfloat * value);
	
	static bool ReadShaderStrFromFile(const char * filePath, std::string & outStr);
	
private:
	ShaderProgram();
	
	//bool m_isValid;
	
	std::string m_errorMsg;
	
	GLuint m_shaderProgramID;
};

#endif
