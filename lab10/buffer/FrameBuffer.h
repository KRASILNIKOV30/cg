#pragma once
#include <GL/gl.h>
#include <cassert>

// Базовый класс "Буфер кадра"
class CFrameBufferBase
{
public:
	// Создание нового буфера кадра
	void Create()
	{
		assert(!m_frameBuffer);
		glGenRenderbuffers(1, &m_frameBuffer);
	}

	// Привязка буфера кадра к указанной цели
	static void Bind(GLuint target, GLuint framebuffer)
	{
		glBindFramebuffer(target, framebuffer);
	}

	// Привязка текущего буфера кадра
	void Bind(GLenum target = GL_FRAMEBUFFER) const
	{
		assert(m_frameBuffer != 0);
		glBindFramebuffer(target, m_frameBuffer);
	}

	// Присоединение буфера визуализации к буферу кадра
	static void SetRenderBuffer(
		GLenum target, GLenum attachment,
		GLenum renderbuffertarget, GLuint renderbuffer)
	{
		glFramebufferRenderbuffer(
			target, attachment, renderbuffertarget, renderbuffer);
	}

	// Присоединение двухмерной текстуры к буферу кадра
	static void SetTexture2D(
		GLenum target, GLenum attachment,
		GLenum textarget, GLuint texture, GLint level)
	{
		glFramebufferTexture2D(
			target, attachment, textarget, texture, level);
	}

	// Проверка статуса буфера кадра

	static GLenum CheckStatus(GLenum target)
	{
		return glCheckFramebufferStatus(target);
	}

	// Удаление буфера кадра
	void Delete()
	{
		assert(m_frameBuffer != 0);
		glDeleteFramebuffers(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}

	// Получение идентификатора буфера кадра
	[[nodiscard]] GLuint Get() const
	{
		return m_frameBuffer;
	}

	// Получение идентификатора буфера кадра
	operator GLuint() const
	{
		return m_frameBuffer;
	}

protected:
	CFrameBufferBase(GLuint frameBuffer = 0)
		: m_frameBuffer(0)
	{
	}

	~CFrameBufferBase()
	{
	}

	// Задание идентификатора буфера кадра
	void SetFrameBuffer(GLuint frameBuffer)
	{
		m_frameBuffer = frameBuffer;
	}

private:
	// Запрещаем копирование и присваивание
	// экземпляров класса CFrameBufferBase
	CFrameBufferBase(CFrameBufferBase const&);
	CFrameBufferBase& operator=(CFrameBufferBase const&);

	GLuint m_frameBuffer;
};

// Управляемая или неуправляемая реализация буфера кадра
template <bool t_managed>
class CFrameBufferImpl : public CFrameBufferBase
{
public:
	CFrameBufferImpl(GLuint frameBuffer = 0)
		: CFrameBufferBase(frameBuffer)
	{
	}

	~CFrameBufferImpl()
	{
		// Удаляем буфер кадра, если класс управляет временем
		// его жизни
		if (t_managed && (Get() != 0))
		{
			Delete();
		}
	}

private:
};

// Буфер кадра (класс управляет временем жизни объекта OpenGL)
typedef CFrameBufferImpl<true> FrameBuffer;

// Дескриптор буфера кадра (класс не управляет временем
// жизни объекта OpenGL)
typedef CFrameBufferImpl<false> FrameBufferHandle;