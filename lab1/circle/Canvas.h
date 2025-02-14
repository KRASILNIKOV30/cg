#pragma once
#include "Drawing.h"

#include <wx/wx.h>

class Canvas final : public wxPanel
{
public:
	explicit Canvas(wxWindow* parent)
		: wxPanel(parent)
	{
	}

private:
	void OnPaint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);
		wxImage image(800, 600);
		Painter painter({ 800, 600 });
		painter.DrawCircle(image, m_center, m_radius, m_color);
		dc.DrawBitmap(wxBitmap(image), 0, 0, false);
	}

private:
	Point m_center = { 400, 300 };
	Color m_color = { 255, 255, 255 };
	int m_radius = 100;
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
wxEND_EVENT_TABLE()