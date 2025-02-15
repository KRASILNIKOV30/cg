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
		image.SetType(wxBITMAP_TYPE_PNG);
		image.InitAlpha();
		Painter painter({ 800, 600 });
		// painter.DrawCircle(image, { 200, 300 }, 100, m_color);
		// painter.DrawCircleWithAntialiasing(image, { 410, 300 }, 100, m_color);
		painter.DrawCircleWithThickness(image, { 400, 300 }, 4, 4, m_color);
		// painter.DrawFillCircle(image, { 400, 300 }, 100, m_color);
		dc.DrawBitmap(wxBitmap(image), 0, 0, true);
	}

private:
	Point m_center = { 400, 300 };
	Color m_color = { 255, 255, 255 };
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
wxEND_EVENT_TABLE()