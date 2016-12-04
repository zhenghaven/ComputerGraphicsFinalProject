
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ShaderProgram.h"

enum ShaderType
{
	Vertex,
	Fragment
};

constexpr int kNumCharsInfoLog = 512;

GLuint CompileShader(const std::string & shaderStr, const ShaderType shaderType, std::string & errorMsg) {
	GLuint shaderID = 0;
	GLenum shaderTypeGL;
	errorMsg.clear();

	switch(shaderType)
	{
		case Vertex:
			shaderTypeGL = GL_VERTEX_SHADER;
			break;
		case Fragment:
			shaderTypeGL = GL_FRAGMENT_SHADER;
			break;
	}

	shaderID = glCreateShader(shaderTypeGL);
	const char* shaderStrPtr = shaderStr.c_str();
	glShaderSource(shaderID, 1, &shaderStrPtr, nullptr);
	glCompileShader(shaderID);
	GLint glResult = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &glResult);

	if(!glResult)
	{
		errorMsg.resize(kNumCharsInfoLog);
		glGetShaderInfoLog(shaderID, kNumCharsInfoLog, nullptr, &errorMsg.front());
		shaderID = 0;
	}
	return shaderID;
}

GLuint CreateShaderProgram(const GLuint vertexShaderID, const GLuint fragmentShaderID, std::string & errorMsg) {
		GLuint ProgramID = 0;
		errorMsg.clear();
		
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShaderID);
		glAttachShader(ProgramID, fragmentShaderID);
		glLinkProgram(ProgramID);
		GLint linkResult = 0;
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &linkResult);
		if(!linkResult)
		{
			errorMsg.resize(kNumCharsInfoLog);
			glGetProgramInfoLog(ProgramID, kNumCharsInfoLog, nullptr, &errorMsg.front());
			ProgramID = 0;
		}
		return ProgramID;
}
  
ShaderProgram::ShaderProgram(const std::string & vertexShaderStr, const std::string & fragmentShaderStr)
{
	GLuint vertexShaderID = CompileShader(vertexShaderStr, ShaderType::Vertex, m_errorMsg);
	
	if(vertexShaderID != 0)
	{
		GLuint fragmentShaderID = CompileShader(fragmentShaderStr, ShaderType::Fragment, m_errorMsg);
		
		if(fragmentShaderID != 0)
		{
			m_shaderProgramID = CreateShaderProgram(vertexShaderID, fragmentShaderID, m_errorMsg);
			glDeleteShader(fragmentShaderID);
		}
		glDeleteShader(vertexShaderID);
	}
}

ShaderProgram::~ShaderProgram()
{
	if(IsValid())
	{
		glDeleteProgram(m_shaderProgramID);
	}
}

bool ShaderProgram::Use() const 
{
	if (IsValid()) 
	{
		glUseProgram(m_shaderProgramID);
		return true;
	}
	return false;
}

const bool ShaderProgram::IsValid() const
{
	return m_shaderProgramID != 0;
}

const std::string ShaderProgram::GetErrorMessage() const
{
	return m_errorMsg;
}

bool ShaderProgram::SetUniformIfExistMatrix4fv(const char * uniformName, const GLfloat * value)
{
	const GLint location = glGetUniformLocation(m_shaderProgramID, uniformName);
	if(location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
		return true;
	}
	else
	{
		return false;
	}
}

bool ShaderProgram::ReadShaderStrFromFile(const char * filePath, std::string & outStr)
{
	std::ifstream in(filePath);
	if (!in.is_open())
	{
		return false;
	}
	std::stringstream string_buffer;
	string_buffer << in.rdbuf();
	in.close();
	outStr = string_buffer.str();
	return true;
}

ShaderProgram::ShaderProgram() : 
	m_shaderProgramID(0)
{

}

