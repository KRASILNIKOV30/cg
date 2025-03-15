#include "BaseWindow.h"
#include "controller/Controller.h"
#include "view/View.h"
#include "model/Model.h"
#include "view/SoundView.h"

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLFWInitializer initGLFW;
	Model model{};
	View view(model);
	Controller controller(model, view);
	SoundView soundView(model);
	controller.Run();
}