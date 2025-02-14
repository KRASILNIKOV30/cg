#include "Model.h"
#include "View.h"

#include <memory>
#include <utility>
#include <vector>
#include <wx/wx.h>

class MyFrame final : public wxFrame
{
public:
	MyFrame(std::shared_ptr<Model> model, const int width, const int height)
		: wxFrame(nullptr, wxID_ANY, "Initials", wxDefaultPosition, wxSize(width, height))
		  , m_model(std::move(model))
	{
		m_view = std::make_shared<View>(this, m_model);
		SetSize(width, height);

		m_timer = new wxTimer(this, wxID_ANY);
		m_timer->Start(16);
	}

	void OnTimer(wxTimerEvent& event)
	{
		const auto currentTime = wxGetLocalTimeMillis().ToDouble();
		const auto deltaTime = (currentTime - lastTime) / 1000;
		lastTime = currentTime;

		m_view->Update(deltaTime);
	}

private:
	std::shared_ptr<View> m_view;
	std::shared_ptr<Model> m_model;
	wxTimer* m_timer;
	double lastTime = wxGetLocalTimeMillis().ToDouble();

	wxDECLARE_EVENT_TABLE();
};

class MyApp final : public wxApp
{
public:
	bool OnInit() override
	{
		m_model = std::make_shared<Model>();
		const auto [width, height] = m_model->GetCanvasSize();
		m_frame = std::make_unique<MyFrame>(m_model, width, height);
		m_frame->Show(true);
		return true;
	}

private:
	std::shared_ptr<Model> m_model;
	std::unique_ptr<MyFrame> m_frame;
};

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_TIMER(wxID_ANY, MyFrame::OnTimer)
wxEND_EVENT_TABLE()
