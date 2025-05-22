#pragma once
#include "FrameBuffer.h"
#include "IColorBuffer.h"

/*
Реализация интерфейса IColorBuffer, осуществляющая графический вывод в буфер кадра, представленный
классом CFrameBuffer
*/
class ColorBuffer final : public IColorBuffer
{
public:
	explicit ColorBuffer(FrameBuffer& frameBuffer)
		: m_frameBuffer(frameBuffer)
	{
	}

	void SetPixelColor(unsigned x, unsigned y, std::uint32_t color) override
	{
		m_frameBuffer.SetPixel(x, y, color);
	}

private:
	FrameBuffer& m_frameBuffer;
};