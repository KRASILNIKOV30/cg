#include <GL/glew.h>
#include "Window.h"
#include "../../lib/glfwWindow/GLFWInitializer.h"

#include <cstdlib>

int main()
{
	glewInit();
	GLFWInitializer initGLFW;
	Window window{ 800, 600, "canabola" };
	window.Run();

	return EXIT_SUCCESS;
}
