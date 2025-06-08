#pragma once
#include <GLES2/gl2.h>
#include <cassert>

// Базовый буфер физуализации
class CRenderBufferBase
{
public:
	// Создание буфера визуализации
	void Create()
	{
		assert(!m_renderBuffer);
		glGenRenderbuffers(1, &m_renderBuffer);
	}

	// Привязка к текущему буферу визуализации
	void Bind(GLenum target = GL_RENDERBUFFER) const
	{
		assert(m_renderBuffer != 0);
		glBindRenderbuffer(target, m_renderBuffer);
	}

	// Привязка к заданному буферу визуализации
	static void Bind(GLenum target, GLenum renderBuffer)
	{
		glBindRenderbuffer(target, renderBuffer);
	}

	// Удаление буфера визуализации
	void Delete()
	{
		assert(m_renderBuffer != 0);
		glDeleteRenderbuffers(1, &m_renderBuffer);
		m_renderBuffer = 0;
	}

	// Получение идентификатора буфера визуализации
	GLuint Get() const
	{
		return m_renderBuffer;
	}

	// Получение идентификатора буфера визуализации
	operator GLuint() const
	{
		return m_renderBuffer;
	}

	// Выделение памяти под хранение буфера кадра заданного размера
	static void SetStorage(
		GLenum target, GLenum internalformat,
		GLsizei width, GLsizei height)
	{
		glRenderbufferStorage(target, internalformat, width, height);
	}

protected:
	CRenderBufferBase(GLuint renderBuffer = 0)
		: m_renderBuffer(renderBuffer)
	{
	}

	// Задание идентификатора буфера визуализации
	void SetRenderBuffer(GLuint renderBuffer)
	{
		m_renderBuffer = renderBuffer;
	}

	~CRenderBufferBase()
	{
	}

private:
	// Запрещаем конструктор копирования и оператор присваивания
	CRenderBufferBase(CRenderBufferBase const&);
	CRenderBufferBase& operator=(CRenderBufferBase const&);

	GLuint m_renderBuffer;
};

// Управляемая, либо неуправляемая реализация
// класса "Буфер визуализации"
template <bool t_managed>
class CRenderBufferImpl : public CRenderBufferBase
{
public:
	CRenderBufferImpl(GLuint renderBuffer = 0)
		: CRenderBufferBase(renderBuffer)
	{
	}

	~CRenderBufferImpl()
	{
		// Удаляем буфер визуализации
		if (t_managed && (Get() != 0))
		{
			Delete();
		}
	}
};

// Буфер визуализации (класс управляем временем жизни объекта OpenGL)
typedef CRenderBufferImpl<true> RenderBuffer;

// Дескриптор буфера визуализации
// (класс не управляет временем жизни объекта OpenGL)
typedef CRenderBufferImpl<false> RenderBufferHandle;