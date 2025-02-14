#pragma once
#include "Controller.h"
#include <wx/graphics.h>
#include <wx/wx.h>

constexpr int KAMAZ_WIDTH = 709;
constexpr int KAMAZ_HEIGHT = 395;

class View final : public wxPanel
{
public:
	explicit View(wxWindow* parent, std::shared_ptr<Model> const& model, std::shared_ptr<Controller> const& controller)
		: wxPanel(parent)
		  , m_model(model)
		  , m_controller(controller)
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
		const auto gc = std::unique_ptr<wxGraphicsContext>(wxGraphicsContext::Create(dc));

		if (!gc)
		{
			return;
		}

		const auto [x, y] = m_model->GetPosition();
		gc->Translate(x, y);

		gc->SetPen(*wxBLACK_PEN);
		gc->SetBrush(*wxTRANSPARENT_BRUSH);

		gc->SetBrush(wxBrush(wxColour(0x29, 0x29, 0x29)));
		gc->DrawRectangle(699 - 280, 236, 280, 59);
		gc->DrawRectangle(419 - 343, 236, 343, 30);

		gc->SetBrush(wxBrush(wxColour(0x5C, 0x5D, 0x58)));
		gc->DrawRectangle(89, 266, 229, 66);
		gc->DrawRectangle(89, 266, 422, 35);

		gc->SetBrush(wxBrush(wxColour(0x5D, 0x5E, 0x59)));
		gc->DrawRoundedRectangle(381, 255, 107, 58, 7);
		gc->SetBrush(wxBrush(wxColour(0x8B, 0x8A, 0x8D)));
		gc->DrawRectangle(401, 255, 11, 58);
		gc->DrawRectangle(457, 255, 11, 58);

		gc->SetBrush(wxBrush(wxColour(0x73, 0x74, 0x6F)));
		gc->DrawEllipse(140.5 - 64.5, 330.5 - 64.5, 129, 129);
		gc->DrawEllipse(279.5 - 64.5, 330.5 - 64.5, 129, 129);
		gc->DrawEllipse(585.5 - 64.5, 324.5 - 64.5, 129, 129);

		gc->SetBrush(wxBrush(wxColour(0xB5, 0xB5, 0xB3)));
		gc->DrawEllipse(140.5 - 35.5, 330.5 - 35.5, 71, 71);
		gc->DrawEllipse(279.5 - 35.5, 330.5 - 35.5, 71, 71);
		gc->DrawEllipse(585.5 - 35.5, 324.5 - 35.5, 71, 71);

		gc->SetBrush(wxBrush(wxColour(0x8B, 0x8B, 0x8B)));
		gc->DrawEllipse(140 - 27, 331 - 27, 54, 54);
		gc->DrawEllipse(279 - 27, 331 - 27, 54, 54);
		gc->DrawEllipse(585 - 27, 325 - 27, 54, 54);

		gc->SetBrush(wxBrush(wxColour(0x5C, 0x5D, 0x58)));
		gc->DrawRoundedRectangle(646, 242, 63, 63, 7);

		wxGraphicsPath path1 = gc->CreatePath();
		path1.MoveToPoint(704.5, 125);
		path1.AddLineToPoint(704.5, 236.5);
		path1.AddLineToPoint(516.5, 236.5);
		path1.AddLineToPoint(492, 177);
		path1.AddLineToPoint(492, 39);
		path1.AddLineToPoint(655, 39);
		path1.AddLineToPoint(704.5, 125);
		gc->SetBrush(wxBrush(wxColour(0xF0, 0x81, 0x1B)));
		gc->FillPath(path1);

		wxGraphicsPath path2 = gc->CreatePath();
		path2.MoveToPoint(575, 124.5);
		path2.AddLineToPoint(575, 54);
		path2.AddLineToPoint(642, 54);
		path2.AddLineToPoint(683, 124.5);
		path2.AddLineToPoint(575, 124.5);
		gc->SetBrush(wxBrush(wxColour(0xE7, 0xF6, 0xFC)));
		gc->FillPath(path2);

		wxGraphicsPath path3 = gc->CreatePath();
		path3.MoveToPoint(655.5, 264.5);
		path3.AddLineToPoint(640, 264.5);
		path3.AddCurveToPoint(622.8, 242.1, 591.833, 237.5, 578.5, 238);
		path3.AddCurveToPoint(519.3, 241.6, 502.5, 291.167, 501.5, 315.5);
		path3.AddLineToPoint(491, 315.5);
		path3.AddCurveToPoint(495, 247.1, 551, 228.667, 578.5, 228);
		path3.AddCurveToPoint(624.1, 228, 648.833, 252.333, 655.5, 264.5);
		path3.CloseSubpath();
		gc->SetBrush(wxBrush(wxColour(0x73, 0x74, 0x6F)));
		gc->FillPath(path3);

		wxGraphicsPath path4 = gc->CreatePath();
		path4.MoveToPoint(493, 14.5);
		path4.AddLineToPoint(421.5, 236);
		path4.AddLineToPoint(0, 236);
		path4.AddLineToPoint(0, 46);
		path4.AddLineToPoint(48, 14.5);
		path4.AddLineToPoint(467, 14.5);
		path4.AddLineToPoint(471.5, 0);
		path4.AddLineToPoint(539, 0);
		path4.AddLineToPoint(534.5, 14.5);
		path4.AddLineToPoint(493, 14.5);
		gc->SetBrush(wxBrush(wxColour(0xEE, 0x81, 0x3D)));
		gc->FillPath(path4);
	}

private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<Controller> m_controller;
	ScopedConnection m_connection;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(View, wxPanel)
	EVT_PAINT(View::OnPaint)
	EVT_LEFT_DOWN(View::OnMouseDown)
	EVT_LEFT_UP(View::OnMouseUp)
	EVT_MOTION(View::OnMouseMove)
wxEND_EVENT_TABLE()