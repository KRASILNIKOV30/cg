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
uniform vec2 step;
uniform float coefficients[20];

void main()
{
    vec2 pos = gl_TexCoord[0].st;

    vec4 color =
        texture2D(image, pos) * coefficients[0] +
        (texture2D(image, pos - step) +
            texture2D(image, pos + step)) * coefficients[1] +
        (texture2D(image, pos - 2.0 * step) +
            texture2D(image, pos + 2.0 * step)) * coefficients[2] +
        (texture2D(image, pos - 3.0 * step) +
            texture2D(image, pos + 3.0 * step)) * coefficients[3] +
        (texture2D(image, pos - 4.0 * step) +
            texture2D(image, pos + 4.0 * step)) * coefficients[4] +
        (texture2D(image, pos - 5.0 * step) +
            texture2D(image, pos + 5.0 * step)) * coefficients[5] +
        (texture2D(image, pos - 6.0 * step) +
            texture2D(image, pos + 6.0 * step)) * coefficients[6] +
        (texture2D(image, pos - 7.0 * step) +
            texture2D(image, pos + 7.0 * step)) * coefficients[7] +
        (texture2D(image, pos - 8.0 * step) +
            texture2D(image, pos + 8.0 * step)) * coefficients[8] +
        (texture2D(image, pos - 9.0 * step) +
            texture2D(image, pos + 9.0 * step)) * coefficients[9];

    gl_FragColor = color;
}
)" }
	{
		m_stepLocation = m_program.GetUniformLocation("step");
		m_imageLocation = m_program.GetUniformLocation("image");
		m_coefficientsLocation = m_program.GetUniformLocation("coefficients");
	}

	GLuint Blur(GLuint texture, int width, int height) const
	{
		PrepareFrameBuffer(width, height);

		m_frameBuffer.Bind();

		SetupViewportAndMatrices();

		glUseProgram(m_program.GetId());
		glUniform1i(m_imageLocation, 0);
		const auto gauss = CalcGauss();
		glUniform1fv(m_coefficientsLocation, static_cast<GLsizei>(gauss.size()), gauss.data());

		BlurHorizontal(texture);
		BlurVertical();

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
		// Задаем шаг выборки значений из текстуры
		glUniform2f(m_stepLocation, 1.0f / static_cast<float>(m_width), 0);

		// Присоединяем 0 уровень деталей текстуры m_colorBuffer0
		// в качестве буфера цвета во вспомогательном буфере кадра
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
		m_colorBuffer0.Bind();
		glUniform2f(m_stepLocation, 0, 1.0f / static_cast<float>(m_height));
		// Устанавливаем 0 уровень деталей текстуры m_colorBuffer0
		// в качестве буфера цвета во вспомогательном буфере кадра
		FrameBuffer::SetTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			m_colorBuffer1,
			0);

		assert(FrameBuffer::CheckStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		DrawRectangle();
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
		bool createFrameBuffer = !m_frameBuffer;
		bool createTexture = createFrameBuffer || !m_colorBuffer0 || !m_colorBuffer1;
		bool updateTextureImage = createTexture || (m_width != width) || (m_height != height);
		bool updateMipmaps = updateTextureImage || createTexture;

		if (createFrameBuffer)
		{
			m_frameBuffer.Create();
		}

		if (createTexture)
		{
			m_colorBuffer0.Create();
			m_colorBuffer1.Create();
		}

		m_width = width;
		m_height = height;

		if (updateTextureImage || updateMipmaps)
		{
			m_colorBuffer0.Bind();
		}

		if (updateTextureImage)
		{
			m_colorBuffer0.Bind();
			// Для первого буфера цвета фильтрация будет осуществляться с выбором
			// ближайшего пикселя
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Объявляем 0 уровень детализации текстурного изображения
			Texture2D::TexImage(
				0, GL_RGBA,
				width, height,
				0, GL_RGBA,
				GL_UNSIGNED_BYTE, nullptr);
		}

		// Выбираем текстуру 1 буфера цвета
		if (updateTextureImage || updateMipmaps)
		{
			m_colorBuffer1.Bind();
		}

		if (updateTextureImage)
		{
			// Объявляем 0 уровень детализации текстурного изображения
			Texture2D::TexImage(
				0, GL_RGBA,
				static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height),
				0, GL_RGBA,
				GL_UNSIGNED_BYTE, nullptr);
		}

		if (updateMipmaps)
		{
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER,
				GL_NEAREST);
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_MAG_FILTER,
				GL_NEAREST);
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

	mutable FrameBuffer m_frameBuffer;
	mutable Texture2D m_colorBuffer0;
	mutable Texture2D m_colorBuffer1;
	mutable unsigned m_width = 0;
	mutable unsigned m_height = 0;
};