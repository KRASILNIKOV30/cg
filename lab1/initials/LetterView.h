#pragma once
#include <wx/dc.h>
#include "Model.h"
#include <wx/dcclient.h>
#include <wx/graphics.h>

class LetterView
{
public:
	void Draw(const wxPaintDC& dc, Letter const& letter) const
	{
		const auto gc = std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create(dc));
		if (!gc)
		{
			return;
		}

		const auto x = static_cast<int>(letter.position.x);
		const auto y = static_cast<int>(letter.position.y);
		gc->Translate(x, y);
		Draw(gc, letter);
	}

	virtual void Draw(std::shared_ptr<wxGraphicsContext> gc, Letter const& letter) const = 0;

	virtual ~LetterView() = default;
};

class NameLetter final : public LetterView
{
public:
	void Draw(std::shared_ptr<wxGraphicsContext> gc, const Letter& letter) const override
	{
		const auto [width, height] = letter.size;

		wxGraphicsPath path = gc->CreatePath();

		path.MoveToPoint(width, 0);
		path.AddLineToPoint(0, 0);
		path.AddLineToPoint(0, height);
		path.AddLineToPoint(width / 5, height);
		path.AddCurveToPoint(width * 1.25, height, width * 1.25, height * 0.35, width / 5, height * 0.35);
		path.AddLineToPoint(0, height * 0.35);

		gc->SetPen(wxPen(*wxBLACK, 8));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->StrokePath(path);
	}
};

class SurnameLetter final : public LetterView
{
public:
	void Draw(std::shared_ptr<wxGraphicsContext> gc, const Letter& letter) const override
	{
		const auto [width, height] = letter.size;

		wxGraphicsPath path = gc->CreatePath();

		path.MoveToPoint(0, 0);
		path.AddLineToPoint(0, height);
		path.MoveToPoint(0, height / 2);
		path.AddLineToPoint(width, 0);
		path.MoveToPoint(0, height / 2);
		path.AddLineToPoint(width, height);

		gc->SetPen(wxPen(*wxBLACK, 8));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->StrokePath(path);
	}
};

class PatronymicLetter final : public LetterView
{
public:
	void Draw(std::shared_ptr<wxGraphicsContext> gc, const Letter& letter) const override
	{
		const auto [width, height] = letter.size;

		wxGraphicsPath path = gc->CreatePath();

		path.MoveToPoint(width, 0);
		path.AddLineToPoint(width, height);
		path.MoveToPoint(width, 0);
		path.AddLineToPoint(width * 4 / 5, 0);
		path.AddCurveToPoint(width * -0.25, 0, width * -0.25, height * 0.65, width * 4 / 5, height * 0.65);
		path.AddLineToPoint(width, height * 0.65);
		path.MoveToPoint(width * 4 / 5, height * 0.65);
		path.AddLineToPoint(0, height);

		gc->SetPen(wxPen(*wxBLACK, 8));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->StrokePath(path);
	}
};