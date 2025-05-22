#pragma once
#include <SDL2/SDL.h>

class Window
{
public:
	Window(int width, int height, const char* title);
	virtual ~Window();

	void Run();

protected:
	// Методы для наследников
	virtual void OnUpdate()
	{
	} // Вызывается каждый кадр
	virtual void OnRender()
	{
	} // Отрисовка содержимого
	virtual void OnEvent(const SDL_Event& event)
	{
	} // Обработка событий

	// Доступ к рендереру и размерам окна
	[[nodiscard]] SDL_Renderer* GetRenderer() const { return m_renderer; }
	[[nodiscard]] int GetWidth() const { return m_width; }
	[[nodiscard]] int GetHeight() const { return m_height; }

	// Утилиты для работы с пикселями
	void UpdateTextureFromPixels(const void* pixels, int pitch) const;

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Texture* m_framebufferTexture = nullptr;
	int m_width, m_height;
	bool m_shouldQuit = false;
};