#include "Window.h"
#include <stdexcept>

Window::Window(int width, int height, const char* title)
	: m_width(width), m_height(height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(SDL_GetError());

	m_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_width,
		m_height,
		SDL_WINDOW_SHOWN
		);

	if (!m_window)
		throw std::runtime_error(SDL_GetError());

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (!m_renderer)
		throw std::runtime_error(SDL_GetError());

	m_framebufferTexture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		m_width,
		m_height
		);
}

Window::~Window()
{
	if (m_framebufferTexture)
		SDL_DestroyTexture(m_framebufferTexture);
	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);
	if (m_window)
		SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::Run()
{
	SDL_Event event;
	while (!m_shouldQuit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				m_shouldQuit = true;
			OnEvent(event);
		}

		OnUpdate();
		OnRender();

		SDL_RenderPresent(m_renderer);
		SDL_Delay(16); // ~60 FPS
	}
}

void Window::UpdateTextureFromPixels(const void* pixels, int pitch) const
{
	SDL_UpdateTexture(m_framebufferTexture, nullptr, pixels, pitch);
	SDL_RenderCopy(m_renderer, m_framebufferTexture, nullptr, nullptr);
}