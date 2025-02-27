#pragma once
#include "Controller.h"
#include "Drawing.h"

#include <wx/graphics.h>
#include <wx/wx.h>

constexpr int KAMAZ_WIDTH = 709;
constexpr int KAMAZ_HEIGHT = 395;

class View final : public wxPanel
{
public:
	explicit View(wxWindow* parent, std::shared_ptr<Model> const& model, std::unique_ptr<Controller>&& controller)
		: wxPanel(parent)
		  , m_model(model)
		  , m_controller(std::move(controller))
	{
		m_connection = m_model->DoOnUpdate([&] {
			Refresh();
		});
	}

private:
	void OnMouseDown(wxMouseEvent& event)
	{
		const auto [x, y] = m_model->GetPosition();
		const int top = y;
		const int left = y;
		const int right = x + KAMAZ_WIDTH;
		const int bottom = y + KAMAZ_HEIGHT;
		const int evtX = event.GetX();
		const int evtY = event.GetY();
		if (evtX >= left && evtX <= right && evtY >= top && evtY <= bottom)
		{
			m_controller->onMouseDown(event);
		}
	}

	void OnMouseMove(const wxMouseEvent& event)
	{
		m_controller->onMouseMove(event);
	}

	void OnMouseUp(wxMouseEvent& event)
	{
		m_controller->onMouseUp();
	}

	void OnPaint(wxPaintEvent& event)
	{
		const wxPaintDC dc(this);
		const auto gc = std::unique_ptr<wxGraphicsContext>(wxGraphicsContext::Create(dc));

		if (!gc)
		{
			return;
		}

		// в отдельный класс (исправлено)
		const auto [x, y] = m_model->GetPosition();
		gc->Translate(x, y);
		DrawKamaz(gc.get());
	}

private:
	std::shared_ptr<Model> m_model;
	std::unique_ptr<Controller> m_controller;
	ScopedConnection m_connection;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(View, wxPanel)
	EVT_PAINT(View::OnPaint)
	EVT_LEFT_DOWN(View::OnMouseDown)
	EVT_LEFT_UP(View::OnMouseUp)
	EVT_MOTION(View::OnMouseMove)
wxEND_EVENT_TABLE()