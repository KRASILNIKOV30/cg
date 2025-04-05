#include "../lib/GLFWInitializer.h"
#include "Window.h"

int main()
{
	GLFWInitializer initGLFW;
	Window window{ 800, 600, "Maze with textures" };
	window.Run();

	return EXIT_SUCCESS;
}