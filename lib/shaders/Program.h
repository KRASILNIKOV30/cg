#pragma once
#include "Shader.h"
#include "ShaderProgram.h"

class Program
{
public:
	Program(const char* vertexShaderSource, const char* fragmentShaderSource);

	[[nodiscard]] GLuint GetId() const noexcept { return m_program; }

	[[nodiscard]] GLint GetUniformLocation(const GLchar* name) const
	{
		return m_program.GetUniformLocation(name);
	}

private:
	Shader m_vertexShader{ GL_VERTEX_SHADER };
	Shader m_fragmentShader{ GL_FRAGMENT_SHADER };
	ShaderProgram m_program;
};