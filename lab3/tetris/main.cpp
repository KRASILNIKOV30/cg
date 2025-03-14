#include "BaseWindow.h"
#include "controller/Controller.h"
#include "view/View.h"
#include "model/Model.h"

int main()
{
	GLFWInitializer initGLFW;
	Model model{};
	View view(model);
	Controller controller(model, view);
	controller.Run();
}