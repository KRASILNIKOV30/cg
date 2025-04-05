#include "./TextureLoader.h"
#include <SOIL/SOIL.h>
#include <stdexcept>
#include <GL/glu.h>

TextureLoader::TextureLoader()
	: m_buildMipmaps(true),
	  m_wrapS(GL_REPEAT),
	  m_wrapT(GL_REPEAT),
	  m_minFilter(GL_LINEAR_MIPMAP_LINEAR),
	  m_magFilter(GL_LINEAR)
{
}

GLuint TextureLoader::LoadTexture2D(std::string const& fileName, GLuint textureName, GLint level) const
{
	// Загрузка изображения с помощью SOIL
	int width, height, channels;
	unsigned char* image = SOIL_load_image(
		fileName.c_str(),
		&width, &height, &channels,
		SOIL_LOAD_AUTO);

	if (!image)
	{
		throw std::runtime_error(std::string("Error loading texture: ") + SOIL_last_result());
	}

	// Определение формата текстуры
	GLenum format;
	switch (channels)
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		SOIL_free_image_data(image);
		throw std::runtime_error("Unsupported texture format");
	}

	// Создание или использование существующей текстуры
	GLuint texture = textureName;
	if (texture == 0)
	{
		glGenTextures(1, &texture);
	}

	glBindTexture(GL_TEXTURE_2D, texture);

	if (m_buildMipmaps)
	{
		gluBuild2DMipmaps(
			GL_TEXTURE_2D,
			format,
			width, height,
			format,
			GL_UNSIGNED_BYTE,
			image);
	}
	else
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			level,
			format,
			width, height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			image);
	}

	// Установка параметров текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);

	SOIL_free_image_data(image);
	return texture;
}

void TextureLoader::SetMinFilter(GLenum minFilter)
{
	m_minFilter = minFilter;
}

void TextureLoader::SetMagFilter(GLenum magFilter)
{
	m_magFilter = magFilter;
}

void TextureLoader::BuildMipmaps(bool buildMipmaps)
{
	m_buildMipmaps = buildMipmaps;
}

void TextureLoader::SetWrapMode(GLenum wrapS, GLenum wrapT)
{
	m_wrapS = wrapS;
	m_wrapT = wrapT;
}