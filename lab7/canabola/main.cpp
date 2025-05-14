#include <GL/glew.h>
#include "Window.h"
#include "../../lib/glfwWindow/GLFWInitializer.h"

#include <cstdlib>

int main()
{
	glewInit();
	GLFWInitializer initGLFW;
	Window window{ 800, 600, "Battle City 3D" };
	window.Run();

	return EXIT_SUCCESS;
}
