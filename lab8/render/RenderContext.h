#pragma once
#include "../geometry/matrix/Matrix4.h"
#include "../buffer/IColorBuffer.h"
#include "../geometry/Ray.h"
#include "../scene/Scene.h"
#include "ViewPort.h"

/*
Класс RenderContext - контекст визуализации
Выполняет вычисление цвета пикселя в заданных координатах внутри буфера кадра
методом трассировки лучей
Видовой порт задает прямоугольную область в буфере кадра, которая принадлежит контексту рисования
Матрицы моделирования-вида и проецирования аналогично соответствующим матрицам OpenGL
*/
class RenderContext
{
public:
	RenderContext();

	/*
	Трассировка лучей по сцене в заданной прямоугольной области экрана
	Работает немного быстрее независимых вызовов метода CalculatePixelColor
	за счет сокращения числа вычислений для определения направления луча.
	Результат записывается в объект, реализующий интерфейс IColorBuffer
	*/
	void RenderRectangle(Scene const& scene, int left, int top, unsigned width, unsigned height, IColorBuffer& colorBuffer) const;

	/*
	Трассирует путь луча по сцене, проходящего через пиксель с указанными координатами
	Возвращает цвет данного пикселя в формате 0xAARRGGBB
	*/
	[[nodiscard]] std::uint32_t CalculatePixelColor(Scene const& scene, int x, int y) const;

	/*
	Задает параметры видового порта
	*/
	void SetViewPort(ViewPort const& viewPort);

	/*
	Устанавливает матрицу проецирования (перспективного, либо ортографического)
	*/
	void SetProjectionMatrix(Matrix4d const& projectionMatrix);

	/*
	Устанавливает матрицу моделирования-вида, задающую положение и ориентацию камеры в пространстве
	*/
	void SetModelViewMatrix(Matrix4d const& modelViewMatrix);

private:
	/*
	Преобразовывает экранные координаты пикселя в нормализованные экранные координаты
	В нормализованных координатах верхний левый угол видового порта имеет координаты (-1, +1), 
		а правый нижний: (+1, -1)
	*/
	[[nodiscard]] Vector2d GetNormalizedViewportCoord(double x, double y) const;

	/*
	Возвращает матрицу, обратную произведению матриц проецирования и моделирования-вида
	*/
	[[nodiscard]] Matrix4d const& GetInverseModelViewProjectionMatrix() const;

	/*
	Вычисляет координаты точки, имеющей нормализованные экранные координатами screenX, screenY и глубину depth
	в мировой системе координат.
	Глубине=0 соответствует ближняя плоскость отсечения, задаваемая матрицей проецирования, а глубине=1 - дальняя
	*/
	[[nodiscard]] Vector3d UnProject(double normalizedX, double normalizedY, double depth) const;

	/*
	Вычисляет значение матрицы обратной произведению матриц проецирования и
	моделирования-вида
	*/
	void UpdateInverseModelViewProjectionMatrix();

	// Вычисление направления луча, проходящего в заданных экранных координатах
	[[nodiscard]] Ray CalculateRay(double screenX, double screenY) const;

	// Квантование цвет в 32-битный ARGB-формат (0xAARRGGBB)
	static std::uint32_t QuantizeColor(Vector4f const& color);

private:
	Matrix4d m_projectionMatrix;
	Matrix4d m_modelViewMatrix;
	Matrix4d m_inverseModelViewProjectionMatrix;
	ViewPort m_viewPort;
};