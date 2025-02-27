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

	void onButtonClick(ElType const type) const
	{
		m_model->CreateElement(type);
	}

	void onElementClick(wxMouseEvent const& event, size_t id)
	{
		m_dragElement = id;
		m_startPosition = { event.GetX(), event.GetY() };
	}

	void onMouseMove(const wxMouseEvent& event)
	{
		if (m_startPosition.has_value() && m_dragElement.has_value())
		{
			const auto [startX, startY] = m_startPosition.value();
			const auto deltaX = event.GetX() - startX;
			const auto deltaY = event.GetY() - startY;
			m_model->Move(m_dragElement.value(), { deltaX, deltaY });
			m_startPosition = { event.GetX(), event.GetY() };
		}
	}

	void onMouseUp()
	{
		m_model->Drop(m_dragElement.value());
		m_dragElement.reset();
		m_startPosition.reset();
	}

private:
	std::optional<size_t> m_dragElement;
	std::optional<Point> m_startPosition = std::nullopt;
	std::shared_ptr<Model> m_model;
};