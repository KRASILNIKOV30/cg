#pragma once
#include "Controller.h"
#include <wx/graphics.h>
#include <wx/wx.h>

constexpr int BUTTONS_IN_ROW = 7;
constexpr int IMG_SIZE = 50;
constexpr int BUTTON_WIDTH = IMG_SIZE + 20;
constexpr int BUTTON_HEIGHT = IMG_SIZE + 10;

class View final : public wxPanel
{
public:
	explicit View(wxWindow* parent, wxPanel* buttonPanel, std::shared_ptr<Model> const& model, std::unique_ptr<Controller>&& controller)
		: wxPanel(parent)
		  , m_buttonPanel(buttonPanel)
		  , m_model(model)
		  , m_controller(std::move(controller))
	{
		RenderButtons();
		m_connection = m_model->DoOnUpdate([&] {
			RenderButtons();
		});
		m_canvasConnection = m_model->DoOnCanvasUpdate([&] {
			Refresh();
		});
	}

private:
	void RenderButtons() const
	{
		int i = 0;
		m_model->ForEachElement([&](const ElementModel& element) {
			const auto [type, name, img, open] = element;
			wxBitmap bitmap("res/" + img, wxBITMAP_TYPE_JPEG);
			wxImage image = bitmap.ConvertToImage();
			image.Rescale(50, 50);
			bitmap = wxBitmap(image);
			const auto pos = wxPoint((BUTTON_WIDTH + 10) * (i % BUTTONS_IN_ROW), BUTTON_HEIGHT * (i / BUTTONS_IN_ROW));
			new wxBitmapButton(m_buttonPanel, wxID_ANY, bitmap, pos);
			++i;
		});
	}

	void OnMouseDown(wxMouseEvent& event)
	{
	}

	void OnMouseMove(wxMouseEvent& event)
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

	}

private:
	wxPanel* m_buttonPanel;
	std::shared_ptr<Model> m_model;
	std::unique_ptr<Controller> m_controller;
	ScopedConnection m_connection;
	ScopedConnection m_canvasConnection;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(View, wxPanel)
	EVT_PAINT(View::OnPaint)
	EVT_LEFT_DOWN(View::OnMouseDown)
	EVT_LEFT_UP(View::OnMouseUp)
	EVT_MOTION(View::OnMouseMove)
wxEND_EVENT_TABLE()