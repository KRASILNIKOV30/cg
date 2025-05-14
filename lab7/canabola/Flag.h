#pragma once
#include "../../lib/texture/Texture.h"
#include "../../lib/shaders/Program.h"

constexpr GLuint FRAME_BUFFER_WIDTH = 800;
constexpr GLuint FRAME_BUFFER_HEIGHT = 600;

class Flag
{
public:
	Flag()
		: m_program{ R"(
varying vec2 texCoord;

void main()
{
	texCoord = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}
)",
		             R"(
varying vec2 texCoord;

void main()
{
	float pos = texCoord.t;
	vec4 whiteColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 blueColor = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 redColor = vec4(1.0, 0, 0, 1.0);

	if (pos < 0.33)
	{
		gl_FragColor = redColor;
	}
	else if (pos < 0.66)
	{
		gl_FragColor = blueColor;
	}
	else
	{
		gl_FragColor = whiteColor;
	}

	if (!gl_FrontFacing)
	{
		gl_FragColor = (vec4(0, 1.0, 0, 1.0));
	}
}
)" }
	{
		m_colorBufferTexture.Create();
		m_colorBufferTexture.Bind();
	}

	void Render() const
	{
		glUseProgram(m_program.GetId());

		glEnable(GL_TEXTURE_2D);
		m_colorBufferTexture.Bind();

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(-2, -1.5);

			glTexCoord2f(1, 0);
			glVertex2f(2, -1.5);

			glTexCoord2f(1, 1);
			glVertex2f(2, 1.5);

			glTexCoord2f(0, 1);
			glVertex2f(-2, 1.5);
		}
		glEnd();

		glUseProgram(0);
	}

private:
	Program m_program;
	Texture2D m_colorBufferTexture;
};