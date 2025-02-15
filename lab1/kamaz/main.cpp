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
		// можно не хранить (исправлено)
		new View(this, m_model, std::make_unique<Controller>(m_model));
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
