#include "view/RaytraceView.h"
#include <iostream>

int main()
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error(SDL_GetError());
		}

		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
		{
			SDL_Quit();
			throw std::runtime_error(SDL_GetError());
		}

		RaytraceView view{ 800, 600 };
		view.Run();
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}
