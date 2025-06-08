#pragma once
#include <GL/gl.h>
#include <vector>
#include "stb_image_write.h"

inline void SaveTextureToPNG(GLuint textureID, int width, int height, const char* filename)
{
	std::vector<unsigned char> pixels(width * height * 4); // RGBA
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	stbi_write_png(filename, width, height, 4, pixels.data(), width * 4);
}