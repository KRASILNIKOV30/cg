#pragma once
#include <GL/glew.h>
#include <stdexcept>

class GLEWInitializer
{
public:
	GLEWInitializer()
	{
		struct Init
		{
			Init()
			{
				if (glewInit() != GLEW_OK)
				{
					throw std::runtime_error("Failed to initialize GLEW");
				}
			}
		};
		static Init init;
	}
};