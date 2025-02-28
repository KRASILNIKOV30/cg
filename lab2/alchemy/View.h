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
		RenderSortButton();
		m_connection = m_model->DoOnUpdate([&](std::optional<std::string> newElement) {
			RenderButtons();
			ShowMessage(newElement);
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
			image.Rescale(IMG_SIZE, IMG_SIZE);
			bitmap = wxBitmap(image);
			const auto pos = wxPoint((BUTTON_WIDTH + 10) * (i % BUTTONS_IN_ROW), BUTTON_HEIGHT * (i / BUTTONS_IN_ROW));
			const auto button = new wxBitmapButton(m_buttonPanel, wxID_ANY, bitmap, pos);
			button->Bind(wxEVT_BUTTON, [type, this](wxCommandEvent const& _) {
				m_controller->onButtonClick(type);
			});
			++i;
		});
	}

	void RenderSortButton() const
	{
		const auto button = new wxButton(m_buttonPanel, wxID_ANY, "Sort", wxPoint(270, 700));
		button->Bind(wxEVT_BUTTON, [this](wxCommandEvent const& _) {
			m_controller->Sort();
		});
	}

	void OnMouseDown(wxMouseEvent& event)
	{
		m_model->ForEachCanvasElementReverse([&](const CanvasElement& element) {
			const auto [x, y] = element.position;
			const auto deltaX = event.GetPosition().x - x;
			const auto deltaY = event.GetPosition().y - y;
			if (deltaX >= 0 && deltaX <= IMG_SIZE && deltaY >= 0 && deltaY <= IMG_SIZE)
			{
				m_controller->onElementClick(event, element.id);
				return false;
			}
			return true;
		});
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
		wxPaintDC dc(this);
		const auto trashPosition = m_model->GetTrashPosition();
		DrawImage(dc, "trash.png", trashPosition, wxBITMAP_TYPE_PNG);
		m_model->ForEachCanvasElement([&](const CanvasElement& element) {
			const auto [id, type, name, img, position] = element;
			DrawImage(dc, img, position, wxBITMAP_TYPE_JPEG);

			const auto [x, y] = position;
			DrawText(dc, { x, y + IMG_SIZE + 5 }, name);

			return true;
		});
	}

	static void DrawImage(wxPaintDC& dc, std::string const& src, Point const& position, const wxBitmapType type)
	{
		wxBitmap bitmap("res/" + src, type);
		wxImage image = bitmap.ConvertToImage();
		image.Rescale(IMG_SIZE, IMG_SIZE);
		bitmap = wxBitmap(image);
		const auto pos = wxPoint(position.x, position.y);
		dc.DrawBitmap(bitmap, pos);
	}

	static void DrawText(wxPaintDC& dc, Point const& position, std::string const& text)
	{
		const auto pos = wxPoint(position.x, position.y);
		dc.DrawText(text, pos);
	}

	void ShowMessage(std::optional<std::string> const& element)
	{
		if (!element.has_value())
		{
			return;
		}
		wxMessageDialog dialog(
			this,
			"You have discovered a new element called " + element.value(),
			"New element discovered!");
		dialog.ShowModal();
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