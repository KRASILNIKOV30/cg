#pragma once
#include "../lib/shaders/Program.h"
#include "../lib/texture/Texture.h"
#include "buffer/FrameBuffer.h"
#include "../lib/texture/SaveTextureToPNG.h"
#include <cmath>
#include <vector>
#include <iostream>

constexpr int RADIUS = 10;
constexpr float SIGMA = 3.3f;
constexpr float PERSISTENCE = 0.8f;

class Gauss
{
public:
	Gauss()
		: m_program{ R"(
void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
)",
			R"(
uniform sampler2D image;
uniform sampler2D prevImage;
uniform vec2 step;
uniform float coefficients[10];
uniform float persistence;
uniform bool firstPass;
#define GAMMA 2.2

vec4 srgbToLinear(vec4 color) {
	return vec4(pow(color.rgb, vec3(GAMMA)), color.a);
}

vec4 linearToSrgb(vec4 color) {
	if (firstPass) {
		return color;
	}
    return vec4(pow(color.rgb, vec3(1.0 / GAMMA)), color.a);
}

vec4 getSample(sampler2D image, vec2 pos) {
	if (firstPass) {
		return srgbToLinear(texture2D(image, pos));
	}
	return texture2D(image, pos);
}

void main()
{
    vec2 pos = gl_TexCoord[0].st;

    vec4 color =
        getSample(image, pos) * coefficients[0] +
        (getSample(image, pos - step) +
            getSample(image, pos + step)) * coefficients[1] +
        (getSample(image, pos - 2.0 * step) +
            getSample(image, pos + 2.0 * step)) * coefficients[2] +
        (getSample(image, pos - 3.0 * step) +
            getSample(image, pos + 3.0 * step)) * coefficients[3] +
        (getSample(image, pos - 4.0 * step) +
            getSample(image, pos + 4.0 * step)) * coefficients[4] +
        (getSample(image, pos - 5.0 * step) +
            getSample(image, pos + 5.0 * step)) * coefficients[5] +
        (getSample(image, pos - 6.0 * step) +
            getSample(image, pos + 6.0 * step)) * coefficients[6] +
        (getSample(image, pos - 7.0 * step) +
            getSample(image, pos + 7.0 * step)) * coefficients[7] +
        (getSample(image, pos - 8.0 * step) +
            getSample(image, pos + 8.0 * step)) * coefficients[8] +
        (getSample(image, pos - 9.0 * step) +
            getSample(image, pos + 9.0 * step)) * coefficients[9];

	vec4 prevColor = srgbToLinear(texture2D(prevImage, pos));
	float cutoff = 0.005;
    vec4 fadedPrevColor = max(prevColor * persistence - cutoff, 0.0);

    gl_FragColor = linearToSrgb(max(color, fadedPrevColor));
}
)" }
	{
		m_stepLocation = m_program.GetUniformLocation("step");
		m_imageLocation = m_program.GetUniformLocation("image");
		m_coefficientsLocation = m_program.GetUniformLocation("coefficients");
		m_prevImageLocation = m_program.GetUniformLocation("prevImage");
		m_persistenceLocation = m_program.GetUniformLocation("persistence");
		m_firstPassLocation = m_program.GetUniformLocation("firstPass");
	}

	GLuint Blur(GLuint texture, int width, int height) const
	{
		PrepareFrameBuffer(width, height);

		m_frameBuffer.Bind();

		SetupViewportAndMatrices();

		glUseProgram(m_program.GetId());
		glUniform1i(m_imageLocation, 0);
		glUniform1i(m_prevImageLocation, 1);
		const auto gauss = CalcGauss();
		glUniform1fv(m_coefficientsLocation, static_cast<GLsizei>(gauss.size()), gauss.data());

		BlurHorizontal(texture);
		BlurVertical();

		CopyTexture(m_colorBuffer1, m_prev);

		FrameBuffer::Bind(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		glDisable(GL_TEXTURE_2D);
		RestoreViewportAndMatrices();

		return m_colorBuffer1;
	}

private:
	void SetupViewportAndMatrices() const
	{
		// Сохраняем параметры видового порта в стеке атрибутов
		glPushAttrib(GL_VIEWPORT_BIT);
		// и задаем видовой порт размером со вспомогательный буфер кадра
		glViewport(0, 0, m_width, m_height);

		// Сохраняем матрицу моделирования-вида в стеке матриц
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		// и загружаем единичную матрицу моделирования вида
		glLoadIdentity();

		// Сохраняем матрицу проецирования в стеке матриц
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		// и задаем новую матрицу проецирования
		glLoadIdentity();
		glOrtho(0, m_width, m_height, 0, -1, 1);
	}

	void RestoreViewportAndMatrices() const
	{
		// Восстанавливаем матрицу проецироваиня
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// Восстанавливаем матрицу моделирования-вида
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		// Восстанавливаем видовой порт
		glPopAttrib();
	}

	void BlurHorizontal(GLuint texture) const
	{
		// Выполняем привязку к переданному текстурному объекту
		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(m_firstPassLocation, GL_TRUE);
		glUniform1f(m_persistenceLocation, 0.0f);
		glUniform2f(m_stepLocation, 1.0f / static_cast<float>(m_width), 0);

		FrameBuffer::SetTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			m_colorBuffer0,
			0);

		assert(
			FrameBuffer::CheckStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		DrawRectangle();
	}

	void BlurVertical() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorBuffer0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_prev);

		glUniform1i(m_firstPassLocation, GL_FALSE);
		glUniform1f(m_persistenceLocation, PERSISTENCE);
		glUniform2f(m_stepLocation, 0, 1.0f / static_cast<float>(m_height));

		FrameBuffer::SetTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			m_colorBuffer1,
			0);

		assert(FrameBuffer::CheckStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		DrawRectangle();
	}

	void CopyTexture(GLuint const& src, Texture2D& dst) const
	{
		glUseProgram(0);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, src);
		glEnable(GL_TEXTURE_2D);

		glColor4f(1, 1, 1, 1);

		FrameBuffer::SetTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			dst, 0);

		assert(FrameBuffer::CheckStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		DrawRectangle();
		glUseProgram(m_program.GetId());
	}

	void DrawRectangle() const
	{
		const auto width = static_cast<float>(m_width);
		const auto height = static_cast<float>(m_height);

		glBegin(GL_QUADS);
		{
			// Верхний левый
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, 0.0f);

			// Нижний левый
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, height);

			// Нижний правый
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(width, height);

			// Верхний правый
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(width, 0.0f);
		}
		glEnd();
	}

	void PrepareFrameBuffer(int width, int height) const
	{
		auto initRenderTargetTexture = [&](Texture2D& tex) {
			tex.Bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			Texture2D::TexImage(
				0, GL_RGBA,
				width, height,
				0, GL_RGBA,
				GL_UNSIGNED_BYTE, nullptr);
		};

		bool createFrameBuffer = !m_frameBuffer;
		bool createTexture = createFrameBuffer || !m_colorBuffer0 || !m_colorBuffer1 || !m_prev;
		bool updateTextureImage = createTexture || (m_width != width) || (m_height != height);

		if (createFrameBuffer)
		{
			m_frameBuffer.Create();
		}

		if (createTexture)
		{
			m_colorBuffer0.Create();
			m_colorBuffer1.Create();
			m_prev.Create();
		}

		m_width = width;
		m_height = height;

		if (updateTextureImage)
		{
			initRenderTargetTexture(m_colorBuffer0);
			initRenderTargetTexture(m_colorBuffer1);
			initRenderTargetTexture(m_prev);
		}
	}

	static std::vector<float> CalcGauss()
	{
		std::vector<float> gauss(RADIUS);
		const float k1 = 1.0f / sqrtf(2.0f * static_cast<float>(M_PI) * SIGMA * SIGMA);
		const float k2 = 1.0f / (2 * SIGMA * SIGMA);

		float sum = 0;

		for (size_t i = 0; i < RADIUS; ++i)
		{
			float k = k1 * expf(-k2 * static_cast<float>(i * i));
			gauss[i] = k;

			sum += k;

			if (i > 0)
			{
				sum += k;
			}
		}

		sum = 1.0f / sum;
		for (size_t i = 0; i < RADIUS; ++i)
		{
			gauss[i] *= sum;
		}

		return gauss;
	}

private:
	Program m_program;

	GLint m_stepLocation;
	GLint m_imageLocation;
	GLint m_coefficientsLocation;
	GLint m_prevImageLocation;
	GLint m_persistenceLocation;
	GLint m_firstPassLocation;

	mutable Texture2D m_prev;
	mutable FrameBuffer m_frameBuffer;
	mutable Texture2D m_colorBuffer0;
	mutable Texture2D m_colorBuffer1;
	mutable unsigned m_width = 0;
	mutable unsigned m_height = 0;
};