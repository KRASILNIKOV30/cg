#pragma once
#include <wx/event.h>
#include "Model.h"
#include <optional>

class Controller
{
public:
	explicit Controller(std::shared_ptr<Model> const& model)
		: m_model(model)
	{
	}

	void onMouseDown(wxMouseEvent const& event)
	{
		m_startPosition = { event.GetX(), event.GetY() };
	}

	void onMouseMove(const wxMouseEvent& event)
	{
		if (m_startPosition.has_value())
		{
			const auto [curX, curY] = m_model->GetPosition();
			const auto [startX, startY] = m_startPosition.value();
			const auto deltaX = event.GetX() - startX;
			const auto deltaY = event.GetY() - startY;
			const auto x = curX + deltaX;
			const auto y = curY + deltaY;
			m_model->SetPosition({ x, y });
			m_startPosition = { event.GetX(), event.GetY() };
		}
	}

	void onMouseUp()
	{
		m_startPosition.reset();
	}

private:
	std::optional<Point> m_startPosition = std::nullopt;
	std::shared_ptr<Model> m_model;
};