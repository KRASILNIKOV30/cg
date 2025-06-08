#include "../lib/glfwWindow/GLFWInitializer.h"
#include "window/Window.h"
#include <GL/glut.h>
#include <cstdlib>

int main(int argc, char* argv[])
{
	glewInit();
	glutInit(&argc, argv);
	GLFWInitializer initGLFW;
	Window window{ 800, 600, "canabola" };
	window.Run();

	return EXIT_SUCCESS;
}
