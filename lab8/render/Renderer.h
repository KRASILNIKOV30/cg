#pragma once
#include "RenderContext.h"
#include "../buffer/FrameBuffer.h"
#include <thread>

/*
	Класс CRenderer, выполняющий визуализацию сцены в буфере кадра.
	Работа по построению изображения в буфере кадра выполняется в
	отдельном потоке, дабы не замедлять работу основного приложения
*/
class Renderer
{
public:
	~Renderer();

	// Выполняется ли в данный момент построение изображения в буфере кадра?
	bool IsRendering() const;

	/*
		Сообщает о прогрессе выполнения работы:
			renderedChunks - количество обработанных блоков изображения
			totalChunks - общее количество блоков изображения
		Возвращаемое значение:
			true - изображение построено полностью
			false - изображение построено не полностью
	*/
	bool GetProgress(unsigned& renderedChunks, unsigned& totalChunks) const;

	/*
		Запускает фоновый поток для визуализации сцены в заданном буфере кадра
		Возвращает true, если поток был запущен и false, если поток запущен не был,
		т.к. не завершилась текущая операция по построению изображения в буфере кадра
	*/
	bool Render(Scene const& scene, RenderContext const& context, FrameBuffer& frameBuffer);

	/*
		Выполняет принудительную остановку фонового построения изображения.
		Данный метод следует вызывать до вызова деструкторов объектов, используемых классом CRenderer,
		если на момент их вызова выполняется построение изображения в буфере кадра
	*/
	void Stop();

private:
	/*
		Визуализация кадра, выполняемая в фоновом потоке
	*/
	void RenderFrame(Scene const& scene, RenderContext const& context, FrameBuffer& frameBuffer);

	// Устанавливаем потокобезопасным образом флаг о том, что идет построение изображения
	// Возвращаем true, если значение флага изменилось, и false, если нет
	bool SetRendering(bool rendering);

	// Устанавливаем флаг, сообщающий рабочему потоку о необходимости остановки
	// Возвращаем true, если значение флага изменилось, и false, если нет
	bool SetStopping(bool stopping);

	// Установлен ли флаг, сообщающий о необходимости завершения работы
	bool IsStopping() const;

private:
	std::jthread m_thread;
	mutable std::mutex m_mutex;
	std::atomic_bool m_rendering{ false };
	std::atomic_bool m_stopping{ false };
	std::atomic_uint32_t m_totalChunks{ 0 };
	std::atomic_uint32_t m_renderedChunks{ 0 };
};