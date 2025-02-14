#include "Model.h"
#include "View.h"
#include "Controller.h"
#include <memory>
#include <vector>
#include <wx/wx.h>

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 800;

class MyFrame final : public wxFrame
{
public:
	MyFrame()
		: wxFrame(nullptr, wxID_ANY, "Initials", wxDefaultPosition, wxSize(WIDTH, HEIGHT))
	{
		m_model = std::make_shared<Model>();
		m_controller = std::make_shared<Controller>(m_model);
		m_view = std::make_shared<View>(this, m_model, m_controller);
	}

private:
	std::shared_ptr<View> m_view;
	std::shared_ptr<Controller> m_controller;
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
