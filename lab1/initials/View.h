#pragma once
#include "LetterView.h"
#include "Model.h"
#include <wx/wx.h>
#include <memory>

class View final : public wxPanel
{
public:
	View(wxWindow* parent, std::shared_ptr<Model> model)
		: wxPanel(parent)
		  , m_model(std::move(model))
	{
		m_connection = m_model->DoOnUpdate([&] {
			Refresh();
		});
	}

	void Update(double const deltaTime)
	{
		m_model->Update(deltaTime);
	}

	void OnPaint(wxPaintEvent& event)
	{
		const wxPaintDC dc(this);

		m_nameLetter->Draw(dc, m_model->GetNameLetter());
		m_surnameLetter->Draw(dc, m_model->GetSurnameLetter());
		m_patronymicLetter->Draw(dc, m_model->GetPatronymicLetter());
	}

private:
	std::shared_ptr<Model> m_model;
	std::unique_ptr<LetterView> m_nameLetter = std::make_unique<NameLetter>();
	std::unique_ptr<LetterView> m_surnameLetter = std::make_unique<SurnameLetter>();
	std::unique_ptr<LetterView> m_patronymicLetter = std::make_unique<PatronymicLetter>();
	ScopedConnection m_connection;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(View, wxPanel)
	EVT_PAINT(View::OnPaint)
wxEND_EVENT_TABLE()