#include "Model.h"
#include "View.h"
#include <memory>
#include <vector>
#include <wx/wx.h>
#include <wx/splitter.h>

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 800;

class MyFrame final : public wxFrame
{
public:
	MyFrame()
		: wxFrame(nullptr, wxID_ANY, "Initials", wxDefaultPosition, wxSize(WIDTH, HEIGHT))
	{
		wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
		wxPanel* buttonPanel = new wxPanel(splitter);

		splitter->SetSashGravity(0.5);

		m_model = std::make_shared<Model>(elements);
		const auto view = new View(splitter, buttonPanel, m_model, std::make_unique<Controller>(m_model));
		splitter->SplitVertically(buttonPanel, view);
	}

private:
	std::shared_ptr<Model> m_model;
};

class MyApp final : public wxApp
{
public:
	bool OnInit() override
	{
		m_frame = new MyFrame;
		m_frame->Show(true);
		return true;
	}

private:
	MyFrame* m_frame = nullptr;
};

wxIMPLEMENT_APP(MyApp);
