#include "render/RaytraceView.h"
#include <iostream>

int main()
{
	try
	{
		RaytraceView view{ 800, 600 };
		view.Run();
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}
